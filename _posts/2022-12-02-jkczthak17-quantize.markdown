---
layout: post
title: "Jacob et al (2017) Quantization and Training of Neural Networks for Efficient Integer-Arithmetic-Only Inference"
date: Fri, 02 Dec 2022 09:39:57 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Quantization and Training of Neural Networks for Efficient Integer-Arithmetic-Only Inference"
  author: "Benoit Jacob and Skirmantas Kligys and Bo Chen and Menglong Zhu and Matthew Tang and Andrew Howard and Hartwig Adam and Dmitry Kalenichenko"
  booktitle: "Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition (CVPR)"
  pages: "2704--2713"
  year: 2018
  arxiv: 1712.05877
  url: https://openaccess.thecvf.com/content_cvpr_2018/html/Jacob_Quantization_and_Training_CVPR_2018_paper.html
---

This is the paper behind TensorFlow Lite quantization.
The objective of the paper is to make inference of a machine learning model
faster, especially on mobile devices. This problem can be solved using a
network architecture that fits the computation and memory contraints, such as
MobileNet, SqueezeNet, ShuffleNet, and DenseNet. Or it can be solved by
reducing 32-bit floating point operations into lower bit-depth, such as in
ternary weight networks and XNOR-net. This paper is on the latter approach.

Previous work on quantization focused only on the weights, i.e., to mitigate
the on-device storage limitations. This, however, addresses the computation
cost. Several situations are observed:

- floating point operations are expensive and most hardware can do integer arithmetics much faster
- multiplications are expensive but only when the operands are wide (e.g., 32-bit)
- power-of-2 multiplications can be done using bit shifts

The quantization in the paper is to find a bit representation $$q$$ for real
number $$r$$. The real number $$r$$ is indeed a 32-bit float, and the the bit
representation $$q$$ is usually uint8. The conversion is affine transform:

$$r = S\times (q-Z)$$

where $$S,Z\in\mathbb{R}$$ are the quantization parameters. There is an
additional requirement that $$q$$ can represent exact zero value $$Z$$.

The quantation is done on weight, bias, and activation. The weight and
activation are usually unit8 while bias is int32. The weights in a neural
network is usually involved in mult-add operations. Take, for example,
multiplication of two square matrices $$R_3=R_1R_2$$ which each element of the
matrices are $$r_\alpha^{(i,j)}$$, then by the rule of matrix multiplication

$$
\begin{aligned}
r_3^{(i,j)} &= \sum_{k=1}^N r_1^{(i,k)}r_2^{(k,j)} \\
S_3(q_3^{(i,j)} - Z_3) &= \sum_{k=1}^N S_1(q_1^{(i,k)} - Z_1) S_2(q_2^{(k,j)} - Z_2) \\
q_3^{(i,j)} &= Z_3 + \frac{S_1S_2}{S_3} \sum_{k=1}^N (q_1^{(i,k)} - Z_1) (q_2^{(k,j)} - Z_2) \\
&= Z_3 + \frac{S_1S_2}{S_3} \big( NZ_1Z_2 - Z_1\sum_{k=1}^N q_2^{(k,j)} - Z_2\sum_{k=1}^N q_1^{(i,k)} + \sum_{k=1}^N q_1^{(i,k)} q_2^{(k,j)} \big) \\
&= Z_3 + M \big( NZ_1Z_2 - Z_1\sum_{k=1}^N q_2^{(k,j)} - Z_2\sum_{k=1}^N q_1^{(i,k)} + \sum_{k=1}^N q_1^{(i,k)} q_2^{(k,j)} \big)
\end{aligned}
$$

where $$M := S_1S_2/S_3$$. The final equation above is significant because the
first two summations are in $$O(N)$$ only and across the entire matrix-matrix
multiplication, there are totally $$2N^2$$ such additions because it shares
across the entire row or column. The last summation is unique to each $$(i,j)$$
and there are $$2N^3$$ arithmetic operations in total. But these are all in
uint8 so even the overall complexity is $$O(N^3)$$, it is still fast.

The quantity $$M$$ is a real number, empirically in the interval $$(0,1)$$. We
can rewrite $$M$$ as

$$M = 2^{-n}M_0$$

which $$M_0\in [0.5,1)$$. In this case, we can further make $$M_0$$ an int32
with $$M_0 \ge 2^30$$ and convert the multiplication by $$M$$ an fixed-point
multiplication (i.e., integer multiplicaation then bit shift). Therefore, the
summation in the equations above should accumulate in int32 while the
multiplication operands are in uint8. Similarly, it is natural to use int32 for
bias as well. But also, using a bias of higher precision can help reducing
quantization error. To facilitate addition of bias, we set

$$
\begin{aligned}
S_{\text{bias}} &= S_1S_2 \\
Z_{\text{bias}} &= 0
\end{aligned}
$$

The handling of activation function is interesting. The most common activation
function in deep networks are ReLU (or ReLU6, which is defined as
$$\max(0, \min(6, x))$$). It is indeed a saturating function. If we make the
quantization appropriately, we can simply make the *saturating cast* of int32
to uint8 the equivalent of ReLU. Therefore, one goal of the quantized training
process is to learn how to use the entire range of uint8 (i.e, 0 to 255) so the
activation function fit perfectly with the satuation cast.

The training process is as follows: The network model is as before and weights
are in float. But we added teh quantization layer after each layer to *simulate
quantization effect* in the forward-pass while the backpropagation is as usual.
The quantization layer is to introduce the rounding behavior for the training.

After  the training, we can simply make

$$
\begin{aligned}
\text{clamp}(r; a,b) &= \min(\max(x,a), b) \\
s(a,b,n) &= \frac{b-a}{n-1} \\
q(r; a,b,n) &= \big\lfloor\frac{\text{clamp}(r;a,b)}{s(a,b,n)}\rceil s(a,b,n) + a
\end{aligned}
$$

and set $$a=\min w$$ and $$b=\max w$$ for weights $$w$$ (but do some nudge to
make quantization use only $$-127$$ to $$+127$$ and make zero exactly
representable). It would be less trivial for the activation function and we
need some input samples from the training data to observe the actual range of
activation.

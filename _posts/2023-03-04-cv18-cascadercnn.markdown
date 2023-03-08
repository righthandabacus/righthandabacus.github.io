---
layout: post
title: "Cai & Vasconcelos (2018) Cascade R-CNN: Delving into High Quality Object Detection"
date: 2023-03-03
category: paper
bibtex:
  type: inproceedings
  title: "Cascade R-CNN: Delving into High Quality Object Detection"
  author: "Zhaowei Cai and Nuno Vasconcelos"
  booktitle: "Proc CVPR"
  pages: "6154--6162"
  year: 2018
  note: "arXiv:1712.00726"
---

This paper proposed the cascaded architecture to improve the bounding box
quality in object detection. It is to improve the Faster R-CNN framework, which
is run in two stages.

In Faster R-CNN, the RPN subnetwork (H0) will infer preliminary detection
hypothesis, a.k.a. proposals, from image (see Figure 3 in paper). Then each
proposal are processed by region of interest detection subnetwork (H1), a.k.a.
detection head to produce a classification score C and bounding box B.

Bounding box regression is to transform bounding box $$b=(b_x, b_y, b_w, b_h)$$
into target bounding box $$g$$. The network is to minimize

$$
\sum_{i=1}^N L_{\text{loc}}(f(x_i, b_i), g_i)
$$

where $$x_i$$ is the image patch and $$b_i$$ is the corresponding bounding box,
and $$g_i$$ is the ground-truth. The loss function $$L_{\text{loc}()$$, was L2
in R-CNN and smoothed L1 in Fast R-CNN. The loss function measures on the
scale- and location-invariant distance vector $$(\delta_x, \delta_y, \delta_w, \delta_h)$$

$$
\begin{aligned}
\delta_x &= (g_x - b_x)/b_w \\
\delta_y &= (g_y - b_y)/b_h \\
\delta_w &= \log(g_w / b_w) \\
\delta_h &= \log(g_h / b_h)
\end{aligned}
$$

To further improve the effectiveness of regression, these quantities may be
standardized by its corresponding mean and variance.

Iterative bounding box regression is to apply the regression step multiple
times, i.e.,

$$
f'(x, b) = f\circ f\circ \dots \circ f(x, b)
$$

where the regressor in the detection head $$f()$$ is identical. But since
$$f()$$ is trained on first step, it is found to have no benefit beyond applying
$$f()$$ twice.

Classification, on the other hand, is a function $$h(x)$$ that assigns image
patch $$x$$ to one of the $$M+1$$ classes (by convention, class 0 is the
background) in terms of posterior distribution $$p(y=k\mid x)$$ over the
classes, where $$y$$ is the class label and $$k\in\{0,\dots,M\}$$. At this part,
the loss function is simply the cross-entropy,

$$
\sum_{i=1}^N L_{\text{cls}}(h(x_i), y_i)
$$

The issue here is how to assign the ground truth label $$y$$ to a bounding box.
Usually it is based on the IoU threshold, i.e.,

$$
y = \begin{cases}
  g_y & \text{IoU}(x, g) \ge u \\
  0   & \text{otherwise}
\end{cases}
$$

where $$g_y$$ is the label for the ground truth bounding box $$g$$ and $$x$$ the
proposed bounding box of which it bears the assigned label $$y$$. Usually we
settle with $$u=0.5$$ but the trade off is:

- if $$u$$ is high, less background is in the bounding box $$x$$ but less number
  of $$x$$ you can find, hence the training dataset would not have enough
  positive samples
- if $$u$$ is low, more diversified positive samples are found in the training
  dataset but the detector trained is less likely to reject close false
  positives

One solution to this problem is to create multiple classifier, each for a
different $$u\in U$$ (e.g., the paper used $$U=\{.5, .6, .7\}$$). Then the
overall classification loss is their sum:

$$
L_{\text{cls}}(h(x),y) = \sum_{u\in U} L_{\text{cls}}(h_u(x), y_u)
$$

but with different $$u$$, the number of positive samples are different and hence
this sum is skewed. Moreover, with less positive samples for high $$u$$ (i.e.,
the high quality classifiers), they are easily overfitted.

The solution in this paper is to use distinct heads (H1, H2 and H3) which make
the regressor a cascade of specialized regressors:

$$
f(x, b) = f_T \circ f_{T-1} \circ \dots \circ f_1(x, b)
$$

such that each regressor $$f_t()$$ is optimized for the output of $$f_{t-1}()$$,
hence the regressors improve the proposal progressively. The cascaded regressor
is a resampling that changes the bounding box proposal distribution, as
illustrated in Figure 4 in the paper. Therefore we can obtain the improved
samples for training as if they are seen in inference.

Because the regression resampled the bounding boxes, as we moved further into
the stages, the box distribution tiles more heavily towards high quality
examples (i.e., higher IoU). Then we can have the classifier in each stage $$t$$
with a higher IoU threshold $$u^t > u^{t-1}$$ and the loss function at each
stage is defined as

$$
\begin{aligned}
L(x^t, g) &= L_{\text{cls}}(h_t(x^t),y^t) + \lambda[y^t\ge 1]L_{\text{loc}}(f_t(x^t, b^t), g) \\
b^t &= f_{t-1}(x^{t-1}, b^{t-1})
\end{aligned}
$$

with $$\lambda=1$$ the weight between classification and localization and
$$[\cdot]$$ the indicator function. Classification result in each head affects
the next stage in the sense of which bounding box is chosen (see Fig 3d in the
paper).

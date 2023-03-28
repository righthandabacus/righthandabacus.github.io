---
layout: post
title: "Lin et al (2017) Focal Loss for Dense Object Detection"
date: 2023-03-15
category: paper
bibtex:
  type: inproceedings
  title: "Focal Loss for Dense Object Detection"
  author: "Tsung-Yi Lin and Priya Goyal and Ross Girshick and Kaiming He and Piotr Dollar"
  booktitle: "Proc CVPR"
  pages: "2980--2988"
  year: 2017
  note: "arXiv:1708.02002"
---

This is the paper proposed RetinaNet and also the *focal loss* function to
better train object detection models. Object detection models are in two camps,
there are two-stage proposal-driven models such as R-CNN, and one-stage
detector such that YOLO and SSD. The paper claimed that the prior result on
one-stage model is not as good as two-stage because of class imbalance during
training.

In two-stage models, the first stage generates a sparse set of candidate
proposals and the second stage classifies the proposals into classes. One-stage
detectors, however, need to evaluate 10K to 100K candidate locations per image
but only a few locations contain objects, whereas two-stage models would reduce
this to 1K to 2K with 1:3 ratio of positive to negative proposals. The easy
negatives can overwhelm training and lead to degenerate models. Therefore the
solution is to down-weight easy examples (a.k.a. *inliers*) such that their
contribution to total loss is small even if their number is large.

The loss function proposed is as follows:

$$
\begin{aligned}
p_t &= \begin{cases}
    p & \text{if }y=1\\
    1-p & \text{otherwise}
\end{cases}\\
\text{FL}(p_t) &= -(1-p_t)^\gamma \log(p_t)
\end{aligned}
$$

Which $$p$$ is the predicted probability in classification and $$y$$ is the
one-hot target (0 or 1). Therefore $$p_t$$ is the probability of truth, which
ideally should be 1 if $$y=1$$ and 0 otherwise. The cross entropy for a sample
is therefore $$-\log(p_t)$$, but the log function decays too gently as $$p_t$$
approaches 1. The modulating factor $$(1-p_t)^\gamma$$ is to make the loss
function decays faster. It degenerates into the simple cross entropy at
$$\gamma=0$$ and the paper suggested $$\gamma=2$$ while it is found relatively
robust in $$\gamma\in[0.5,5]$$. This helps to make loss closer to zero once
$$p_t$$ is large enough (i.e. ,$$p_t>0.5$$).

To test this proposal, the paper suggested a RetinaNet model. It combines a
backbone network with two task-specific subnetworks, such that:

- The backbone, such as ResNet, connects to feature pyramid network (FPN). The
  ResNet backbone provides a bottom-up pathway to produce feature maps, and FPN
  is the top-down pathway to combine feature maps of different resolution into a
  multiscale feature pyramid
  - RetinaNet use ResNet feature maps $$C_3$$ to $$C_5$$ with FPN to produce
    pyramid levels $$P_3$$ to $$P_7$$, which $$P_k$$ has resolution $$1/2^k$$
    of the input
  - ResNet residual stage $$C_3$$ to $$C_5$$ contributes to $$P_3$$ to $$P_5$$
    using FPN top-down pathway; but $$P_6$$ is obtained via a 3×3 stride-2 conv
    on $$P_5$$, and $$P_7$$ is from ReLU then another 3×3 stride-2 conv on
    $$P_6$$
  - All pyramid levels have $$C=256$$ channels
- Anchor boxes similar to that of RPN are produced from the FPN, with areas of
  32² to 512² on $$P_3$$ to $$P_7$$ and aspect ratios 1:2, 1:1, 2:1. Optionally
  at each layer, anchor boxes are scaled to the ratio of
  $$2^0, 2^{1/3}, 2^{2/3}$$ of its original size. In total $$A=3\times 3=9$$
  anchors are produced at each spatial location.
  - Each anchor is assigned a length $$K$$ one-hot vector as classification
    target and length $$4K$$ vector as bbox regression target
  - anchor label: compare bbox to ground-truth using IoU, positive samples for
    $$>0.5$$ and background for $$\in[0,0.4)$$
- Classification subnet: Take the input feature map of $$C=256$$ channels at
  each pyramid level $$P_k$$, then
  - pass through 4× of: 3×3 conv with $$C$$ channels, then ReLU
  - then 3×3 conv with $$KA$$ channels, then sigmoid activation to produce
    binary classification of each anchor
- Regression subnet: Parallel to and same design as classification subnet,
  except the output is length $$4A$$ and with linear activation
  - output length is $$4A$$ instead of $$4KA$$: the bounding box regression is
    class agnostic but found to be equally effective
- Model output $$A$$ anchors per pyramid level and spatial location, but to
  trade-off for speed, only decode the bbox of the top-scoring 1K predictions
  - at training, focal loss applied to all 100K anchors per image, normalized by
    the number of anchors assigned to a ground-truth box
  - detector confidence threshold: 0.05
  - then run non-maximum suppression, with threshold of 0.5
- New layers in the model are initialized with:
  - bias are zeroed, except the final conv layer for classification to use
    $$b=-\log\frac{1-\pi}{\pi}$$ with $$\pi=0.01$$ as the initial probability
    of finding a foreground class
  - weights are Gaussian with $$\sigma=0.01$$

The network is trained with a minibatch size of 16 images, 90K steps, weight
decay of 10e-4, momentum of 0.9, and initial learning rate of $$10^{-2}$$ but
tuned down to $$10^{-3}$$ at step 60K and further to $$10^{-4}$$ at step 80K.
The overall loss function is the sum of focal loss for classification and
smoothed L1 loss for regression.

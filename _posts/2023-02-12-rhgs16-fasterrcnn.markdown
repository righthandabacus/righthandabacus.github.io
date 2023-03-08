---
layout: post
title: "Ren, He, Girshick, Sun (2016) Faster R-CNN"
date: 2023-02-12
category: paper
bibtex:
  type: inproceedings
  title: "Faster R-CNN. Towards Real-Time Object Detection with Region Proposal Networks"
  author: "Shaoquing Ren and Kaiming He and Ross Girshick and Jian Sun"
  booktitle: "Proc NIPS"
  month: Jan
  year: 2016
  note: "arXiv:1506.01497"
---

This is the Faster R-CNN paper. It improves over Fast R-CNN and R-CNN in the
sense that region proposals are generated from the image. It combines a *region
proposal network* (RPN) with the Fast R-CNN detection network so the
convolutional network part is shared. It eliminated the use of region proposal
algorithms such as Selective Search or EdgeBoxes.

## Region Proposal Network

RPN is a fully convolutional network (FCN). It simultaneously regress region bonds and
objectness scores *at each location* on a regular grid.

It operates on an image (of any size) and outpus a set of rectangular object
proposals, each with an objectness score. The paper built the RPN based on
ZF-net (5 shareable convolutional layers) or VGG-16 (13 shareable convolutional
layers). An input image transformed into output feature map by the ConvNet. Then
an $$n\times n$$ spatial window ($$n=3$$) is sliding on the feature map and
produce a feature vector (256-dim for ZF-net and 512-dim for VGG-16, with ReLU
activation). The feature vector is then feed into two sibling fully connected
layers (implemented as 1×1 conv layers), one for box regression and another for
box classification.

At each sliding window location, $$k$$ region proposals are predicted. The
output of the classification layer is $$2k$$ scores to product the probability
of foreground object or background. The regression layer outputs $$4k$$
encoding, 4 for each region bounding box, and the regression weight of each box
is not shared. The regions are derived from *anchors* (i.e., primitive bounding
boxes from which refined into the final region bounding boxes). An anchor is at
the centre of the sliding window (usually at centre of a square pixel for odd
$$n$$). The paper used 3 scales and 3 aspect ratios, yielding $$k=9$$ anchors at
each position. For a feature map of $$H\times W$$ there would be $$HWk$$ anchors
in total.

The approach in this paper creates a *pyramid of anchors*, in which anchors of
different (i.e., 3) scales are stacked together at the same centre. The input
and ConvNet depends on image of only one scale until at the final classification
and regression layers.

The detail of the network architecture is as follows:

- Input image: Resized to shorter side as 600 pixels
- VGG-16 or ZF-net is used, which the last convolutional layer output has a
  stride size of 16 (i.e., 16 pixels in the input image contributed to 1 pixel
  at the output feature map)
- For 3 scales of anchors, the areas are fixed to $$128^2$$, $$256^2$$,
    and $$512^2$$ pixels
- The 3 aspect ratios of anchors are 1:1, 1:2, and 2:1. That is, for pixel are
  of $$N^2$$, the anchors are $$N\times N$$, $$N/\sqrt{2}\times 2N/\sqrt{2}$$,
  and $$2N/\sqrt{2}\times N/\sqrt{2}$$

## Loss Function for RPN

The RPN only predicts a bounding box for its objectness. Therefore, each anchor
is assigned a binary classification label.

An anchor is positive if (1) the anchor has IoU $$\ge 0.7$$ with any
ground-truth bounding box, or in case not found, (2) the anchor has highest IoU
to a ground-truth bounding box. Any single ground-truth bounding box may assign
positive labels to multiple anchors.

An anchor is negative if IoU $$\le 0.3$$ for all ground-truth boxes. If an
anchor is neither positive nor negative, it is not contributed to the loss
function computation in training.

The loss function for an image is

$$
L(\{p_i\}, \{t_i\}) = \frac{1}{N_{\text{cls}}} \sum_i L_{\text{cls}}(p_i,
p_i^ast) + \lambda \frac{1}{N_{\text{reg}}} \sum_i p_i^\ast L_{\text{reg}}(t_i, t_i^\ast)
$$

where

- $$i$$ is index of an anchor in a mini-batch
- $$p_i$$ is the predicted probability of anchor $$i$$ being an object
- $$p_i^\ast\in\{0,1\}$$ is the objectness ground-truth (only for positive and
    negative anchors)
- $$t_i,t_i^\ast$$ are the 4 parameterized coordinates of the bounding box,
    predicted and ground-truth respectively: (subscript $$a$$ refers to anchor
    and superscript $$\ast$$ refers to ground-truth)

$$
\begin{aligned}
t_x      &= (x-x_a)/w_a        &   t_y       &= (y-y_a)/h_a \\
t_x^\ast &= (x^\ast-x_a)/w_a   &   t_y^\ast  &= (y^\ast-y_a)/h_a \\
t_w      &= \log(w/w_a)        &   t_h       &= \log(h/h_a) \\
t_w^\ast &= \log(w^\ast/w_a)   &   t_h^\ast  &= \log(h^\ast/h_a)
\end{aligned}
$$

- $$L_{\text{cls}}()$$ is the log loss for binary classification
- $$L_{\text{reg}}()$$ is the smooth L1 loss for regression, as used in Fast
    R-CNN; $$p_i^\ast L_{\text{reg}}()$$ denotes regression applies only to
    positive anchors
- $$N_{\text{cls}}=256$$ is the size of the mini-batch
- $$N_{\text{reg}}\approx 2400$$ is the number of anchor locations
- $$\lambda=10$$ is the balancing parameter, to make classification loss and
    regression loss roughly equally weighted

Note that each pixel at feature map produces $$k$$ anchors but those anchors
that go beyond image boundary are excluded from the loss calculation. An input
image of 1000×600 pixels with convolution output at stride 16 produces roughly
60×40×9=20K anchors but after cross-boundary removal, only 6K anchors are left.

The training is based on SGD. In each mini-batch, 256 anchors are drawn from a
single image with random sample of 128 positive and 128 negative anchors. If
there are not enough positive anchors, the mini-batch is padded with more
negative anchors. The conv layers are initialized with ImageNet classification.
Learning rate of 1e-3 is used for first 60K mini-batches and then 1e-4 for next
20K mini-batches. Momentum of 0.9 and weight decay of 5e-4 are used. In case
VGG-16 is used for RPN, the layers before "conv3_1" are marked not trainable.

## Alternating Training

The convolutional network in Faster R-CNN is shared between the RPN (which
outputs refined bounding boxes from anchors and their objectness) and the Fast
R-CNN parts (which takes the bounding boxes from RPN as region proposals and run
RoI pooling, classification, and bounding box regression). There are multiple
ways to train this shared convolutional network:

1. Alternating training: Train the RPN with ImageNet-initialized CNN, then use
   the proposal from RPN to train Fast R-CNN, and use the CNN fine-tuned by Fast
   R-CNN to retrain the RPN. This process repeats.
2. Approximate join training: Make RPN and Fast R-CNN one network in training.
   At each SGD step, the forward pass generates region proposals from RPN and
   treated as fixed proposals to Fast R-CNN detector. The backward process
   propagates loss from the detector and RPN. At the shared layers, the loss are
   combined.
   - In this case, the loss at detector assumes RPN output are constant and not
     adjustable. Hence ignores the derivative of detector loss w.r.t. RPN output
3. Non-approximate join training: Consider the combined RPN and Fast R-CNN
   network and includes the derivative of detector loss w.r.t. RPN output
   - requires the RoI pooling layer that is differentiable w.r.t. RPN box
     coordinates, e.g. RoI wrapping layer

This paper uses 4-step training algorithm: (1) First train the RPN initialized
with ImageNet weights. (2) Then train a separate Fast R-CNN detection network
assuming RPN input as non-adjustable box coordinates. These two networks are
separate and the convolutional layers are not shared. (3) Afterwards, the
detector network's weight are used to initialize RPN again on the shared layers.
These shared layers are frozen and only the layers unique to RPN are fine-tuned.
(4) At the Fast R-CNN, the shared convolutional layers are keep frozen and the
other layers are fine-tuned once again.

The 4-step training algorithm can be repeated but the paper found the
improvement is negligible.

## Inference

At inference, the anchors generated by RPN will have the cross-boundary
proposals boxes clipped to the image boundary.

---
layout: post
title: "He, Gkioxari and Dollar and Girshick (2017) Mask R-CNN"
date: 2023-03-03
category: paper
bibtex:
  type: inproceedings
  title: "Mask R-CNN"
  author: "Kaiming He and Georgia Gkioxari and Piotr Dollár and Ross Girshick"
  booktitle: "Proc ICCV"
  month: Oct
  year: 2017
  note: "arXiv:1703.06870"
---

This is the Mask R-CNN paper. It extends Faster R-CNN to produce pixel masks,
i.e., for semantic segmentation. It extends Faster R-CNN by adding a branch for
predicting segmentation masks on each *Region of Interest (RoI)*. However, this
have to support with RoIAlign instead of RoIPooling, to avoid error introduced
by quantization.

The key contribution of design is to point out the importance of decoupling
mask and class prediction, i.e., inference of the binary mask should not depend
on classification result. The Mask R-CNN model runs these two tasks in parallel.

## Architecture

In Faster R-CNN, features are extracted from feature map based on a region
proposal (RoI). The feature vector is obtained from RoIPool layer, which the
feature vector is feed into classification and bounding-box regression.

In Mask R-CNN, the RPN stage is same as Faster R-CNN, but afterwards, the
classification, box offset, and binary mask prediction are run *in parallel*
for each RoI. The classification does not depend on mask prediction as in other
model, but performed in parallel.

The classification and bounding-box regression is same as that in Faster R-CNN,
but the mask prediction is to produce a $$Km^2$$-dimensional output for **each
RoI**, for $$K$$ binary masks of resolution $$m\times m$$. It is implemented as
per-pixel sigmoid using FCN.

Mask prediction requires per-pixel spatial correspondence, hence we need the
feature maps to aligned to pixels. This is done by RoIAlign: It is to produce a
small feature map (e.g., 7×7) from an arbitrary size RoI. The algorithm is as follows:

1. Given RoI, subdivide its spatial dimension $$x$$ into bins of size $$x/m$$
   (a floating point)
2. In each bin, calculate the value at, say, 4 sampling points. The 4 sampling
   points are the center intersections by drawing 3×3 grids on the bin. The
   value is computed by bilinear interpolation with neighboring pixels.
3. The value assigned to this bin is aggregated (e.g., max or average) from the
   sampling points

In training this model, the loss function is
$$L=L_{\text{cls}}+L_{\text{box}}+L_{\text{mask}}$$, which the first two are
the classification and bounding-box regression loss as in Faster R-CNN, and the
last one is the mask prediction loss defined as the average binary cross-entropy
loss of each pixel.

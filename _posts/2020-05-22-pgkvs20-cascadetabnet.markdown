---
layout: post
title: "Prasad et al (2020) CascadeTabNet"
date: 2020-05-22
category: paper
bibtex:
   type: inproceedings
   title: "CascadeTabNet: An approach for end to end table detection and structure recognition from image-based documents"
   author: Devashish Prasad and Ayan Gadpal and Kshitij Kapadni and Manish Visave and Kavita Sultanpure
   booktitle: Proceedings of CVPR2020 Workshop on Text and Documents in the Deep Learning Era
   year: 2020
   address: Seattle, WA
---

Code: https://github.com/DevashishPrasad/CascadeTabNet

Scope: Table detection and table structure recognition

This paper: CascadeTabNet, single deep learning-based end-to-end model for two problems solved at once

Architecture:

- Cascade R-CNN (Cai and Vasconcelos, 2018) is a multistage model for detection
- HRNet (Wang et al, 2019) for semantic segmentation and object detection
- Foundings: Cascaded multistaged model with HRNet backbone yields the best result
- Network (implemented using mmdetection)
  - Image feed to `HRNetV2p_W32` to produce feature map
  - RPN (region proposal network) takes feature map and predicts object proposals (B0, region of interest, RoI)
    - B0 = boudning box classification scores + box regression points
  - BBox takes RoI B0 and feature map and makes predictions (B1)
  - Another BBox heads take B1 and feature map to produce B2, and B2 to produce B3 (final bounding box)
  - A mask layer takes B2 and feature map to produce masks for objects (S, segmentation output)
    - mmdetection: `cascade_mask_rcnn_hrnetv2p_w32_20e`
  - Branching at output of B2: Produces B3 and S (from segmentation branch)

Transfer learning in 2 stages
1. General task of table detection: Using general dataset
   - images of different types of documents, containing tables of various types
   - goal: detect tables in images
2. Detect tables according to table types (bordered, semi-bordered, borderless)
   - two-stage transfer learning strategy helps to use only small amount of data

 First iteration
 - start with CNN model with pretrained ImageNet COCO model weights
 - learn only task-specific high level features
 - result: predicts the detection masks for table in images

 Second iteration
 - model fine-tuned on smaller dataset for specific tasks
   - predicting cell masks in borderless tables
   - detecting table types
- need smaller data set with advanced annotations
- semibordered and borderless tables share the same class because the border is not helpful

Image augmentation techniques
- for generating training data, and avoid overfitting
- digital documents in datasets are perfectly axis-aligned
  - should not do shear and rotation transform
- dilation transform: Thicken the black pixel regions
  - convert into binary image first
  - use a 2x2 kernel filter with one iteration to do the transform
- smudge transform: Spread black pixel to create smeary blurred black pixel region
  - implemented using various distance transforms
  - algorithm: Gilani et al (2017), using Euclidean distance transform, linear distance transform, max distance transform

Pipeline
- CascadeTabNet take images containing zero or more tables, predicts segmentation masks for tables (bordered and borderless)
- Branches: Bordered and Borderless
  - Borderless: Predict cells, then arrange into rows nad columns based on their positions. Detect cells using contour-based text detection algorithm
  - Bordered: Using line detection algorithm. No cell segmentation as line information is easier

Data
- General dataset is merge of 3:
  1. ICDAR19 (cTDaR)
  2. Marmot
  3. https://github.com/sgrpanchal31/table-detection-dataset
- Removed some ground truth errors in dataset
- Create 4 training sets
  1. original images
  2. original + dilate transform
  3. smudge transform
  4. original + dilate + smudge
- Evaluation using tablebank dataset


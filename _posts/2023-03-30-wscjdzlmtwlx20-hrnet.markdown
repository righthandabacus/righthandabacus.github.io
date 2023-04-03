---
layout: post
title: "Wang et al (2020) HRNet"
date: Thu, 30 Mar 2023 13:18:45 -0400
category: paper
bibtex:
  type: article
  title: "Deep High-Resolution Representation Learning for Visual Recognition"
  author: "Jingdong Wang and Ke Sun and Tianheng Cheng and Borui Jiang and Chaorui Deng and Yang Zhao and Dong Liu and Yadong Mu and Mingkui Tan and Xianggang Wang and Wenyu Liu and Bin Xiao"
  journal: "IEEE Transactions on Pattern Analysis and Machine Intelligence"
  pages: 3349--3364,
  volume: 43,
  number: 10,
  month: Oct,
  year: 2020
  arxiv: 1908.07919
---

Classification networks such as AlexNet, VGGNet, GoogLeNet, ResNet are all
reducing spatial size and produce a low-resolution representation.
High-resolution representation is produced in U-net, for example, using dilated
convolution and upsampling. This paper proposes HRNet that maintain
high-resolution representation through the whole process. It starts from high
resolution convolution stream and add high-to-low resolution convolution
streams gradually to run in parallel. The network is in multiple (4) stages,
which stage $$n$$ has $$n$$ parallel streams representing $$n$$ resolutions.

The representation produced by HRNet should be semantically strong and
spatially precise. The high resolution representations are boosted by fusion
with low-resolution representations.

Several versions are produced from the paper:

- HRNetV1: Only output the high-resolution representation computed from the high resolution convolution stream
- HRNetV2: Combines representations from all high-to-low resolution parallel streams. Can improve semantic segmentation
- HRNetV2p: Multi-level representations

These all share the same base architecture and only the output head differed.


## Architecture

This network assumes a high resolution image as input and the network is fully
convolutional. First, the image pass through two conv layers, each is 3×3
stride 2, result in a 1/4 resolution feature map. Then filtered by multiple
stages and conv blocks: (See table in Appendix A of the paper)

- Block: Each block is 4 conv unit, which preserved the resolution, and then a
  fusion module. The fusion module is a mesh to mix the feature maps of
  different resolutions
- Stage: Each stage can contain 1 or more blocks. In the paper, stages 1 and 2
  each has one block and stage 3 anad 4 has 4 and 3 blocks respectively. At the
  last fusion module of each stage, one additional parallel stream is created
  for halved resolution.
- Conv unit: It is multiple conv layers. At stage 1, it is 1×1 conv of 64
  channels, 3×3 conv of 64 channels, than a 1×1 conv of 256 channels. At later
  stages, each conv unit is two 3×3 conv of $$2^rC$$ channels, for
  $$r=1,2,\dots$$ the resolution index (see below).
- Width: The parameter $$C$$ in that controls number of channels in the conv
  unit (above). This is also the number of channels for the highest resolution
  feature map. The paper mentioned a $$C=128$$ model in appendix B and $$C=32$$
  to $$C=48$$ models in the body of the paper.

Input to a fusion module are multiple parallel streams $$R^{(i)}_r$$ with
$$r\in\{1,\dots,k\}$$, which index $$r$$ denotes the resolution is
$$1/2^{r-1}$$ of the input to the first stage (i.e., $$r=1$$ corresponds to the
1/4 resolution of the source, see sec 3 of the paper). Output to a fusion
module are $$R^{(o)}_r$$ with $$r\in\{1,\dots,k+1\}$$ (one more resolution).
Input-output are fully-connected: All inputs should be upsampled or downsampled
to the output resolution and summed, i.e., $$R^{(o)}_s = \sum_r f_{rs}(R^{(i)}_r)$$:

- For $$r=s$$, the resolution are the same and $$f_{rs}(R^{(i)}_r)=R^{(i)}_r$$
- For $$r<s$$, the input resolution is finer than the output, hence downsample
  is required. We filter $$R^{(i)}_r$$ with $$r-s$$ consecutive conv of 3×3
  stride 2.
- For $$r>s$$, the input resolution is coarser than the output, hence upsample
  is required. It is through bilinear upsampling followed by 1×1 conv to align
  the number of channels.

The paper suggested to have $$n=4$$ stages, which there will be 4 parallel
resolution streams. After the last stage, there will be feature maps of $$n$$
different resolution immediately from the final fusion module:

- HRNetV1 emits $$R^{(o)}_1$$ from the fusion module and ignored the rest
- HRNetV2 rescaled $$R^{(o)}_r$$ for $$r\ge 2$$ to the resolution of
  $$R^{(o)}_1$$ using bilinear upsampling and then channel-wise concatenate
  them, followed by a 1×1 conv to mix the channels. The output is a feature map
  at resolution of $$R^{(o)}_1$$.
- HRNetV2p extended HRNetV2 that downsampled its output to multiple
  resolutions. They would be useful for object detection.

At $$n=4$$, the output of the final fusion module would be at resolutions of
1/4, 1/8, 1/16, and 1/32 of the original.

## Network Pretraining

Pretrain the network on ImageNet is to take the HRNet with 4 parallel output,
for example, and downsample the higher resolution to lower by a 3×3 conv
stride-2 to match the resolution and channel depth to next level. Since stream
$$r$$ resolution has resolution of $$1/2^{r-1}$$ and $$2^rC$$ channels
($$C=128$$), each stride-2 conv halved the resolution but doubled the channel,
so the result can *element-wise add* to the next stream. Until all 4 streams
are merged as such gradually to the lowest resolution (with $$8C$$ channels),
it is pass through a 1×1 conv to transform into $$16C$$ channels, then a global
average pooling to a $$16C=2048$$ vector, which is then feed into a classifier.
See Fig 11 in Appendix B for the architecture.

The paper said this network is trained with 100 epochs with batch size of 256,
with SGD at initial learning rate of 1e-1 and reduced by a factor of 10 at
epochs 30, 60, and 90. Nesterov momentum of 0.9, weight decay of 1e-4 are used.

## Applications

Human pose estimation with HRNet is described in sec 4 of the paper. It takes
an image with the groundtruth keypoints, then rescaled the groundtruth to 1/4
of the resolution (i.e., $$(W,H)\to (W/4,H/4)$$). The keypoint locations are
transformed into heatmap using 2D Gaussian function with $$\sigma=2$$ pixels.

Set up a HRNetV1, which outputs a 1/4 resolution feature map, and treat it as a
regression to the groundtruth heatmap, trained using L2 loss function. Then the
output can be evaluated with:

- Object Keypoint Similarity (OKS), i.e. $$\dfrac{\sum_i \exp(-d_i^2/2s^2k_i^2)\delta[v_i>0]}{\sum_i \delta[v_i>0]}$$ with distance $$d_i$$, object scale $$s$$, per-keypoint constant $$k_i$$, and visibility $$v_i$$
- AP-50 (AP at OKS=0.5), AP-75 (AP at OKS=0.75)
- AP (mean of AP at OKS=0.5, 0.55, ..., 0.9, 0.95)
- AP-M (for medium objects), AP_L (for large objects)
- AR (mean of AR at OKS=0.5, 0.55, ..., 0.9, 0.95) 

The paper said this network is trained with COCO images (train2017) of fixed
size 256×192 or 384×288, using augmentation of random rotation between $$\pm
45^\circ$$, random scale between 0.65 to 1.35, and flipping. Adam optimizer
used, trained for 210 epochs, with init learning rate of 1e-3, drop to 1e-4 at
170th epoch and 1e-5 at 200th epoch.

Semantic segmentation is in sec 5 of the paper. It uses HRNetV2 model, with 15C
channels at each position. Then filter the output to a linear classifier
(softmax loss) to predict the segmentation maps. PASCAL-Context and Cityscapes
datasets are used. Mean of class-wise IoU (mIOU) is the evaluation metric.

The model is trained with images randomly cropped (from 1024×2048 to 512×1024),
randomly scaled from 0.5 to 2.0, and random horizontal flipping. SGD optimizer
with base learning rate of 0.01, momentum of 0.9, and weight decay of 5e-4 is
used, with poly learning rate policy of power of 0.9. The training runs for
120K iterations at batch size of 12.

Object detection is described in sec 6 of  the paper. HRNetV2p is used and
trained on MS COCO 2017 detection dataset (118K training images). The input
images are resized to 800px at shorter edge.

## Implementation

Official GitHub: https://github.com/HRNet

Keras implementation: https://github.com/niecongchong/HRNet-keras-semantic-segmentation

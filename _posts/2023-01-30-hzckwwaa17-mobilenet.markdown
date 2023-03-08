---
layout: post
title: "Howard et al (2017) MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications"
date: 2023-01-30
category: paper
bibtex:
  type: misc
  title: "MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications"
  author: "Andrew G. Howard and Menglong Zhu and Bo Chen and Dmitry Kalenichenko and Weijun Wang and Tobias Weyand and Macro Andreetto and Hartwig Adam"
  howpublished: "arXiv:1704.04861"
  month: Apr
  year: 2017
---

Convolutional networks such as AlexNet demonstrated the accuracy image
recognition. However, latency as well as model size (i.e., memory) can be a
concern. MobileNet proposed in this paper is to make these adjustable.


## Depthwise Separable Convolution

The key component of MobileNet is the *depthwise separable convolution* layer.
It is to break a standard convolution into a depthwise convolution and 1×1
pointwise convolution. A standard convolution is to filter and combine inputs
into output in one step. For example, a Conv2D with kernel of size
$$D_K\times D_K$$ applied on an $$M$$-channel input tensor of
$$H\times W\times M$$ is parameterized by a kernel tensor of
$$D_K\times D_K\times M\times N$$, where $$N$$ is the number of output
channels. Assume stride 1 and appropriate padding, the input tensor $$F$$ and
output tensor $$G$$ is related as

$$
G_{k,l,n} = \sum_{i,j,m} K_{i,j,m,n} \times F_{k+i, l+j, m}
$$

In other words, the input tensor is 3D and the kernel parameter is 4D. For each
output channel, the kernel is applied by scanning the width and height but
fitted completely on all input channels. The sum of the Hadamard product is
assigned to one element in that channel of the output.

Each Hadamard product has compute cost of $$D_K^2MN$$ and the output feature
map has size $$H\times W$$, resulting the total cost per convolution layer to
be $$D_K^2MNHW$$.

Depthwise separable convolution has a single-channel kernel, hence the kernel
tensor has size $$D_K\times D_K\times 1$$ only. This is applied to each channel
separately, producing an output tensor of $$H\times W\times M$$, i.e., same
number of channel as input:

$$
G'_{k,l,m} = \sum_{i,j} K_{i,j} \times F_{k+i, l+j, m}
$$

The pointwise convolution is just a convolution layer with 1×1 kernel, hence
its kernel tensor has size $$1\times 1\times M\times N$$. This is like a
fully-connected layer applied on channels, transforming $$M$$ channels in the
input to $$N$$ channels in the output:

$$
G_{k,l,n} = \sum_{m} W_{m,n} \times G'_{k,l,m}
$$

The total computation cost is $$D_K^2MWH + MNWH$$, which is a reduction of
$$1/N+1/D_K^2$$. For example, with 3×3 kernel, the reduction is roughly 1/9,
while the accuracy is only lowered a little.


## Architecture

Typically a convolution network has a building block of "3×3 Conv-BN-ReLU". In
MobileNet, this is replaced into:

    3×3 Depthwise Conv - BN - ReLU - 1×1 Conv - BN - ReLU

It is found that 94.76% of the Mult-Add operation is on the 1×1 convolutions,
and 74.59% of parameters are the 1×1 convolution weights. (Compare: 3×3
depthwise convolution contributed 3.06% Mult-Add operations and 1.06% of
parameters, the only 3×3 Conv later at input is 1.19% Mult-Add and the fully
connected layer at output is 0.18% Mult-Add, but 24.33% parameters)

The paper suggested to train the model with less regularization and data
augmentation for this model is smaller, e.g., than Inception. The model
architecture is provided by Table 1 in the paper:

| Type     | Stride | Filter Shape  | Input Size | Note       |
|----------|--------|---------------|------------|------------|
| Conv     | 2      | 3×3×3×32      | 224×224×3  | Input      |
| Conv DW  | 1      | 3×3×32 DW     | 112×112×32 |            |
| Conv     | 1      | 1×1×32×64     | 112×112×32 |            |
| Conv DW  | 2      | 3×3×64 DW     | 112×112×64 |            |
| Conv     | 1      | 1×1×64×128    | 56×56×64   |            |
| Conv DW  | 1      | 3×3×128 DW    | 56×56×128  |            |
| Conv     | 1      | 1×1×128×128   | 56×56×128  |            |
| Conv DW  | 2      | 3×3×128 DW    | 56×56×128  |            |
| Conv     | 1      | 1×1×128×256   | 28×28×128  |            |
| Conv DW  | 1      | 3×3×256 DW    | 28×28×256  |            |
| Conv     | 1      | 1×1×256×256   | 28×28×256  |            |
| Conv DW  | 2      | 3×3×256 DW    | 28×28×256  |            |
| Conv     | 1      | 1×1×256×512   | 14×14×256  |            |
| Conv DW  | 1      | 3×3×512 DW    | 14×14×512  | repeat 1   |
| Conv     | 1      | 1×1×512×512   | 14×14×512  |            |
| Conv DW  | 1      | 3×3×512 DW    | 14×14×512  | repeat 2   |
| Conv     | 1      | 1×1×512×512   | 14×14×512  |            |
| Conv DW  | 1      | 3×3×512 DW    | 14×14×512  | repeat 3   |
| Conv     | 1      | 1×1×512×512   | 14×14×512  |            |
| Conv DW  | 1      | 3×3×512 DW    | 14×14×512  | repeat 4   |
| Conv     | 1      | 1×1×512×512   | 14×14×512  |            |
| Conv DW  | 1      | 3×3×512 DW    | 14×14×512  | repeat 5   |
| Conv     | 1      | 1×1×512×512   | 14×14×512  |            |
| Conv DW  | 2      | 3×3×512 DW    | 14×14×512  | repeat end |
| Conv     | 1      | 1×1×512×1024  | 7×7×512    |            |
| Conv DW  | 2      | 3×3×1024 DW   | 7×7×1024   |            |
| Conv     | 1      | 1×1×1024×1024 | 7×7×1024   |            |
| Avg pool | 1      | 7×7 Pool      | 7×7×1024   |            |
| FC       |        | 1024×1000     | 1024       |            |
| Softma×  |        | Classifier    | 1000       |            |

In the paper, the model is tested with ImageNet and found to achieve 70.6%
accuracy, down 1% by converting between full convolution and the depthwise
separable convolution in the design (i.e., 3×3×64×128 Conv vs vs 3×3×64 DW +
1×1×64×128 Conv).

The pooling layer before the fully-connected layer is a
*2D global average pooling* layer. It takes an input tensor of shape
(batch, height, width, channel) and output a tensor of shape (batch, channel),
which the value of each element is the average of all spatial data in the same
channel.


## Scaling Parameters

The width multiplier $$\alpha$$ allows adjusting the number of channels. For
the baseline design above, where each layer has $$M$$ input channels and $$N$$
output channels, it becomes $$\alpha M$$ input channels and $$\alpha N$$ output
channels (rounded), which typically $$\alpha\in [1, 0.75, 0.5, 0.25]$$.
Setting $$\alpha$$ makes the total number of parameters as well as number of
Mult-Add scaled by $$\alpha^2$$.

There is also a resolution parameter $$\rho$$, which scales the input
resolution (hence normally not part of the model). Typically, MobileNet takes
input size the square of 224, 192, 160, or 128. Setting $$\rho$$ scales both
the number of Mult-Add and number of parameters to $$\rho^2$$.

The paper evaluated and found that decreasing $$\alpha$$ or $$\rho$$ drops the
accuracy smoothly.


## Keras Implementation

In Tensorflow, the MobileNet model is built-in. This is how you can plot a model:

```python
import tensorflow as tf

model = tf.keras.applications.MobileNet()
# print summary
model.summary(line_length=120)
# save model to `model.png`
tf.keras.utils.plot_model(model, show_shapes=True, show_dtype=True,
                          show_layer_names=True, expand_nested=True,
                          show_layer_activations=True)
```

This generates `model.png` like [this](/img/mobilenet-keras.png). The MobileNet function signature (and parameter default) are:

```python
tf.keras.applications.MobileNet(
    input_shape=None,
    alpha=1.0,
    depth_multiplier=1,
    dropout=0.001,
    include_top=True,
    weights="imagenet",
    input_tensor=None,
    pooling=None,
    classes=1000,
    classifier_activation="softmax",
    **kwargs
)
```

And its model summary is:

```text
Model: "mobilenet_1.00_224"
________________________________________________________________________________________________________________________
 Layer (type)                                         Output Shape                                    Param #
========================================================================================================================
 input_1 (InputLayer)                                 [(None, 224, 224, 3)]                           0

 conv1 (Conv2D)                                       (None, 112, 112, 32)                            864

 conv1_bn (BatchNormalization)                        (None, 112, 112, 32)                            128

 conv1_relu (ReLU)                                    (None, 112, 112, 32)                            0

 conv_dw_1 (DepthwiseConv2D)                          (None, 112, 112, 32)                            288

 conv_dw_1_bn (BatchNormalization)                    (None, 112, 112, 32)                            128

 conv_dw_1_relu (ReLU)                                (None, 112, 112, 32)                            0

 conv_pw_1 (Conv2D)                                   (None, 112, 112, 64)                            2048

 conv_pw_1_bn (BatchNormalization)                    (None, 112, 112, 64)                            256

 conv_pw_1_relu (ReLU)                                (None, 112, 112, 64)                            0

 conv_pad_2 (ZeroPadding2D)                           (None, 113, 113, 64)                            0

 conv_dw_2 (DepthwiseConv2D)                          (None, 56, 56, 64)                              576

 conv_dw_2_bn (BatchNormalization)                    (None, 56, 56, 64)                              256

 conv_dw_2_relu (ReLU)                                (None, 56, 56, 64)                              0

 conv_pw_2 (Conv2D)                                   (None, 56, 56, 128)                             8192

 conv_pw_2_bn (BatchNormalization)                    (None, 56, 56, 128)                             512

 conv_pw_2_relu (ReLU)                                (None, 56, 56, 128)                             0

 conv_dw_3 (DepthwiseConv2D)                          (None, 56, 56, 128)                             1152

 conv_dw_3_bn (BatchNormalization)                    (None, 56, 56, 128)                             512

 conv_dw_3_relu (ReLU)                                (None, 56, 56, 128)                             0

 conv_pw_3 (Conv2D)                                   (None, 56, 56, 128)                             16384

 conv_pw_3_bn (BatchNormalization)                    (None, 56, 56, 128)                             512

 conv_pw_3_relu (ReLU)                                (None, 56, 56, 128)                             0

 conv_pad_4 (ZeroPadding2D)                           (None, 57, 57, 128)                             0

 conv_dw_4 (DepthwiseConv2D)                          (None, 28, 28, 128)                             1152

 conv_dw_4_bn (BatchNormalization)                    (None, 28, 28, 128)                             512

 conv_dw_4_relu (ReLU)                                (None, 28, 28, 128)                             0

 conv_pw_4 (Conv2D)                                   (None, 28, 28, 256)                             32768

 conv_pw_4_bn (BatchNormalization)                    (None, 28, 28, 256)                             1024

 conv_pw_4_relu (ReLU)                                (None, 28, 28, 256)                             0

 conv_dw_5 (DepthwiseConv2D)                          (None, 28, 28, 256)                             2304

 conv_dw_5_bn (BatchNormalization)                    (None, 28, 28, 256)                             1024

 conv_dw_5_relu (ReLU)                                (None, 28, 28, 256)                             0

 conv_pw_5 (Conv2D)                                   (None, 28, 28, 256)                             65536

 conv_pw_5_bn (BatchNormalization)                    (None, 28, 28, 256)                             1024

 conv_pw_5_relu (ReLU)                                (None, 28, 28, 256)                             0

 conv_pad_6 (ZeroPadding2D)                           (None, 29, 29, 256)                             0

 conv_dw_6 (DepthwiseConv2D)                          (None, 14, 14, 256)                             2304

 conv_dw_6_bn (BatchNormalization)                    (None, 14, 14, 256)                             1024

 conv_dw_6_relu (ReLU)                                (None, 14, 14, 256)                             0

 conv_pw_6 (Conv2D)                                   (None, 14, 14, 512)                             131072

 conv_pw_6_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_pw_6_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_dw_7 (DepthwiseConv2D)                          (None, 14, 14, 512)                             4608

 conv_dw_7_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_dw_7_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_pw_7 (Conv2D)                                   (None, 14, 14, 512)                             262144

 conv_pw_7_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_pw_7_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_dw_8 (DepthwiseConv2D)                          (None, 14, 14, 512)                             4608

 conv_dw_8_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_dw_8_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_pw_8 (Conv2D)                                   (None, 14, 14, 512)                             262144

 conv_pw_8_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_pw_8_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_dw_9 (DepthwiseConv2D)                          (None, 14, 14, 512)                             4608

 conv_dw_9_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_dw_9_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_pw_9 (Conv2D)                                   (None, 14, 14, 512)                             262144

 conv_pw_9_bn (BatchNormalization)                    (None, 14, 14, 512)                             2048

 conv_pw_9_relu (ReLU)                                (None, 14, 14, 512)                             0

 conv_dw_10 (DepthwiseConv2D)                         (None, 14, 14, 512)                             4608

 conv_dw_10_bn (BatchNormalization)                   (None, 14, 14, 512)                             2048

 conv_dw_10_relu (ReLU)                               (None, 14, 14, 512)                             0

 conv_pw_10 (Conv2D)                                  (None, 14, 14, 512)                             262144

 conv_pw_10_bn (BatchNormalization)                   (None, 14, 14, 512)                             2048

 conv_pw_10_relu (ReLU)                               (None, 14, 14, 512)                             0

 conv_dw_11 (DepthwiseConv2D)                         (None, 14, 14, 512)                             4608

 conv_dw_11_bn (BatchNormalization)                   (None, 14, 14, 512)                             2048

 conv_dw_11_relu (ReLU)                               (None, 14, 14, 512)                             0

 conv_pw_11 (Conv2D)                                  (None, 14, 14, 512)                             262144

 conv_pw_11_bn (BatchNormalization)                   (None, 14, 14, 512)                             2048

 conv_pw_11_relu (ReLU)                               (None, 14, 14, 512)                             0

 conv_pad_12 (ZeroPadding2D)                          (None, 15, 15, 512)                             0

 conv_dw_12 (DepthwiseConv2D)                         (None, 7, 7, 512)                               4608

 conv_dw_12_bn (BatchNormalization)                   (None, 7, 7, 512)                               2048

 conv_dw_12_relu (ReLU)                               (None, 7, 7, 512)                               0

 conv_pw_12 (Conv2D)                                  (None, 7, 7, 1024)                              524288

 conv_pw_12_bn (BatchNormalization)                   (None, 7, 7, 1024)                              4096

 conv_pw_12_relu (ReLU)                               (None, 7, 7, 1024)                              0

 conv_dw_13 (DepthwiseConv2D)                         (None, 7, 7, 1024)                              9216

 conv_dw_13_bn (BatchNormalization)                   (None, 7, 7, 1024)                              4096

 conv_dw_13_relu (ReLU)                               (None, 7, 7, 1024)                              0

 conv_pw_13 (Conv2D)                                  (None, 7, 7, 1024)                              1048576

 conv_pw_13_bn (BatchNormalization)                   (None, 7, 7, 1024)                              4096

 conv_pw_13_relu (ReLU)                               (None, 7, 7, 1024)                              0

 global_average_pooling2d (GlobalAveragePooling2D)    (None, 1, 1, 1024)                              0

 dropout (Dropout)                                    (None, 1, 1, 1024)                              0

 conv_preds (Conv2D)                                  (None, 1, 1, 1000)                              1025000

 reshape_2 (Reshape)                                  (None, 1000)                                    0

 predictions (Activation)                             (None, 1000)                                    0

========================================================================================================================
Total params: 4,253,864
Trainable params: 4,231,976
Non-trainable params: 21,888
________________________________________________________________________________________________________________________
```

The factory function `MobileNet()` accepts parameter `alpha` as the width
multiplier $$\alpha$$ and `depth_multiplier` is the resolution parameter
$$\rho$$. By default, the model created has the weight initialized for ImageNet
training set, hence the default input shape is (224, 224, 3).

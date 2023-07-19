---
layout: post
title: "Ledig et al (2017) Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network"
date: 2023-05-30
lastupdate: 2023-06-10
category: paper
bibtex:
  type: inproceedings
  title: "Photo-Realistic Single Image Super-Resolution Using a Generative Adversarial Network"
  author: "Christian Ledig and Lucas Theis and Ferenc Huszár and Jose Caballero and Andrew Cunningham and Alejandro Acosta and Andrew Aitken and Alykhan Tejani and Johannes Totz and Zehan Wang and Wenzhe Shi"
  booktitle: "Proc. CVPR"
  year: 2017
  pages: 4681--4690
  arXiv: 1609.04802
---

This is the "SR-GAN" paper. The problem of upscaling a photo with details is called "SISR" as in the title. This paper takes a 4x upscaling as an example problem and build a GAN model to do it.

The optimization target for a super-resolution algorithm is usually MSE between the pixels of the high-resolution image (HR) and that of the super-resolution output (SR). Minimizing the MSE is effectively minimizing the PSNR, or SSIM. But MSE cannot capture *perceptual* differences, such as texture details. The major contribution of this paper is to introduce a *perceptual loss* function using the feature maps of the VGG network.

The SR-GAN is using a CNN architecture to process images. Some key design components in the CNN:
- Deep CNN is difficult to train but has a potential for better accuracy. Batch normalization can help counteract the covariate shift inside the model
- Skip-connection relieved the network of modeling the identity mapping, which is non-trivial in a convolutional kernel

## GAN Model
Adversarial min-max problem:

$$
\min_{\theta_G} \max_{\theta_D}
\Big\{
\mathbb{E}_{I^{\text{HR}}\in T^{\text{HR}}}\Big[\log D_{\theta_D}(I^{\text{HR}})\Big]
+
\mathbb{E}_{I^{\text{LR}}\in T^{\text{LR}}}\Big[\log\Big(1-D_{\theta_D}\big(G_{\theta_G}(I^{\text{LR}})\big)\Big)\Big]
\Big\}
$$
- $\theta_D$ and $\theta_G$ are the model parameters of the discriminator and the generator respectively; to be learned
- $T^{\text{HR}}$ and $T^{\text{LR}}$ are the set of hi-res and lo-res images
- above is same as the binary crossentropy loss from the discriminator

Model design: (Fig.4 in the paper on page 5)
- Parametric ReLU activation is used on the generator so the activation can be learned
- Sub-pixel convolution is used for upscaling
- LeakyReLU (with $\alpha=0.2$) is used throughout the discriminator so max-pooling is avoided (Radford et al, 2016)
- At discriminator, stride-2 convolution is used to half the image resolution but whenever it is applied, the number of features is doubled

The key to GAN training is the *perceptual loss* function, defined as equation (3) in the paper:
$$
l^{\text{SR}} = l_X^{\text{SR}} + 10^{-3} l_{\text{Gen}}^{\text{SR}}
$$
which $l_X^{\text{SR}}$ is the content loss (based on VGG19 network features) and $l_{\text{Gen}}^{\text{SR}}$ is the adversarial loss (based on discriminator).

The content loss $l_X^{\text{SR}}$ is modeled after pixel-wise MSE loss, which is shown to positively correlate with PSNR. However, MSE loss on pixels tend to overly smooth textures as it fail to account for the high frequency content. The paper propose to do MSE loss on feature output from VGG19 of the layer $\phi_{5,4}$, i.e., the 4th conv layer in the block preceding the 5th pooling layer (note not the 4th conv layer from beginning, but the 4th in that block starting from an activation layer). There are $C=512$ feature channels. The MSE is computing elementwise.

The adversarial loss or the generative loss $l_{\text{Gen}}^{\text{SR}}$ is the cross entropy over all training samples:
$$
l_{\text{Gen}}^{\text{SR}} =
- \sum_{n=1}^N
\log D_{\theta_D}\big(G_{\theta_G}(I^{\text{LR}})\big)
$$
which we expects the discriminator $D_{\theta_G}(\cdot)$ produces sigmoidal output.

## Training
The model is trained as follows: BSD300 dataset is used as the test set. The model is designed for a scale factor of $4\times$ or $16\times$ the pixel count. PSNR (in dB) and SSIM are used as the evaluation metric. The model is compared to the upscaling algorithms nearest neighbor, bicubic, SRCNN (Dong et al, 2014), and SelfExSR (Huang et al, 2015).

Training set is a random sample of 350K images from ImageNet database, which LR images are from downsampling the HR images (RGB) using bicubic $4\times$ downsampling. Then 16 random $96\times 96$ subimages are cropped from distinct image samples for a mini-batch.

The LR images (input to generator) are in pixel range of $[0,1]$ and HR images (output from generator) are in range of $[-1,1]$. The VGG output features is scaled by a factor of $1/12.75$ to make the MSE on VGG features comparable to pixel MSE loss.

Training is using Adam optimizer ($\beta_1=0.9$) with learning rate of $10^{-4}$ for the first 100K update steps and learning rate $10^{-5}$ for another 100K update steps.

## Implementation

There are quite a number of implementations on the web. Below is what I polished from various sources:

```python
#!/usr/bin/env python
# coding: utf-8

"""
Based on the paper
"""

import os

import cv2
import numpy as np
import tensorflow as tf
import tqdm
from tensorflow.keras.layers import \
        Input, Conv2D, LeakyReLU, BatchNormalization, Flatten, Dense, PReLU, Add, UpSampling2D
from tensorflow.keras.models import Model
from tensorflow.keras.losses import BinaryCrossentropy, binary_crossentropy, mean_squared_error
from tensorflow.keras.optimizers import Adam, SGD


#
# Data generator
#

def make_dataset(image_dir, hires_size=(256,256), lores_size=(64,64), batch_size=8):
    """Tensorflow dataset of batches of (lores,hires) images"""
    hires = tf.keras.utils.image_dataset_from_directory(image_dir, labels=None,
                                                        color_mode="rgb",
                                                        image_size=hires_size,
                                                        batch_size=None)
    hires = hires.batch(batch_size, drop_remainder=True)
    lores = hires.map(lambda nhwc: tf.image.resize(nhwc, lores_size))
    dataset = tf.data.Dataset.zip((hires, lores))
    return dataset


#
# Discriminator
#

def discriminator_block(input, n_filters, strides=1, bn=True, name_prefix=""):
    """Repeated discriminator block. Batch normalization is not used on the first block"""
    y = Conv2D(n_filters, (3, 3), strides, padding="same", name=name_prefix+"_conv")(input)
    if bn:
        y = BatchNormalization(momentum=0.8, name=name_prefix+"_bn")(y)
    y = LeakyReLU(alpha=0.2, name=name_prefix+"lrelu")(y)
    return y


def discriminator_model(input, name="discriminator"):
    """The complete discriminator that takes an input image and output a logit value"""
    n_filters = 64
    # k3n64s1 and k3n64s2
    y = discriminator_block(input, n_filters, bn=False, name_prefix="block1")
    y = discriminator_block(y, n_filters, strides=2, name_prefix="block2")
    # k3n128s1 and k3n128s2
    y = discriminator_block(y, n_filters*2, name_prefix="block3")
    y = discriminator_block(y, n_filters*2, strides=2, name_prefix="block4")
    # k3n256s1 and k3n256s2
    y = discriminator_block(y, n_filters*4, name_prefix="block5")
    y = discriminator_block(y, n_filters*4, strides=2, name_prefix="block6")
    # k3n512s1 and k3n512s2
    y = discriminator_block(y, n_filters*8, name_prefix="block7")
    y = discriminator_block(y, n_filters*8, strides=2, name_prefix="block8")
    # Dense layers and logit output
    y = Flatten(name="flatten")(y)
    y = Dense(n_filters*16, name="fc1")(y)
    y = LeakyReLU(alpha=0.2, name="lrelu")(y)
    output = Dense(1, name="fc2")(y)  # no sigmoid act, to make logit output
    return Model(inputs=input, outputs=output, name=name)


#
# Generator
#

def residual_block(input, name_prefix=""):
    """Residual block in generator"""
    # two layers of k3n64s1
    y = Conv2D(64, (3, 3), padding="same", name=name_prefix+"_conv1")(input)
    y = BatchNormalization(momentum=0.5, name=name_prefix+"_bn1")(y)
    y = PReLU(shared_axes=[1, 2], name=name_prefix+"_prelu")(y)
    y = Conv2D(64, (3, 3), padding="same", name=name_prefix+"_conv2")(y)
    y = BatchNormalization(momentum=0.5, name=name_prefix+"_bn2")(y)
    y = Add(name=name_prefix+"_add")([input, y])   # skip connection
    return y


def upscale_block(input, name_prefix=""):
    """Upscale the image 2x, used at the end of the generator network
    """
    # k3n256s1
    y = Conv2D(256, (3, 3), padding="same", name=name_prefix+"_conv")(input)
    y = tf.nn.depth_to_space(y, 2)  # 2x upsampling
    y = PReLU(shared_axes=[1, 2], name=name_prefix+"_prelu")(y)
    return y


def generator_model(input, num_res_blocks=16, name="generator"):
    """Create the generator model of SR-GAN for 4x super-resolution"""
    # k9n64s1 and PReLU layer before the residual block
    y = Conv2D(64, (9, 9), padding="same", name="entry_conv")(input)
    y = PReLU(shared_axes=[1, 2], name="entry_prelu")(y)
    # B times the residual blocks
    res_input = y
    for n in range(num_res_blocks):
        y = residual_block(y, name_prefix=f"residual{n}")
    # k3n64s1 Conv+BN block
    y = Conv2D(64, (3, 3), padding="same", name="mid_conv")(y)
    y = BatchNormalization(momentum=0.5, name="mid_bn")(y)
    y = Add(name="mid_add")([y, res_input])
    # two upscale blocks
    y = upscale_block(y, name_prefix="up1")
    y = upscale_block(y, name_prefix="up2")
    # k9n3s1 conv at output
    output = Conv2D(3, (9, 9), padding="same", name="out_conv")(y)
    return Model(inputs=input, outputs=output, name=name)


#
# VGG model for content loss
#


def vgg_model(output_layer=20):
    """Create VGG19 model for measuring the perceptual loss
    """
    # take VGG model from Keras, output at layer "block5_conv4" (20),
    # paper referred this layer as \phi_{5,4}
    vgg = tf.keras.applications.VGG19(input_shape=(None, None, 3), weights="imagenet", include_top=False)
    model = Model(inputs=vgg.input, outputs=vgg.layers[output_layer].output, name="VGG19")
    model.trainable = False      # need model.compile()
    for layer in model.layers:
        layer.trainable = False  # no need model.compile()
    return model


#
# Training
#


def save_weights(generator, discriminator, epoch, basedir="checkpoint"):
    """Syntax sugar for saving the generator and discriminator models"""
    os.makedirs(basedir, exist_ok=True)
    gen_path = os.path.join(basedir, f"generator_{epoch}.h5")
    disc_path = os.path.join(basedir, f"discriminator_{epoch}.h5")
    generator.save(gen_path)
    discriminator.save(disc_path)


def main():
    image_dir = "dataset_images"
    batch_size = 8
    n_epochs = 100

    # try to build and print the discriminator
    hr_input = Input(shape=(256, 256, 3))
    discriminator = discriminator_model(hr_input)
    discriminator.summary(line_length=120, expand_nested=True, show_trainable=True)

    # try to build and print the generator (1/4 size of the discriminator input)
    lr_input = Input(shape=(64, 64, 3))
    generator = generator_model(lr_input)
    generator.summary(line_length=120, expand_nested=True, show_trainable=True)

    # VGG model to reuse for feature extraction during loss calculation
    vgg = vgg_model()
    vgg.summary(line_length=120, expand_nested=True, show_trainable=True)

    # The loss metrics
    ones = tf.ones(batch_size)
    zeros = tf.ones(batch_size)

    def content_loss(hires, supres):
        """Use VGG model to compare features extracted from hires and supre-res images.
        Keras VGG model expects "caffe" image format (BGR, mean-shifted), hence
        preprocess_input() is required. This function is for use with model.compile()

        Args:
            hires: Hires image, pixels in [0,255]
            supres: Generator output, pixels in [0,1] supposedly

        Returns:
            tf.Tensor of a scalar value
        """
        supres = tf.keras.applications.vgg19.preprocess_input(tf.clip_by_value((supres+1)*127.5, 0, 255))
        hires = tf.keras.applications.vgg19.preprocess_input(hires)
        hires_feat = vgg(hires, training=False) / 12.75
        supres_feat = vgg(supres, training=False) / 12.75
        return tf.math.reduce_mean(tf.math.squared_difference(hires_feat, supres_feat))

    disc_loss = BinaryCrossentropy(from_logits=True)

    def gan_loss(hires, supres):
        """Generator perceptual loss = content loss + 1e-3 * adversarial loss"""
        disc_output = discriminator(supres, training=False)
        content = content_loss(hires, supres)
        adversarial = disc_loss(ones, disc_output)
        return content + 1e-3 * adversarial

    # Optmizers for use in training: Separate because these optimizers are stateful
    gen_opt = Adam(learning_rate=1e-4, beta_1=0.9, beta_2=0.999, epsilon=1e-8)
    disc_opt = Adam(learning_rate=1e-4, beta_1=0.9, beta_2=0.999, epsilon=1e-8)

    # compile models
    generator.compile(loss=gan_loss, optimizer=gen_opt)
    discriminator.compile(loss=disc_loss, optimizer=disc_opt)

    # training loop
    dataset = make_dataset(image_dir, batch_size=batch_size).prefetch(tf.data.AUTOTUNE)

    p_mean = tf.keras.metrics.Mean()   # to average perceptual loss
    d_mean = tf.keras.metrics.Mean()   # to average discriminator loss

    for epoch in range(n_epochs):
        with tqdm.tqdm(dataset, unit="step", desc=f"Epoch {epoch}") as tqdmbar:
            for hires_batch, lores_batch in tqdmbar:
                # train the discriminator; generator input is [0,1] output is [-1,1]
                lores_batch /= 255.0
                supres_batch = generator(lores_batch, training=False)  # output pixel [-1,1]
                disc_loss0 = discriminator.train_on_batch(supres_batch, zeros)
                disc_loss1 = discriminator.train_on_batch(hires_batch/127.5-1, ones) # convert [0,255] -> [-1,1]

                # train the generator
                percep_loss = generator.train_on_batch(lores_batch, hires_batch)

                p_mean.update_state(percep_loss)
                d_mean.update_state(disc_loss0+disc_loss1)
                tqdmbar.set_postfix(percep=f"{p_mean.result():.3f}",
                                    disc=f"{d_mean.result():.3f}")

        # save model at end of each epoch
        save_weights(generator, discriminator, epoch+1)
        p_mean.reset_states()
        d_mean.reset_states()


main()
```

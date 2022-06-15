---
layout: post
title: "Dey et al (2017) SigNet"
date: 2022-06-13
category: paper
bibtex:
  type: unpublished
  title: "SigNet: Convolutional Siamese Network for Writer Independent Offline Signature Verification"
  author: "Sounak Dey and Anjan Dutta and J. Ignacio Toledo and Suman K.Ghosh and Josep Lladós and Umapada Pal"
  year: 2017
  arxiv: 1707.02131.pdf
---

This is a paper proposing a siamese network for signature verification.
Essentially, it can also be applied to other image matching tasks. The network,
called SigNet, has the main branch to encode a signature image into a vector.
With the signature pair, the encoded vectors are compared using contrastive
loss function. The loss function value is treated as the score for the
signature match.

The paper mentioned several sources of signature dataset. Including the [CEDAR
signature database](http://www.cedar.buffalo.edu/NIJ/data/signatures.rar),
which has 1320 genuine and forgery signatures for 55 signers each, and [GPDS
synthetic signature database](http://www.gpds.ulpgc.es/download), which has
hundreds to thousands of signers but need to sign a license to download.

As input to the siamese network, the signature images are resized to 155x220
pixels using bilinear interpolation. The paper suggested to invert the image to
make the background pixel value 0 (i.e., white on black) and then normalize the
pixel value with the standard deviation of the pixel values of that image.

The main branch of the siamese network is a deep convolutional network with
kernel sizes 11x11 to 3x3 with ReLU activation and interleaved by pooling
layers. The network is fed with two input images to output two vectors as a
dimensionality reduction network. The Euclidean distance of the two vectors is
then computed. It is assumed that matching signature pair will be close to each
other. Hence the Euclidean distance is used as the similarity metric. The
siamese network is regarded as a "weakly supervised metric learning".

The loss function for the network is the *contrastive loss* (See [the
paper](http://yann.lecun.com/exdb/publis/pdf/chopra-05.pdf) or [this
post](https://lilianweng.github.io/posts/2021-05-31-contrastive/)). The formula
is:

$$
L(s_1, s_2, y) = \alpha(1-y)D_w^2 + \beta y \max(0, m-D_w)^2
$$

where $$s_1,s_2$$ are the two images and $$y$$ is a binary indicator for
whether the two images are matching signatures. The quantity $$m$$ is a margin
and $$D_w$$ is the Euclidean distance. If the main branch of the siamese
network is $$f()$$, which converts an image to a vector, then
$$D_w = \lVert f(s_1) - f(s_2) \rVert_2$$. The contrastive loss defined above
means when $$y=0$$ or the images match, we should expect a small $$D_w$$. But
if $$y=1$$ or the images not match, we expect $$m-D_w$$ to be small, i.e., the
Euclidean distance should close to or even beyond $$m$$. The coefficients
$$\alpha,\beta$$ are to weight the contribution by positive and negative
samples. The siamese network is trained to move the two vectors closer if they
are matched pairs or move them away otherwise.

The paper mentioned a network architecture like the following, but using local
response normalization (from AlexNet) instead of batch normalization. The
reference code from the author is at
<https://github.com/sounakdey/SigNet/blob/master/SigNet_v1.py>. It is rewritten
into the following to use modern TensorFlow:

```python
from tensorflow.keras import Sequential, Model, Input
from tensorflow.keras.layers import InputLayer, Conv2D, BatchNormalization, \
                                    MaxPool2D, ZeroPadding2D, Dropout, Flatten, \
                                    Dense, Lambda
from tensorflow.keras.regularizers import L2
import tensorflow.keras.backend as K

IMAGE_SHAPE=(155,220,3)

def make_base_signet():
    seq = Sequential([
        InputLayer(input_shape=IMAGE_SHAPE),

        Conv2D(96, 11, strides=4, activation="relu", name="conv1_1",
               kernel_initializer="glorot_uniform", data_format="channels_last"),
        BatchNormalization(axis=-1, momentum=0.9, epsilon=1e-6), # feature normalization
        MaxPool2D(pool_size=(3,3), strides=(2,2)),
        ZeroPadding2D(padding=(2,2), data_format="channels_last"),

        Conv2D(256, 5, strides=1, activation="relu", name="conv2_1",
               kernel_initializer="glorot_uniform", data_format="channels_last"),
        BatchNormalization(axis=-1, momentum=0.9, epsilon=1e-6),
        MaxPool2D(pool_size=(3,3), strides=(2,2)),
        Dropout(rate=0.3),
        ZeroPadding2D(padding=(1,1), data_format="channels_last"),

        Conv2D(384, 3, strides=1, activation="relu", name="conv3_1",
               kernel_initializer="glorot_uniform", data_format="channels_last"),
        ZeroPadding2D(padding=(1,1), data_format="channels_last"),

        Conv2D(256, 3, strides=1, activation="relu", name="conv3_2",
               kernel_initializer="glorot_uniform", data_format="channels_last"),
        MaxPool2D(pool_size=(3,3), strides=(2,2)),
        Dropout(rate=0.3),

        Flatten(name="flatten"),
        Dense(1024, activation="relu", kernel_regularizer=L2(l2=5e-4),
              kernel_initializer='glorot_uniform'),
        Dropout(rate=0.5),

        Dense(128, activation="relu", kernel_regularizer=L2(l2=5e-4),
              kernel_initializer='glorot_uniform'),
    ], name="base_network")
    return seq

def sq_euclid_dist(pair):
    """Squared Euclidean distance between two vectors"""
    a, b = pair
    return K.sum(K.square(a - b), axis=-1)

def contrastive(y_true, y_pred):
    '''Contrastive loss from Hadsell-et-al.'06
    http://yann.lecun.com/exdb/publis/pdf/hadsell-chopra-lecun-06.pdf

    If similar (y_true==0), prefer zero Euclidean distance
    If dissimilar (y_true==1), prefer Euclidean distance at max
    '''
    maxdist = 1.0
    similar = tf.cast(1-y_true, tf.float32) * y_pred
    dissimilar = tf.cast(y_true, tf.float32) * tf.maximum(maxdist - y_pred, 0)
    return tf.reduce_mean(similar + dissimilar, axis=-1)

# Build Siamese network
base_network = make_base_signet()

input_a = Input(shape=IMAGE_SHAPE, name="input_a")
input_b = Input(shape=IMAGE_SHAPE, name="input_b")
output_a = base_network(input_a)
output_b = base_network(input_b)
distance = Lambda(sq_euclid_dist, name="dist")([output_a, output_b])

model = Model(inputs=[input_a, input_b], outputs=distance)
model.compile(loss=contrastive,
              optimizer=tf.keras.optimizers.RMSprop(learning_rate=1e-4, rho=0.9, epsilon=1e-8))
```

The code above is not difficult to train. And surprisingly quite easy to get
close to perfect prediction capability with the dataset. While the paper
mentioned about the preprocessing of the image such as normalizing the pixel
values, it seems unnecessary due to the batch normalization layer after the
first convolutional layer.

In case of training, the paper mentioned the case of imbalanced data: There are
55 signer in the CEDAR dataset and 24 each for genuine and forgery. So the
total pairs of genuine match per signer would be 24C2 = 276. But for
forgery-genuine pair, there would be 24x24=576. If we count the case of
*unskilled forged signatures*, i.e., using the genuine signature of a different
signer to pair up as a forged pair, there would be even more. The paper
suggested to use subsampling to handle this.

The output of the siamese network is the distance. We need to find a threshold
for genuine/forge decision. This can be done by picking a point from the ROC curve.

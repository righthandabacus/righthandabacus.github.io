---
layout: post
title: "An Intuitive Analogy to Attention Operation"
date: 2025-02-20
tags: math
---

I [tried to explain what is attention](/2023-05-21-attention/), but there should
be a simpler explanation of what the attention operation is actually doing.
Let's focus on the core operation of attention, namely:

$$
O = \textrm{softmax}\big(\frac{QK^\top}{\sqrt{d}}\big) V
$$

which all symbols are tensors. Let's consider $Q$ a vector of RGB values, e.g.,

$$
Q = (83, 36, 120)
$$

and $K$ is a matrix of several RGB values, e.g.,

$$
K = \begin{bmatrix}
    254 & 240 & 217 \\
    253 & 204 & 138 \\
    252 & 141 &  89 \\
    215 &  48 &  31 \\
    246 & 239 & 247 \\
    189 & 201 & 225 \\
    103 & 169 & 207 \\
      2 & 129 & 138
\end{bmatrix}.
$$

Each row in $K$ is a color. If you're curious, these are the colors:

<span style="width:1em; display:inline-block; background-color:#fef0d9;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#fdcc8a;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#fc8d59;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#d7301f;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#f6eff7;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#bdc9e1;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#67a9cf;">&nbsp;</span>
<span style="width:1em; display:inline-block; background-color:#02818a;">&nbsp;</span>

The first four are warm colors and the last four are cool colors. So we can define $V$ to indicate warm-cool:

$$
V = \begin{bmatrix}
    1 & 0 \\
    1 & 0 \\
    1 & 0 \\
    1 & 0 \\
    0 & 1 \\
    0 & 1 \\
    0 & 1 \\
    0 & 1
\end{bmatrix}.
$$

Now let's scale down $Q$ and $K$ by dividing 255 elementwise (so each RGB value
is in 0 to 1). This is handy to avoid exploding values in computing $e^x$ in
softmax. Then,

$$
\textrm{softmax}\big(\frac{QK^\top}{\sqrt{d}}\big) = (0.150, 0.128, 0.114, 0.096, 0.158, 0.140, 0.121, 0.093)
$$

and then

$$
\textrm{softmax}\big(\frac{QK^\top}{\sqrt{d}}\big) V = (0.488, 0.512)
$$

This means the color as represented by $Q$ is 48.8% warm and 51.2% cool. The color is:

<span style="width:1em; display:inline-block; background-color:#8517dc;">&nbsp;</span>

Then it is intuitive what $Q,K,V$ means: $Q$ is the query because it is the
input vector to the attention operation. $K$ is the key because it is the
representative value that $Q$ should compare to. $V$ is value. It is neither
query nor key, but might be in a different domain. What it does is to interpret
the meaning (value) of each key.

The result of softmax gives the weights to the values to sum up. Hence the
output is the weighted sum of the values according to the input.

Converting the above into code is the following:

```python
import numpy as np

Q = [133, 23, 220]
Q = np.array(Q, dtype=float) / 255.0
K = [
    [254, 240, 217],
    [253, 204, 138],
    [252, 141,  89],
    [215,  48,  31],
    [246, 239, 247],
    [189, 201, 225],
    [103, 169, 207],
    [  2, 129, 138],
]
K = np.array(K, dtype=float)  / 255.0

# scaled dot product
sdp = Q @ K.T / np.sqrt(3)
# (safe) softmax of it
maxval = max(sdp)
exp_sdp = np.exp(sdp - maxval)
softmax = exp_sdp / sum(exp_sdp)

V = [[1, 0], [1, 0], [1, 0], [1, 0], [0, 1], [0, 1], [0, 1], [0, 1]]
V = np.array(V, dtype=float)
print(softmax @ V)
```

In a transformer model, all of $Q,K,V$ are projected by a learnable projection
matrix. Also, attention layer in a transformer model is usually multi-headed,
meaning for the same input, it is projected differently to compute various
attention outputs. In the example above, the attention output finds how warm or
cool the RGB color is. The definition of warm-cool would be learned if the
projection matrix is learnable. Moreover, color may be presented in HSV space
instead of RGB and the learnable projection matrix should tell what would be
the best way to represent a color. If multihead attention unit is used, not
just warm-cool, but a multitude of properties of a color will be found.

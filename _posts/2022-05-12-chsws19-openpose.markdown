---
layout: post
title: "Cao et al (2019) OpenPose"
date: Thu, 12 May 2022 09:53:13 -0400
category: paper
bibtex:
  type: article
  title: "OpenPose: Realtime Multi-Person 2D Pose Estimation Using Part Affinity Fields"
  author: "Zhe Cao and Gines Hidalgo and Tomas Simon and Shih-En Wei and Yaser Sheikh"
  journal: "IEEE Transactions on Pattern Analysis and Machine Intelligence"
  volume: 43
  number: 1
  year: 2019
  pages: 172--186
  arxiv: 1812.08008
---

This is a library Goal of the paper: Human 2D pose estimation by identifying keypoints,
supporting multiple persons in the same image.

Single-person pose estimation can be done using top-down approach, but suffer
from the problem of early commitment (i.e., wrong bounding box fails the
subsequent steps). Examples of the top-down approach includes Mask R-CNN (He et
al, 2017) and Alpha-Pose (from "RMPE: Regional multi-person pose estimation",
Fang et al, 2017). Deepcut (Pishchulin et al, 2016) and Deepercut (Insafutdinov
et al, 2016) are previous work on the same topic using bottom-up approach, but
the challenge is on association of body parts to the same person. The
characteristics of this paper is to use *part affinity fields* (PAF) to encode
the limbs in additional to key point detection. Part affinity fields encode the
pairwise relationship between body parts of variable number of people. (Sec. 2)

## Model for PAF and confidence maps (Sec. 3.1 to 3.2)

Input is a color image of w×h pixels. It is first analyzed by a CNN, e.g.,
first 10 layers of VGG-19, to create a feature map $$F$$. Then the feature map
is used to derive the first stage of PAF,

$$
L^1 = \phi^1(F)
$$

which $$\phi^k$$ is the stage $$k$$ CNN for PAF inference. Subsequently, this
PAF inference process is repeated:

$$
L^t = \phi^t(F, L^{t-1}) \quad t = 2,\dots,T_P
$$

There are $$T_P$$ stages of pose inferences with the final output $$L^{T_P}$$.
Then there is a confidence map (i.e., heatmap of confidence of the location of
keypoints) detection:

$$
S^{T_P} = \rho^{T_P}(F, L^{T_P})
$$

and subsequently,

$$
S^t = \rho^t(F, L^{T_P}, S^{t-1}) \quad t = T_P+1, \dots, T_P+T_C
$$

so there will be $$T_P$$ of PAFs and $$T_C+1$$ of confidence maps. Each of the
$$L_t$$ will be C×w×h×2 for C vector fields (one for each "limb") and each
vector is identified by a 2-tuple. Each $$S^t$$ will be J×w×h for confidence (0
to 1) of a pixel being each of the J keypoints.

(**Note**: I believe the paper is inconsistent here with the loss evaluation. It
should be $$S^{T_P+1} = \rho^{T_P+1}(F, L^{T_P})$$ and no $$S^{T_P}$$ ever
defined. Other $$S^t$$ are defined for $$t=T_P+2,\dots,T_P+T_C$$.)

The output of PAF is used for confidence map prediction because intuitively, we
can guess the location from the PAF output.

In Sec.3.1, the paper mentioned that in each of these $$T_P+T_C$$ stages, the
original 7×7 convolutional layers are replaced with 3 consecutive 3×3 kernels.
Each output pixel of a $$N\times N$$ kernel will do $$2N^2-1$$ operations
(multiplication and addition). So each pixel from 7×7 kernel involves 97
operations while three of 3×3 kernel will be 51 operations.


## Defining the PAF and confidence map (Sec. 3.3 to 3.4)


Let $$x_{j,k}\in\mathbb{R}^2$$ be the position of person $$k$$ keypoint $$j$$
(body parts) in the groundtruth image. Then at any location
$$p\in\mathbb{R}^2$$, the value of the groundtruth confidence map
$$S_{j,k}^\ast$$ is

$$
S_{j,k}^\ast(p) = \exp\Big(-\frac{\lVert p-x_{j,k}\rVert_2^2}{\sigma^2}\Big)
$$

where $$\sigma$$ is a parameter to control the spread of the peak. Then the
groundtruth confidence map for keypoint $$j$$ is the max across multiple
persons,

$$
S_k^\ast(p) = \max_k S_{j,k}^\ast(p)
$$

so that multiple peaks can be retained in the image. (Mean of gaussians is
gaussian, but max of them is multimodal)

The part affinity fields (PAFs) is to provide location and orientation
information. Let $$x_{j_1,k}$$ and $$x_{j_2,k}$$ be the groundtruth position of
two keypoints of the same body, corresponding to limb $$c$$. We define the
length between the two points

$$
\ell_{c,k} = \lVert x_{j_2,k}-x_{j_1,k} \rVert_2
$$

and make the unit vector

$$
v = \frac{x_{j_2,k}-x_{j_1,k}}{\lVert x_{j_2,k}-x_{j_1,k} \rVert_2}
$$

then for any point $$p$$, the PAF for limb $$c$$ of person $$k$$ is

$$
L_{c,k}^\ast(p) = \begin{cases}
v & \text{if } 0\le v\cdot (p-x_{j_1,k}) \le \ell_{c,k} \text{ and }
               \vert v_{\perp} \cdot (p-x_{j_1,k}) \vert \le \sigma_{\ell} \\
0 & \text{otherwise}
\end{cases}
$$

This is to mean that on $$L_{c,k}^\ast$$, each position is either the unit
vector or zero. It is a unit vector if the position $$p$$ is within a rectangle
region from $$x_{j_1,k}$$ to $$x_{j_2,k}$$ (this rectangle is not axis-aligned,
but rotated to along the direction of $$v$$). The "width" of the rectangle is
$$2\sigma_{\ell}$$ while the "length" is $$\ell_{c,k}$$.

Then for an image of multiple persons, the groundtruth PAF is the average of
non-zero vectors across all people:

$$
L_c^\ast(p) = \frac{1}{n_c(p)} \sum_k L_{c,k}^\ast(p)
$$

where $$n_c(p)$$ is the count of non-zero vectors $$L_{c,k}(p)$$ at point $$p$$.

## Loss function and evaluation (Sec. 3.2 and 3.4)

A $$L_2$$ loss function is applied at end of each stage.

The output of stage $$t$$ of PAF inference is $$L^t$$. We evaluate this output
with the loss function:

$$
f_L^t = \sum_{c=1}^C \sum_p W(p) \cdot \lVert L_c^t(p) - L_c^\ast(p) \rVert_2^2
$$

where $$W(p)$$ is a binary mask to mean that annotation at position $$p$$ is
present (avoid comparing inference output to missing value).

Similarly, the output of stage $$t$$ of confidence map detection is $$S^t$$. We
evaluate them with the loss function:

$$
f_S^t = \sum_{j=1}^J \sum_p W(p) \cdot \lVert S_j^t(p) - S_j^\ast(p) \rVert_2^2
$$

Then the overall loss function for the entire model is:

$$
f = \sum_{t=1}^{T_P} f_L^t + \sum_{t=T_P+1}^{T_P+T_C} f_S^t
$$

This is the technique of intermediate supervision at each stage so we can
replenish the vanishing gradient.

At the time of using the output of the model, we evaluate the location of two
candidate positions of a limb, $$d_1$$ and $$d_2$$, by a *line integral* along
the line segment between the two points, namely, the confidence of $$d_1$$ to
$$d_2$$ is a limb $$c$$ is defined to be

$$
E = \int_0^1 L_c(p(u)) \cdot \frac{d_2 - d_1}{\lVert d_2 - d_1\rVert_2} du
$$

The vector dot product in the integral may produce negative values. The
position $$p(u)$$ is the affine:

$$
p(u) = (1-u) d_1 + u d_2
$$

Note that this line integral is agnostic to the length $$d_2-d_1$$ since
$$L_c(p(u))$$ and the fraction are both unit vectors. Hence $$E$$ should be in
range $$[-1,1]$$. But negative candidates are not considered.

In practice, this line integral can be computed as by summing over sample
points of uniformly spaced $$u$$.

## Identify multiple persons (Sec. 3.5)

Partitioning keypoints is a NP-hard problem. An edge should be connecting two
distinct-kind keypoints but the keypoints from an image should be $$K$$-partite
for $$K$$ persons.

The network produces a set of points $$d_j^m$$ for candidate $$m$$ of body part
$$j$$. We know $$j=1,\dots,J$$ as the number of body parts we are interested.
But each body part $$j$$ has candidates $$m=1,\dots,N_j$$. The edge between
$$d_{j_1}^m$$ and $$d_{j_2}^n$$ has weight defined by the line integral $$E$$
as defined above. An indicator variable $$z_{j_1,j_2}^{mn}\in\{0,1\}$$ is to
tell whether $$d_{j_1}^m$$ and $$d_{j_2}^n$$ are connected (i.e., belong to the
same person). This can be set up as an optimization problem for limb type $$c$$:

$$
\begin{aligned}
\max && E_c &= \max_Z \sum_{m\in D_{j_1}} \sum_{n\in D_{j_2}} E_{mn} z_{j_1j_2}^{mn} & \\
\text{subject to}
&& \sum_{n\in D_{j_2}} z_{j_1j_2}^{mn} &\le 1 & \forall m\in D_{j_1} \\
&& \sum_{m\in D_{j_1}} z_{j_1j_2}^{mn} &\le 1 & \forall n\in D_{j_2}
\end{aligned}
$$

where $$Z$$ should contain only the appropriate keypoint types $$j_1$$ and $$j_2$$
for lime type $$c$$. Two relaxations added: (1) instead of a complete graph
between any keypoints of the same person, we work on a spanning tree. Hence we
reduced the number of limbs $$C$$. (2) Decomposed the matching problem into
bipartite matching subproblems of adjacent tree nodes. Since the PAFs is
modelling the adjacent tree nodes. With these relaxations, the objective is

$$
\max_Z E = \sum_{c=1}^C \max_Z E_c
$$

i.e., to find the edges with maximum total line integral weights.

Further improvement: Consider redundant PAF connections such as ears to
shoulders. This addition makes the overall graph not a spanning tree but can
improve the accuracy in crowded images. This would need a slight change in the
algorithm: All pairwise possible connections are sorted by the PAF score and
added in the descending order. Any limb to add that may create a graph with
multiple keypoints with the same type (i.e., joining two persons) will have
this limb ignored as it is contradicting with another limb with higher
confidence.

These operations are not part of the neural network, but as post-processing of
the neural network output.

## Links

OpenPose: The model is in Caffe
- https://towardsdatascience.com/openpose-research-paper-summary-realtime-multi-person-2d-pose-estimation-3563a4d7e66
- https://github.com/CMU-Perceptual-Computing-Lab/openpose
- https://github.com/CMU-Perceptual-Computing-Lab/openpose_train
- https://github.com/CMU-Perceptual-Computing-Lab/openpose_caffe_train
- https://cmu-perceptual-computing-lab.github.io/foot_keypoint_dataset/

Alternatives:
- https://github.com/tensorflow/tfjs-models/tree/master/pose-detection

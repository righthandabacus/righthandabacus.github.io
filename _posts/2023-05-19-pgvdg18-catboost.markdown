---
layout: post
title: "Prokhorenkova et al (2018) CatBoost: Unbiased boosting with categorical features"
date: 2023-05-19
category: paper
bibtex:
  type: inproceedings
  title: "CatBoost: Unbiased boosting with categorical features"
  author: "Liudmila Prokhorenkova and Gleb Gusev and Aleksandr Vorobev and Anna Veronika Dorogush and Andrey Gulin"
  booktitle: "Proc. NeurIPS"
  year: 2018
  arXiv: 1706.09516
---

CatBoost is a library for random forest. This paper describes the key feature behind it.

## Sec 2

Symbols and key concepts are provided.

Dataset $D=\{(x_i, y_i)\}_{i=1,\dots,n}$ with $x_i = (x^i_1,\dots,x^i_m)$ a
vector of $m$ features and $y_i\in\mathbb{R}$ a target value. Random forest is
a model $F:\mathbb{R}^m\mapsto\mathbb{R}$ to minimize expected loss $\mathbb{E}L(y, F(x))$

Gradient boosting: Builds iteratively a sequence of approximations $F^t$ in greedy fashion
- additive: $F^t = F^{t-1} + \alpha h^t$ with step size $\alpha$
- $h^t:\mathbb{R}^m\mapsto\mathbb{R}$ is the *base predictor* chosen from a family of functions $H$, such that

  $$h^t = \arg\min_{h\in H}\mathbb{E}L(y,F^{t-1}(x)+h(x))$$

- optimization using Newton's method, in particular, least-square approximation

$$
\begin{aligned}
h^t &= \arg\min_{h\in H}\mathbb{E}(-g^t(x,y)-h(x))^2
\text{with}\quad
   g^t(x,y) &= \left.\frac{\partial L(y,s)}{\partial s}\right\vert_{s=F^{t-1}(x)}
\end{aligned}
$$


## Sec 3

The problem of the gradient boosting tree is when we encountered categorical features.

Handling categorical features in a boosted tree is usually to use one-hot
encoding. But if the feature has high cardinality, e.g., user ID, this would
lead to an infeasibly large number of new features.

Other method: Group categories by the target statistics (TS)
- TS = expected value of the target within that group of categories
- LightGBM approach: categorical features become gradient statistics at each step of gradient boosting
	- providing information for the tree
	- high computation cost to calculate statistics for each categorical value at each step
	- high memory cost to store the categories of each node for each split
- using TS as a new numerical feature is cost-efficient with minimal information loss

How to substitute category $x^i_k$ of sample $k$ with a numeric feature $\hat{x}^i_k$ of TS?
- make $\hat{x}^i_k = \mathbb{E}[y\mid x^i = x^i_k]$, i.e., the expectation of target of the same category over the entire population
- Greedy approach to estimate $\mathbb{E}[y\mid x^i = x^i_k]$ on low-frequency categories:

   $$\hat{x}^i_k = \frac{\sum_{j=1}^n \mathbb{1}[x^i_j=x^i_k]y_j + ap}{\sum_{j=1}^n \mathbb{1}[x^i_j=x^i_k] + a}$$

  - $p$ the prior estimate, usually the average target value of the dataset
  - $a>0$ the smoothing parameter
  - subject to target leaking, i.e., there is a conditional shift from the desired expectation to the estimated expectation
    $$\mathbb{E}[\hat{x}^i\mid y=v] \ne \mathbb{E}[\hat{x}^i_k\mid y_k = v]$$

Improvement to mitigate conditional shift: Compute target statistics from a special subset

$$\hat{x}^i_k = \frac{\sum_{j\in J} \mathbb{1}[x^i_j=x^i_k]y_j + ap}{\sum_{j\in J} \mathbb{1}[x^i_j=x^i_k] + a}$$

- holdout TS: Partition the training dataset into two parts, use one part to calculate the TS and another for actual training. But this made the size of the training set smaller
- Leave-one-out TS: Use $D_k = D\setminus x_k$ for TS
- Ordered TS (CatBoost):
	- inspired by online learning algorithms (getting samples sequentially in time)
	- in offline setting, random permute the samples $\sigma(j)$ as artificial "time"
	- to compute TS at $x_k$, consider the subset $D_k = \{x_j: \sigma(j)<\sigma(k)\}$ of samples seen before in the permuted sequence
	- CatBoost use different permutation in different gradient boosting steps (i.e., order boosting), to reduce the variance

## Sec 4

$h^t$ is usually approximated from the dataset,
$$h^t = \arg\min_{h\in H}\frac{1}{n}\sum_{k=1}^n(-g^t(x_k,y_k)-h(x_k))^2$$
- base predictor $h^t$ is biased since the distribution of $g^t(x_k,y_k)\mid x_k\ne$ that of $g^t(x,y)\mid x$
- causing target leakage: gradients used at each step are estimated using the target values of the same data points the current model $F^{t-1}$ was built on

Prediction shift: Theorem 1 in the paper said, unbiased estimate can be
achieved if independent datasets are used at each gradient step. Otherwise
there is a bias of $-\frac{1}{n=1}c_2(x^2-\frac12)$ for $n$ the dataset size

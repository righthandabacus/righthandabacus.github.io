---
layout: post
title: "Multivariate Gaussian Distribution"
date: 2012-05-09 19:40:47 -0400
tags: math
---

Consider the i.i.d. Gaussian variables $$z_i \sim N(\mu,\sigma)$$, where $$\mu=0$$
and $$\sigma=1$$. The random variables from a linear combination of $$z_i$$, e.g.

$$
\begin{aligned}
x_1 &= a_{11} z_1 + a_{12} z_2 + \cdots + a_{1n} z_n  \\
x_2 &= a_{21} z_1 + a_{22} z_2 + \cdots + a_{2n} z_n
\end{aligned}
$$

are correlated. Their covariance is given by

$$
\begin{aligned}
cov(x_1, x_2) &= E[(x_1 - \bar{x}_1)(x_2 - \bar{x}_2)] \\
&= E[(a_{11} z_1 + a_{12} z_2 + \cdots + a_{1n} z_n)(a_{21} z_1 + a_{22} z_2 + \cdots + a_{2n} z_n)] \\
&= a_{11}a_{21} + a_{12}a_{22} + \cdots + a_{1n}a_{2n}
\end{aligned}
$$

as the covariance between two independent standard normal variables is zero.

Consider a $$k$$-vector $$\vec{z}$$ of independent standard normal variables $$z_i$$,
and define the $$n$$-vector of random variables $$\vec{x}$$ as the linear
combination of the $$k$$ variables in $$\vec{z}$$ and a $$n$$-vector of constants
$$\vec{\mu}$$:

$$
\vec{x} = \mathbf{A}\vec{z} + \vec{\mu},
$$

then the covariance matrix of $$\vec{x}$$ is $$\mathbf{\Sigma} = \mathbf{AA}^T$$ and
the mean is $$\vec{\mu}$$.

The p.d.f. of a univariate Gaussian variable is

$$
f(x) = \frac{1}{\sqrt{2\pi\sigma^2}} \exp(-\frac{(x-\mu)^2}{2\sigma^2})
$$

while the p.d.f. of a multivariate Gaussian variable ($$n$$-vector) is

$$
f(\vec{x}) = \frac{1}{\sqrt{(2\pi)^n|\mathbf{\Sigma}|}} \exp(-\frac{1}{2}(\vec{x}-\vec{\mu})^T\mathbf{\Sigma}^{-1}(\vec{x}-\vec{\mu}))
$$

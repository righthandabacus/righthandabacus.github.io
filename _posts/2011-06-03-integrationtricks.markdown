---
layout: post
title: "Integration tricks"
date: 2011-06-03 23:56:25 -0400
tags: math
---
[Source](https://1over137.wordpress.com/2011/06/04/integration-by-integration-under-the-integral-sign/)

## Integration by differentiation:

Compute $\int_0^\infty xe^{-\lambda x} dx$ without integration by parts:

Consider the integral: $\int_0^\infty e^{-\lambda x} dx = \frac{1}{\lambda}$. Differentiating both side w.r.t. $\lambda$ gives:

$\frac{\partial}{\partial\lambda}\int_0^\infty e^{-\lambda x}dx = -\int_0^\infty xe^{-\lambda x}dx = -\frac{1}{\lambda^2}$

Therefore, $\int_0^\infty xe^{-\lambda x}dx = \frac{1}{\lambda^2}$.

## Integration by integration:

Compute $\int_0^1 \frac{x^b-x^a}{\ln x}dx$:

Consider $\int_0^1 x^\alpha dx = \frac{1}{\alpha + 1}$ for $\alpha > -1$. Integrate both side w.r.t. $\alpha$ from $a$ to $b$ gives:

$$
\begin{aligned}
\int_a^b \int_0^1 x^\alpha dx d\alpha &= \int_a^b \frac{1}{\alpha+1} d\alpha \\
\int_0^1 \left.\frac{e^{\alpha\ln x}}{\ln x} \right|_a^b dx &= \ln\left|\frac{b+1}{a+1}\right| \\
\int_0^1 \frac{x^b - x^a}{\ln x} dx &= \ln\left|\frac{b+1}{a+1}\right|
\end{aligned}
$$

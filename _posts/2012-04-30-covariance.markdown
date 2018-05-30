---
layout: post
title: "Covariance & Correlation"
date: 2012-04-30 15:44:12 -0400
tags: math
---

Consider random variables $$X$$ and $$Y$$, to measure how much they change together, we use covariance:

Firstly, we center them at their mean, $$X'=X-\bar{X}$$, $$Y'=Y-\bar{Y}$$, and find
the expectation of the product. The product is positive if both of them move
beyond (both $$X',Y'$$ positive) or below (both $$X',Y'$$ negative) the mean. But
negative if they are at different side about the mean value. Thus the measure,
named covariance, defined as

$$
cov(X,Y)=E[(X-\bar{X})(Y-\bar{Y})].
$$

For random vectors $$X$$ and $$Y$$, the covariance is a matrix whose $$ij$$-th element
is the correlation of the $$i$$-th element in $$X$$ and $$j$$-th element in $$Y$$, i.e.
$$Cov(X_i,Y_j)$$. Precisely,

$$
\begin{aligned}
Cov(X,Y) &= E[(X-\bar{X})(Y-\bar{Y})^T]  \\
&= E[XY^T] - \bar{X}\bar{Y}^T.
\end{aligned}
$$

Covariance has the property that, $$Cov(X,X)=Var(X)$$.

The magnitude of covariance does not carry much interpretation. Thus we usually
normalize the covariance into $$[-1,1]$$ and produce the Pearson's coefficient:

$$
r_{XY} = \frac{Cov(X,Y)}{\sigma_X \sigma_Y} = \frac{\sum_{i=1}^n(X_i - \bar{X})(Y_i-\bar{Y}}{\sqrt{\sum_{i=1}^n(X_i-\bar{X})^2\sum_{i=1}^n(Y_i-\bar{Y})^2}}.
$$

The Pearson's coefficient measures the strength of linear dependence between
random variables $$X$$ and $$Y$$.

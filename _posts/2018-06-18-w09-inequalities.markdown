---
layout: post
title: Woeginger (2009) When Cauchy and Hölder Met Minkowski
date: Mon, 18 Jun 2018 16:20:49 -0400
tags: math
category: paper
bibtex:
  type: article
  title: "When Cauchy and Holder Met Minkowski: A Tour through Well-Known Inequalities"
  author: "Gerhard J. Woeginger"
  journal: "Mathematics Magazine"
  volume: 83
  number: 3
  pages: "202--207"
  year: 2009
---

A short and interesting paper try to derive a series of inequalities from a
basic theorem (the master theorem as dubbed in the paper):

A strictly concave function $$g:\mathbb{R}_+\to\mathbb{R}$$ and another function
$$f:\mathbb{R}_+^2\to\mathbb{R}$$ defined as $$f(x,y)=y\cdot g(x/y)$$. For all
positive real numbers $$x_1,\cdots,x_n$$ and $$y_1,\cdots,y_n$$ satisfy the
inequality

$$\sum_{i=1}^n f(x_i,y_i) \le f(\sum_{i=1}^n x_i, \sum_{i=1}^n y_i)$$

with equality holds iff the two sequences are proportional, i.e., $$x_i/y_i=t$$
for some constant $$t$$. If the function $$g()$$ is concave but not strictly,
the condition for equality relaxed. This theorem can be generalized into higher
dimension for $$f(x_1,x_2,\cdots,x_d) = x_d g(x_1/x_d,x_2/x_d,\cdots,x_{d-1}/x_d)$$.

Concave function is satisfies the following inequalities on affine combination:

$$\lambda g(x) + (1-\lambda)g(y) \le g(\lambda x+(1-\lambda)y)$$

where $$\lambda\in[0,1]$$. In higher dimension, the function arguments $$x,y$$
can be vectors in $$\mathbb{R}_+^d$$. A twice-differentiable concave function
has its second derivative nonpositive (or in high dimension, the Hessian matrix
is negative semidefinite). Convex functions reverse the inequality sign above.

This paper concerns about concave function but it also mentioned two results of
convex functions:

#### AM > GM

$$ \frac{1}{n}\sum_{i=1}^n a_i \ge \left(\prod_{i=1}^n a_i\right)^{1/n} $$

#### Jensen's inequality

$$ \frac{1}{n}\sum_{i=1}^n f(x_i) \ge f\left(\frac{1}{n}\sum_{i=1}^n x_i\right)^{1/n} $$

for convex function $$f:\mathbb{R}\to\mathbb{R}$$

Below is how the different inequalities can be derived from the master theorem:

#### Cauchy-Schwarz inequality

$$ \sum_{i=1}^n a_i b_i \le \sqrt{\sum_{i=1}^n a_i^2} \sqrt{\sum_{i=1}^n b_i^2} $$

Normally this is proved by expansion of $$\sum(a_ib_j - a_jb_i)^2 \ge 0$$ but we
can deduce this immediately from setting $$g(x)=\sqrt{x}$$ in the theorem, with
$$f(x,y)=\sqrt{x}\sqrt{y}$$.

#### Hölder's inequality

$$ \sum_{i=1}^n a_ib_i \le \left(\sum_{i=1}^n a_i^p\right)^{1/p} \left(\sum_{i=1}^n b_i^q\right)^{1/q} $$

on the condition that $$p,q>1$$ and $$1/p+1/q=1$$. Which $$p=q=2$$ reduces
Hölder's inequality into Cauchy-Schwarz inequality.

Paper mentioned that the Hölder's inequality can be proven by using Young's
inequality, namely, $$xy \le x^p/p + y^q/q$$ but we can also deduce this
immediately by using $$g(x)=x^{1/p}$$ with $$f(x,y)=x^{1/p}y^{1/q}$$.

#### Minkowski inequality

$$ \left(\sum_{i=1}^n (a_i+b_i)^p\right)^{1/p} \le \left(\sum_{i=1}^n a_i^p\right)^{1/p} + \left(\sum_{i=1}^n b_i^p\right)^{1/p} $$

This is the generalized triangle inequality $$\|a+b\|_2 \le \|a\|_2 + \|b\|_2$$.
It can be deduced immediately using $$g(x)=(x^{1/p}+1)^p$$ with
$$f(x,y)=(x^{1/p}+y^{1/p})^p$$, and then substitute $$x_i=a_i^p, y_i=b_i^p$$.

#### Milne inequality

$$ \left(\sum_{i=1}^n (a_i+b_i)\right)\left(\sum_{i=1}^n \frac{a_ib_i}{a_i+b_i}\right) \le \left(\sum_{i=1}^n a_i\right) \left(\sum_{i=1}^n b_i\right) $$

The inequality studied by E. A. Milne and arose from astronomy. This can be
obtained by using $$g(x)=x/(1+x)$$ with $$f(x,y)=xy/(x+y)$$.

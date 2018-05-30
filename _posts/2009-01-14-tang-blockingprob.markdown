---
layout: post
title: "Tang, Huang, Chen (2000) Internet Flow Blocking Probability Calculation (Canadian Conf on ECE)"
date: 2009-01-14 02:49:49 -0500
category: paper
bibtex:
  type: inproceedings
  title: Internet Flow Blocking Probability Calculation
  author: "Helen Y Tang and Steed J Huang and H.-M. Fred Chen"
  booktitle: Proc. Canadian Conference on Electrical and Computer Engineering
  volume: 2
  pages: "659--663"
  month: "Mar 7-10"
  year: 2000
  address: "Halifax, NS, Canada"
---

This paper derives equations. Assume the Internet operates as
$$M$$/Pareto/$$1$$/$$K$$ queues, the flow blocking probability is $$P_B =
1-\dfrac{\Gamma(K)}{m+\lambda\Gamma(K)}$$, where $$\lambda$$ is the arrival rate,
$$m$$ is the first busy period (i.e. mean of Pareto),
$$\Gamma(K)=\dfrac{m}{1-\rho}(P_0+P_1+...+P_{K-1})$$ is the $$k$$-th busy period,
and $$P_j$$ denotes the probability of having $$j$$ customers in the
$$M$$/Pareto/$$1$$/$$K$$ queue.

The generating function for $$P_j$$ is obtained from the M/G/1 analysis:  

$$ P(z)=\sum_{j=0}^\infty P_j z^j = (1-\rho)(1-z)\dfrac{B(\lambda(1-z))}{B(\lambda(1-z))-z} $$

with $$B(s)$$ is the Laplace transform of the Pareto pdf $$b(t)=\alpha
t^{-(\alpha+1)}$$, where $$t>1$$, $$1<\alpha<2$$, mean equals to
$$m=\alpha/(\alpha-1)$$. However, using this method is computationally intensive
and the paper proposed the following approximation:

$$
\begin{align*}
p &= \left.\dfrac{dP(z)}{dz}\right|_{z=1}  \\
L &= p-\rho  \\
x &= (L-p)/L  \\
g &= \dfrac{\rho(1-x)}{x(1-\rho)}  \\
y &= 1+\dfrac{gx(1-x|{K-1})}{1-x+gx|K}  \\
P_B &= \dfrac{gx|K}{y}
\end{align*}
$$

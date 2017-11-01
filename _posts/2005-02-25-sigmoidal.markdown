---
layout: post
title: "What is Sigmoidal function?"
date: 2005-02-25 07:16:06 +0800
tags: math
---

Sigmoidal function is any function with the following properties:
  * Monotonically increasing
  * Differentiable
  * Bounded

Sigmoidal function is useful in fuzzy logic, where it gives a value between 0 and 1 to tell how is the membership of a member to a set, with 1 means the full membership and 0 means not a member.

Usually $x$ is a measure (e.g. length) between two quantity, $\ell_1$ and $\ell_2$, where they are the hard boundaries. In other words, $x<\ell_1$ implies membership quality $\mu$=0 and $x\ge\ell_2$ implies $\mu$=1. Between the boundaries, membership quality of measure $x$ is determined by a sigmoidal function.

Examples of sigmoidal functions are:
  * $\sin^2\alpha$ where $\alpha=\frac{\pi}{2}\frac{x-\ell_1}{\ell_2-\ell_1}$
  * $1/(1+e^{-\lambda x})$ with $\lambda>0$, here $\ell_1=-\infty$ and $\ell_2=\infty$
    * this one is called the "unipolar sigmoidal function"
  * $\dfrac{1-e^{-\lambda x}}{1+e^{-\lambda x}}$
    * this one is called the "bipolar sigmoidal function"
    * with $\lambda>0$ and $\ell_1=0, \ell_2=\infty$
  * signum function: $(x)=1$ if $x\ge 0$ and $f(x)=-1$ if $x<0$
  * saturation function: $f(x)=2x$ when $-0.5<x<0.5$, $f(x)=-1$ when $x<-0.5$ and $f(x)=1$ when $x>0.5$
    * the op-amp

## Reference
MathWorks' Documentation on Fuzzy logic toolbox: Membership functions, <http://www.mathworks.com/access/helpdesk/help/toolbox/fuzzy/fp608.html>

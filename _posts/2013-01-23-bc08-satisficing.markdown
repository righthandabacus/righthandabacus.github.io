---
layout: post
title: "Bearden & Connolly (2008) On Optimal Satisficing"
date: 2013-01-23 14:09:47 -0500
category: paper
bibtex:
  type: incollection
  title: "On Optimal Satisficing: How Simple Policies can Achieve Excellent Results"
  author: "J. N. Bearden and T. Connolly"
  booktitle: "Decision Modelling in Uncertain and Complex Environments"
  editor: "T. Kugler and J. C. Smith and T. Connolly and Y. J. Son"
  address: "New York"
  publisher: "Springer"
  year: "2008"
---
Satisficing is to accept the first-seen option that met certain thresholds, known as *aspirations*. Assume options are presented as vectors $\vec{x}=(x^1,x^2,\cdots,x^k)$, and the aspiration level as $\vec{\theta}=(\theta^1,\theta^2,\cdots,\theta^k)$. There is an indicator function for accept,

$$ \sigma=\mathbb{1}\{x^i \ge \theta^1 \ \forall i\}, $$

and a payoff function of the option, $\phi(\vec{x})\in\mathbb{R}$. The option is assumed to be stochastic with density function $f(\vec{x})$.

The probability of an option is acceptable is

$$ \Pr[\sigma=1 \mid \theta] = \int_\theta^\infty f(\vec{x})d\vec{x} $$

the expected payoff of *any* option is

$$ E[\phi(\vec{x})] = \int_\Omega f(\vec{x})\phi(\vec{x})d\vec{x} $$

and the expected payoff of an *acceptable* option is

$$ E[\phi(\vec{x}) \mid \sigma=1] = \frac{\int_\theta^\infty f(\vec{x})\phi(\vec{x})d\vec{x}}{\int_\theta^\infty f(\vec{x})d\vec{x}}.  $$

Consider $N$ options presented in tandem, the decision maker must take one of them. Thus the last one must be accepted if presented. The policy would be on setting the aspiration $\vec{\theta}$ to maximize the expected payoff. Let the value of a policy at state $n$ to be $V_n(\vec{\theta})$. Then we have

$$ V_N(\vec{\theta}) = E[\phi(\vec{x})] $$

and

$$
  V_n(\vec{\theta}) = E[\phi(\vec{x}) \mid \sigma=1]\Pr[\sigma=1 \mid \vec{\theta}]
    + V_{n+1}(\vec{\theta})\Pr[\sigma=0 \mid \vec{\theta}]
$$

for $n=1,\cdots,N-1$.

The goal of optimal satisficing is to find $\arg\max_{\theta\in\Omega} V_n(\theta)$. As $E[\phi(\vec{x})] < E[\phi(\vec{x}) \mid \sigma=1]$ for positive function $\phi(\vec{x})$, the optimal $\theta$ usually decreases as $n$ increases.

Heuristic satisficing is to use a fixed $\theta$ for all $n$. That is, with

$$
  V_H(\theta) = \sum_{n=1}^{N-1}\Big((1-P_\sigma)^{n-1}P_\sigma E[\phi(\vec{x}) \mid \sigma=1]\Big)
      + (1-P_\sigma)^{N-1}E[\phi(\vec{x})]
$$

to find $\arg\max_{\theta\in\Omega} V_H(\theta)$. It is found that, the value of heuristic satisficing $V_H$ has only a slight decrease from the optimal satisficing $V_1$.

Satisficing can be converted into infinite horizon: Introduce a time cost of $c$ per unit time in heuristic satisficing. The value is then

$$ V_\infty = \int_0^\infty (1-P_\sigma)^t \Big(P_\sigma E[\phi(\vec{x}) \mid \sigma=1]-c\Big) dt $$

It is found that, as $c$ increases, $\theta$ decreases. This reflects that the higher the time cost, the more relaxed the aspiration have to be.

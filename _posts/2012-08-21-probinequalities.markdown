---
layout: post
title: "Inequalities of Probability"
date: 2012-08-21 13:01:25 -0400
tags: math
---

There are several famous inequalities in the theory of probability. The simplest one is the Markov inequality:

$$
\Pr(|X|\ge a) \le \frac{E[|X|]}{a},
$$

for any random variable $X$ and real number $a>0$.

If we consider the random variable $(X-E[X])^2$, substitute into the Markov inequality, we have

$$
\begin{align}
\Pr((X-E[X])^2\ge a^2) &\le \frac{E[(X-E[X])^2]}{a^2} = \frac{Var[X]}{a^2} \\
\Pr(|X-E[X]|\ge a) &\le \frac{Var[X]}{a^2}.
\end{align}
$$

By substituting $a=k\sigma$ where $Var[X]=\sigma^2$, we have the Chebyshev inequality

$$
\Pr(|X-E[X]| \ge k\sigma) \le \frac{1}{k^2}.
$$

which it is required that $k>0$ and $\sigma$ is non-zero and finite.

Two other useful but more complicated probability inequalities are about
deviation from mean. The Chernoff bound says that, for $n$ i.i.d. Bernoulli
random variables $X_1,X_2,\cdots,X_n$, each having the probability
$p>\frac{1}{2}$, the probability of having more than $n/2$ occurrences among the
$n$ of them is

$$
\sum_{j=\lfloor n/2\rfloor+1}^{n} \binom{n}{j} p^j (1-p)^{n-j}
\ge
1-\exp\big(-2n(p-\frac{1}{2})^2\big).
$$

While these $n$ Bernoulli random variable shall produce the expectation of $np$
occurrences, the probability of deviation from $np$ is bounded by the Hoefding's
inequality, saying that for $\epsilon > 0$, the probability of no less than
$n(p+\epsilon)$ occurrences is

$$
P_{\ge n(p+\epsilon)} \le \exp(-2\epsilon^2 n),
$$

and the probability of no more than $n(p-\epsilon)$ occurrences is

$$
P_{\le n(p-\epsilon)} \le \exp(-2\epsilon^2 n),
$$

so the probability of having $k$ occurrences, which $k\in[n(p-\epsilon),n(p+\epsilon)]$, is

$$
P_{\in[n(p-\epsilon),n(p+\epsilon)]} > 1-2\exp(-2\epsilon^2 n).
$$

Hoefding's inequality can be generalized so that, for $X_i\in[a_i,b_i]$ a.s. and
$X_1,X_2,\cdots,X_n$ are independent, we have the empirical mean and its
expectation

$$
\begin{align}
\bar{X} &= \frac{1}{n}(X_1+X_2+\cdots+X_n) \\
E[\bar{X}] &= \frac{1}{n}(E[X_1]+E[X_2]+\cdots+E[X_n]),
\end{align}$$

then for $t>0$ we have

$$
\begin{align}
\Pr[\bar{X}-E[\bar{X}]\ge t] &\le \exp(-\frac{2t^2n^2}{\sum_{i=1}^n (b_i-a_i)^2}) \\
\Pr[E[\bar{X}]-\bar{X}\ge t] &\le \exp(-\frac{2t^2n^2}{\sum_{i=1}^n (b_i-a_i)^2}).
\end{align}$$

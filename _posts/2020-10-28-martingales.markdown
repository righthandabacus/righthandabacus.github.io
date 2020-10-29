---
layout: post
title: Martingale and local martingale
date: Wed, 28 Oct 2020 20:51:16 -0400
tags: math
---

Martingale is a stochastic process with the martingale property. If we have
$$X_t$$ as the stochastic process, the martingale property says that
$$\mathbf{E}[X_t\mid\mathcal{F}_s] = X_s$$, for $$s<t$$. Closely
related to this is the local martingale. However, the [Wikipedia
page](https://en.wikipedia.org/wiki/Local_martingale) does not have it clearly
explained. Here is my narrative.

A martingale is a stochastic process that (1) adapted to the filtration
$$\mathbb{F}$$, (2) uniformly integrable, and (3) exhibits the martingale
property. The filtration part is easy to understand, simply the information up
to time $$t$$ is enough to tell about the stochastic process $$X_t$$. The
uniformly integrable property simply means [there is only a finite amount of
mass to go around](https://www.quora.com/What-is-a-local-martingale).
Technically, it means
$$\forall \epsilon>0,\ \exists K: \mathbf{E}[\lvert X_t\rvert \cdot \mathbf{1}\{\lvert X_t\rvert \ge K \}] < \epsilon$$.
Note that since $$X_t$$ is a stochastic process, the $$\epsilon$$ and $$K$$
above applies to all time index $$t$$. This may not hold for a heavy-tail
distribution. If the martingale $$X=\{X_t: 0\le t<\infty\}$$ is uniformly
integrable, we can have the *closure property* such that $$X_\infty$$ can be
defined and append to $$X$$ and we can reverse to compute
$$X_t=\mathbf{E}[X_\infty\mid\mathcal{F}_t]$$.

A localized stochastic process is defined as follows. If the set $$C$$ is a
family of stochastic processes of certain properties, the localized set $$C'$$
is a superset of $$C$$ such that, if $$X_t\in C'$$ then we can find an
increasing sequence of stopping time $$\tau_n$$ such that
$$\lim_{n\to\infty}\tau_n=\infty$$ and the stopped processes
$$X_{\min(t,\tau_n)}\in C$$. This is an example of a localized stochastic
process: If $$C$$ is the set of all bounded stochastic processes, then the
Brownian motion $$W_t\notin C$$ as it is not bounded. However, we can define
the stopping time $$\tau_n=\inf\{t: \lvert W_t\rvert = n\}$$ such that the stopped
process $$W_{\min(t,\tau_n)}\in C$$. Therefore $$W_t\in C'$$ is a locally
bounded process.

A local martingale is simply a stochastic process which when localized is a
martingale. The [Wikipedia page](https://en.wikipedia.org/wiki/Local_martingale)
use the following as an example:

$$
X_t = \begin{cases}
  W^T_{t/(1-t)} & 0\le t < 1, \\
  -1            & 1\le t < \infty.
\end{cases}
$$

The stopping time $$T=\inf\{t: W_t=-1\}$$ is the first hitting time of
$$W_t=-1$$. The stopped process $$W^T_{t/(1-t)}$$ is a "compressed" Brownian
motion started at $$W_0=0$$ until it hits $$-1$$ the first time than holds this
value constant. Since a Brownian motion $$W_t$$ will hit $$-1$$ at some time
$$t$$ almost surely, we can have $$W^T_\infty=-1$$ a.s., and hence the
"compressed" version $$W^T_{t/(1-t)}=-1$$ before $$t=1$$. Thus the limit agrees
on both side:

$$
\lim_{t\to 1} W^T_{t/(1-t)} = -1.
$$

Brownian motion $$W_t$$ exhibits martingale property, i.e.
$$\mathbf{E}[W_t\mid\mathcal{F}_s]=W_s$$, so as the stopped process $$W^T_t$$.
But this does not apply to the example process $$X_t$$ because at least this
expectation does not hold at $$t=1$$ and $$s=0$$, which $$X_0=W^T_0=0$$ and
$$X_1=-1$$ so $$\mathbf{E}[X_1\mid\mathcal{F}_0]\ne X_0$$. However, it is a
local martingale when we consider
[the sequence of stopping times](https://math.stackexchange.com/questions/2077827)

$$
T_n = \frac{n}{n+1} \mathbf{1}\{T \ge n\} + (\frac{T}{T+1} + n)\mathbf{1}\{T<n\}.
$$

In summary, a local martingale is a relaxed version of martingale which
exhibits the martingale property often except in some corner cases.

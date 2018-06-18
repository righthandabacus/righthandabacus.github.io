---
layout: post
title: Thorp (1969) Optimal gambling systems for favorable games
date: Fri, 15 Jun 2018 20:17:37 -0400
tags: math
category: paper
---

There are two papers of the same title:

```bibtex
@inproceedings{
    title = "Optimal gambling systems for favorable games",
    author = "L. Breiman",
    year = 1961,
    booktitle = "Fourth Berkeley Symposium on Probability and Statistics",
    volume = 1,
    pages = "65--78",
}

@article{
    title = "Optimal gambling systems for favorable games",
    author = "E. O. Thorp",
    booktitle = "Review of the International Statistical Institute",
    volume = 37,
    number = 3,
    pages = 273--293,
    year = 1969,
}
```

# Breiman (1961) paper

The 1961 paper is more mathematically vigorous. It discussed the following model
of gambling:

Let outcome $$X$$ be taken on a set $$I=\{1,\cdots,s\}$$ such that
$$\Pr[X=i]=p_i$$. There is a class of subsets (part of σ-algebra) of $$I$$:
$$A_1,\cdots,A_r$$ such that $$\bigcup_i A_i = I$$ and carry the payoffs
$$o_1,\cdots,o_r$$ respectively. We bet $$\beta_1,\cdots,\beta_r$$ for each of
them and in case the outcome is $$i\in I$$, we receive $$\sum_{j: i\in A_j}
\beta_jo_j$$. In order to allow we hold part of our fortune in reserve, we
define $$A_1=I, o_1=1$$. Let $$S_n$$ be the fortune after $$n$$ plays. If there
is a gambling strategy $$\Lambda^{\ast}$$ such that $$S_n\to\infty$$ a.s., we say
the game is favorable.

The paper assume the game is favorable and asked (1) the strategy to minimize
the expected number of trials needed to win or exceed an fixed amount $$x$$; (2)
the size of fortune after some fixed number of plays $$n$$.

It turns out, the two problems solved by the same optimal strategy
$$\Lambda^{\ast}$$, which maximizes $$E[\log S_n]$$. The betting system allows
$$S_n$$ to grow exponentially and the optimal strategy is to maximize the rate
of growth.

# Thorp (1969) paper

This paper provides more examples than the 1961 paper but avoids some detailed
derivations. It gives the following example as "favorable games":

- Blackjack
- Baccarat, if side bets permitted
- Roulette, using "Newtonian method" and if the roulette wheel is tilted for 0.2 degree or more
- Wheel of fortune
- Warrant hedging in stock market (short a call warrant and buy stock)

It mentioned that Blackjack can be modelled as a coin toss (Bernoulli trial)
with probability of success $$p$$ is selected independently on each trial and
announced before each trial. Probability $$p$$ may even be dependent in short
consecutive group. The model is as follows: let $$X_0$$ be the initial capital
and the capital after trial $$i$$ be $$X_i$$. Let the bet on trial $$i$$ be
$$B_i$$ (the sequence $$\{B_i\}$$ is the bettig strategy) and the gain (or loss)
will be $$X_i - X_{i-1}$$. The trial result is modeled as $$T_i \in \{-1,1\}$$,
then

$$\begin{align}
X_j &= X_{j-1} + T_jB_j \\
X_n &= X_0 + \sum_{j=1}^n T_jB_j
\end{align}$$

The betting strategy

$$\begin{align}
B_j &= X_{j-1}           & \textrm{if } p & > 1/2 \\
B_j &= 0                 & \textrm{if } p & < 1/2 \\
B_j &\textrm{ arbitrary} & \textrm{if } p & = 1/2
\end{align}$$

is equivalent to maximizing $$E[X_j]$$ for each $$j$$.

On stock market, the paper model (call) warrants as follows. Price of warrant
$$W$$, of stock $$S$$, exercise price $$E$$. Normally $$S > W\ge S-E$$. The
option pricing is empirically modelled as (note, the paper dated before
Black-Scholes):

$$ W = (S^z + E^z)^{1/z} - E $$

where $$z=1.3+5.3/T$$ with $$T$$ the number of months until expiration. The
paper found that historically warrants are overpriced until close to expiration
(34.5% annualized overpriced).

Warrant hedging means to simultaneously short sell overpriced warrant and long
common stock in a fixed ratio (commonly 1-3 unit of warrants per stock). This
can reduce variance and retain high expectation. Let $$W_0, W_f$$ be warrant
price at of now and at expiration respectively, then

$$\begin{align}
W_f &= 0        & \textrm{if } S_f &\le E \\
W_f &= S_f - E  & \textrm{if } S_f &> E
\end{align}$$

If we make $$S_0, S_f$$ the stock price per share of now and at expiration, the
gain of shorting a warrant is

$$\begin{align}
- (W_f-W_0) &= W_0            & \textrm{if } S_f &\le E \\
- (W_f-W_0) &= W_0 - S_f + E  & \textrm{if } S_f &> E
\end{align}$$

and the gain of buying a share is $$S_f - S_0$$. Therefore the gain of hedging
is

$$\begin{align}
G_f = s(S-S_0) - w(W-W_0) \\
g_f = \frac{G_f}{\alpha s S_0 + \beta w W_0}
\end{align}$$

where $$s,w$$ are the number of shares of common stock to buy and unit of
warrants to short, $$\alpha,\beta$$ are margin used to fund the
stock and warrants, respectively. If we do not borrow, i.e. 100% margin,
$$\alpha=\beta=1$$. We can further use the lognormal distribution to model stock
price:

$$\begin{align}
f(x) &= \frac{1}{x\sigma\sqrt{2\pi}} \exp(-\frac{(\log x-\mu)^2}{2\sigma^2}) \\
E[S_f] &= \exp(\mu + \frac{1}{2}\sigma^2)
\end{align}$$

Proportional betting: $$B_j = fX_{j-1}$$ for a constant $$f$$. If we let number
of success and failure in $$n$$ Bernoulli trials to be $$S_n,F_n$$ respectively,
then

$$ X_n = X_0(1+f)^{S_n}(1-f)^{F_n} $$

Thus we have

$$\begin{align}
\frac{X_n}{X_0} &= (1+f)^{S_n}(1-f)^{F_n} \\
\left(\frac{X_n}{X_0}\right)^{1/n} &= (1+f)^{S_n/n}(1-f)^{F_n/n} \\
\log\left[\left(\frac{X_n}{X_0}\right)^{1/n}\right] &= \frac{S_n}{n}\log(1+f) + \frac{F_n}{n}\log(1-f)
\end{align}$$

which the logarithm in the last equation above models the rate of increase per
trial in average.

J. L. Kelly (1956) proposes to maximize

$$ E\left[\log\left(\frac{X_n}{X_0}\right)^{1/n}\right] = p \log(1+f)+(1-p)\log(1-f) $$

which then defined the growth function:

$$ G(f) = p \log(1+f)+(1-p)\log(1-f) $$

and it is described by the following theorems:

(Theorem) If $$p\in(\frac{1}{2},1)$$,
$$G(f)$$ attains maximum at $$f^{\ast} = p - (1-p)$$ with
$$G(f^{\ast}) = p\log p+(1-p)\log(1-p)+\log 2>0$$, and

- $$G(f)$$ is increasing in $$f\in[0,f^{\ast}]$$, and decreasing in $$f\in[f^{\ast},1]$$
- There is a $$f_c\in(f^{\ast},1)$$ that $$G(f_c)=0$$
- $$G(f)>0$$ for $$f < f_c$$, and $$G(f)<0$$ for $$f > f_c$$
- $$G(f)\to\infty$$ for $$f\to 1$$

(Theorem) $$f^{\ast}$$ is the bet to maximize $$E[\log X_n]$$ because

$$\begin{align}
\textrm{if }G(f) &> 0        & \lim_{n\to\infty} X_n &= \infty \textrm{ a.s.} \\
\textrm{if }G(f) &< 0        & \lim_{n\to\infty} X_n &= 0 \textrm{ a.s.} \\
\textrm{if }G(f_1) &> G(f_2) & \textrm{then } \lim_{n\to\infty} X_n(f_1)/X_n(f_2) &= \infty \textrm{ a.s.}
\end{align}$$

(Theorem) Maximizing $$E[\log X_n]$$ is equivalent to maximizing $$G(f)=E[\log(1+fX)]$$


# Modeling gambler's ruin situation

There is a formula mentioned in the paper and I prove it here.

Define the gambler's ruin to be $$X_j=0$$ after some $$j$$. Let the bet be of
fixed size $$B_j = r$$ for $$0 < X_{j-1} < ar$$, $$a > z > 0$$, and initial
fortune $$X_0=zr$$. The gamble will stop at gambler's ruin or reached a
predefined goal, $$X_j \ge ar$$. Let the Bernoulli probabily be $$p$$. The ruin
probability is

$$R(r) = \frac{\theta^{ar} - \theta^{zr}}{\theta^{ar}-1} \quad \textrm{where }\theta=\frac{1-p}{p}\ne 1$$

Prove: Let $$R_n$$ be the $$R(r)$$ if $$X_0=nr$$. Then there is a probability
$$p$$ to win and it will be equivalent to $$R_{n+1}$$, similarly, with a
probability $$(1-p)$$ it will be equivalent to $$R_{n-1}$$. Then

$$\begin{align}
R_n &= (1-p)R_{n-1} + pR_{n+1} \\
\therefore\ R_{n+1} - R_n &= \frac{1-p}{p}(R_n - R_{n-1})
\end{align}$$

The above is valid for $$1\le n\le a-1$$ and by definition of the model,
$$R_0=1$$ and $$R_a=0$$. Now,

$$\begin{align}
R_{n+1} - R_n &= \frac{1-p}{p}(R_n - R_{n-1}) = \left(\frac{1-p}{p}\right)^n (R_1 - R_0) \\
R_{n+1} - R_1 &= \sum_{k=1}^n (R_{k+1}-R{k}) \\
&= \sum_{k=1}^n \frac{1-p}{p} (R_k - R_{k-1}) = \sum_{k=1}^n \left(\frac{1-p}{p}\right)^k (R_1 - R_0) \\
R_{n+1} - R_0 &= (R_1 - R_0) \sum_{k=0}^n \left(\frac{1-p}{p}\right)^k \\
&= (R_1 - R_0) \frac{\theta^{n+1}-1}{\theta-1}
\end{align}$$

So for $$n=a$$,

$$\begin{align}
R_a - R_0 &= (R_1 - R_0) \frac{\theta^{a}-1}{\theta-1} \\
-1 &= (R_1 - 1) \frac{\theta^{a}-1}{\theta-1} \\
R_1 - 1 &= - \frac{\theta-1}{\theta^{a}-1} \\
\therefore\ 
1- R_{n} &= \frac{\theta-1}{\theta^{a}-1} \frac{\theta^n-1}{\theta-1} = \frac{\theta^{n}-1}{\theta^{a}-1} \\
R_{n} &= 1 - \frac{\theta^{n}-1}{\theta^{a}-1} \\
&= \frac{\theta^{a}-\theta^{n}}{\theta^{a}-1}
\end{align}$$

Theorem: For $$p\in(\frac{1}{2},1)$$, $$R(r)$$ is a strictly decreasing function
of $$r$$. And for $$p\in(0,\frac{1}{2})$$, $$R(r)$$ is a strictly increasing
function of $$r$$.

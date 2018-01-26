---
layout: post
title: How to calculate percentile
date: Fri, 26 Jan 2018 09:23:03 -0500
tags: math
published: true
bibtex:
  type: article
  title: Sample Quantiles in Statistical Packages
  author: Rob J. Hyndman and Yanan Fan
  year: 1996
  journal: The American Statistician
  volume: 50
  number: 4
  month: Nov
  pages: 361-365
  doi: http://dx.doi.org/10.1080/00031305.1996.10473566
---

Think about it for a short while, anyone can see that there is no definitive way
to define what is a percentile. What is surprising is how many different ways we
can define it.

In a 1996 paper, there is a generic model proposed for what a percentile
function can be and nine different variations enumerated.

# Terms and definition

Percentile, or any quantile, is a rank statistic. Assume we have a set of
measurements $X_1,\cdots,X_n$ and the version in ascending order of magnitude
$\mathbf{X}=\{X_{(1)}, X_{(2)}, \cdots, X_{(n)}\}$. Roughly speaking, the
quantile function $Q(p)$, defined over $p\in[0,1]$, gives some value
$X_{(k)}\in\mathbf{X}$ such that the number of measurements is fraction $p$ of
$|\mathbf{X}|$:

$$ \frac{1}{n}|\{X_i: X_i\in\mathbf{X}, X_i\le X_{(k)} \}| \approx p. $$

We call this a percentile function if $p$ is defined in percentage.

There are several characteristics that the quantile function $Q(p)$ may satisfy.
As mentioned in Hyndman & Fan (1996),

1. Surjective from $[0,1]$ to $\mathbf{X}$: $Q(p)$ is continuous on $p$
2. Approximation of distribution function: $pn$ gives the lower bound of the
   number of observations $X_i$ in $\mathbf{X}$ less than $Q(p)$,\\
    $$ |\{X_i: X_i\in\mathbf{X}, X_i\le Q(p)\}| \ge pn $$
3. Quantile are based on samples arranged evenly and symmetrically on $[0,1]$:\\
    $$ \begin{gather}
    |\{X_i: X_i\in\mathbf{X}, X_i\le Q(p)\}| = |\{X_i: X_i\in\mathbf{X}, X_i\ge Q(1-p)\}| \\
    Q^{-1}(X_{(k)}) + Q^{-1}(X_{(n-k+1)}) = 1
    \end{gather} $$
4. Min and max in $\mathbf{X}$ are in $[0,1]$:\\
    $$ Q^{-1}(X_{(1)}) \ge 0, Q^{-1}(X_{(n)}) \le 1 $$
5. $Q(0.5)$ is the median

# Classes of quantile functions

Quantile function can look like a step function, or a piecewise linear
continuous function.

Hyndman & Fan proposed a generic form for quantile function:

$$ Q_i(p) = (1-\gamma)X_{(j)} + \gamma X_{(j+1)} $$

such that

$$\begin{gather}
\frac{j-m}{n} \le p < \frac{j-m+1}{n} \\
\gamma \in [0,1] \\
j = \lfloor pn + m \rfloor \\
g = pn + m - j
\end{gather}$$

## Step functions

[Wikipedia](https://en.wikipedia.org/wiki/Percentile) covers one step function
variant, the *nearest rank method*, which defined $Q(p)=X_{(j)}$ such that
$j=\lceil pn \rceil$. This is similar to the *1st definition* of Hyndman & Fan,
which has the properties that

- step function with jumps on $p=j/n$ for integral $j$.
- caglad: each step is a horizontal line segment with closed end on left and
open end on right, i.e. $Q(j/n) = X_{(j+1)}$ instead of $X_{(j)}$ (Wikipedia
article's definition use $Q(j/n)=X_{(j)}$).

We can have variation on how to handle the end points of steps to this function.
The *2nd definition* of Hyndman & Fan is to use midpoint, i.e., the step
function still have jumps on $p=j/n$ but has the particular cases defined
$Q(j/n)=\frac{1}{2}(X_{(j+1)} + X_{(j)})$.

Formally, the 1st definition has $m=0$ (so jumps on $p=j/n$), $\gamma=1$
if $g>0$ or 0 otherwise. The 2nd definition has $m=0$, $\gamma=\frac{1}{2}$ for $g=0$
and $\gamma=1$ for $g>0$.

The *3rd definition* of the paper gives an alternative way to jump: The step
function has jumps at the midpoint of $p=j/n$ and $p=(j+1)/n$ instead. And each
step is a line segment that is closed on both end if it is a step on $j/n$ for
$j$ an even number (even steps) or the line segment is open on both end
otherwise (odd steps). Formally, $m=-\frac{1}{2}$, $\gamma=0$ if $g=0$ and $j$
is even, or $\gamma=1$ otherwise. This is the implementation in SAS.

## Interpolated functions

This is not a step function but a monotonically increasing continuous function.
Instead of jumps, the function is a connected line segments. Wikipedia gives the
following generic form: $Q(p)=X_{(j)}$ such that

$$ j = f(p) = (n+c_1)p+c_2 = (n+1-2c)p+c $$

where $c\in[0,1]$ and for $j\notin\mathbb{N}$. $X_{(j)}$ is interpolated from
adjacent measurements $X_{(\lfloor j \rfloor)}$ and $X_{(\lceil j \rceil)}$.
Hence this is not a step function.

This is used in Excel. The function `PERCENTILE.INC(array,k)` has $c=1$ and
$p=\frac{j-1}{n-1}$ and `PERCENTILE.EXC(array,k)` has $c=0$ and
$p=\frac{j}{n+1}$. The former, also as the *7th definition* in Hyndman & Fan,
defined the 0th and 100th percentile as the min and max value of $\mathbb{X}$
respectively. The latter, also known as the *6th definition* in Hyndman & Fan,
however, has the min above 0th percentile and the max below 100th percentile
(assuming $\mathbf{X}$ is a sample and the sample min/max is not the population
min/max).

Matlab has $c=\frac{1}{2}$ or $p=\frac{j-1/2}{n}$. Same as
`PERCENTILE.EXC(array,k)`, the min and max are not on 0th and 100th percentile.
This is also the *5th definition* in Hyndman & Fan.

These three variation can be understood from a histogram setting: Assume we have
to layout the $n$ data points as histogram on $[0,1]$:

1. The $c=1$ case: We can split $[0,1]$ into $n-1$ equal segments and put the
   $n$ data points at the $n$ segment boundaries (including 0 and 1)
2. The $c=\frac{1}{2}$ case: We can split $[0,1]$ into $n$ equal segments and
   put the $n$ data points at the middle of each segment
3. The $c=0$ case: We can split $[0,1]$ into $n+1$ equal segments and put the
   $n$ data points at the $n$ segment boundaries (*not* including 0 and 1)

After the data points are placed, $Q(p)$ is the function that connected them
with straight line segments.

Hyndman & Fan has three more variation of the interpolation function:

- *4th definition*: $p=\frac{j}{n}$, max is the 100th percentile. Histogram
  representation is to split $[0,1]$ into $n$ equal segments like the
  $c=\frac{1}{2}$ way above but locate the data point at right end of each
  segment
- *8th definition*: $p=\frac{k-1/3}{n+1/3}$. Histogram representation is to
  split $[0,1]$ into $n+\frac{1}{3}$ equal segments, and from the
  $\frac{2}{3}$-th segment onward up to $(n-\frac{1}{3})$-th segment, there are
  $n-1$ segments apart. We place the $n$ data points evenly in between,
  including the ends on the $\frac{2}{3}$-th and $(n-\frac{1}{3})$-th. This is
  the optimal estimator for median, if $\mathbf{X}$ is a sample.
- *9th definition*: similar to above but defines $p=\frac{k-3/8}{n+1/4}$. This
  is a better estimator for mean for samples drawn from normal distribution.

# Inverse quantile function

In excel, the inverse of percentile function is called percentile rank
(`PERCENTILERANK.INC(array,x)` and `PERCENTILERANK.EXC(array,x)`). The inverse
function is similarly confusing to define and additionally imposed another
problem.

In case of duplicated values in the order statistics $\mathbf{X}$, e.g.,
$X_{(j)}=X_{(j+1)}$, what should be $Q^{-1}(X_{(j)})$?

Given the inverse property, we knows that $p_{\min} \le Q^{-1}(X_{(j)}) \le
p_{\max}$ for $p_{\min} = \min(p: Q(p)=X_{(j)})$ and $p_{\max}$ defined
similarly.

Excel uses $Q^{-1}(X_{(j)}) = p_{\min}$. But it is equally reasonable for 
$Q^{-1}(X_{(j)}) = p_{\max}$ and $Q^{-1}(X_{(j)}) = \frac{1}{2}(p_{\max}+p_{\min})$.

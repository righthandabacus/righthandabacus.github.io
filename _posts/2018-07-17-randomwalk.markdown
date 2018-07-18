---
layout: post
title: "Random Walks in Lattice Grid"
date: Tue, 17 Jul 2018 17:24:20 -0400
tags: math
---

A problem introduced to me by Professor Peter Yum.  Consider random walk on
integers. The simplest one is on

$$\mathbb{Z}=\{\cdots, -2, -1, 0, 1, 2, \cdots\}$$

and we start on 0 at step 0. The random walk proceed y flipping a coin at each
step to decide to go left or right.

We can also define the counterpart on a higher dimensional lattice
$$\mathbb{Z}^d$$, which we decide on each step one of the $$2d$$ possible
direction to go. We always start with the position 0 in such lattice and
concern whether we will *return* to 0 (aka equalization) in some future step.
Formally, the random walk is defined as

$$ S_n = S_0 + \sum_{i=1}^n X_i $$

where $$S_n\in\mathbb{Z}^d$$ is the position of the random walk at step $$n$$
and $$S_0=0$$. $$X_i = \pm e_j$$ is the direction taken at each step $$i$$ where
$$e_j$$ is the $$d$$-dimensional standard basis vector, i.e. only the $$j$$-th
element of the vector is 1 and all other elements are 0.

If at each step, all direction can be taken at equal probability, i.e.

$$ \Pr[X_i = e_j] = \Pr[X_i = -e_j] = \frac{1}{2d}\quad\forall j=1,\cdots,d $$

the random walk is called symmetric.

Polya studied this and some of the following are developed:

We define $$u_n$$ to be the probability that the random walker returned to 0 at
step $$n$$. For any lattice, only $$u_{2n}$$ makes sense as no random walk can
return to 0 in odd number of steps. We define $$u_0 = 1$$ as the random walk
starts at 0.

We further define $$f_n$$ to be the probability that the random walk returns to
0 *for the first time*. And we define $$f_0=0$$ for notational convenience (and
at step 0 is not really a return). With this, we can define the probability that
a random walk will ever return to 0 as

$$ f = \sum_{n=0}^{\infty} f_n = \sum_{n=0}^{\infty} f_{2n} $$

Considering the definition of $$u_n$$ and $$f_n$$, we have the following
relations are established:

$$\begin{align}
  u_1 &= f_0 u_1 + f_1 u_0 \\
  u_2 &= f_0 u_2 + f_1 u_1 + f_2 u_0 \\
  \vdots \\
  u_n &= f_0 u_n + f_1 u_{n-1} + \cdots + f_k u_{n-k} + \cdots + f_n u_0 \\
      &= \sum_{k=0}^n u_k f_{n-k}
\end{align}$$

or equivalently,

$$ u_{2n} = \sum_{k=0}^n u_k f_{n-k} = \sum_{k=0}^n u_{2k} f_{2(n-k)} $$

where $$f_k u_{n-k}$$ represents the probability that the random walker returned to 0 for
the first time after $$k$$ steps and then returned to 0 again in $$n-k$$ more steps.
And if we define the generator functions

$$\begin{align}
S(x) = \sum_{n=0}^{\infty} u_{2n} x^n \\
F(x) = \sum_{n=0}^{\infty} f_{2n} x^n
\end{align}$$

then we have:

$$\begin{align}
S(x)F(x)
&= u_0 f_0 + (u_0 f_2 + u_2 f_0) x + \cdots + \left(\sum_{k=0}^n u_{2k} f_{2(n-k)} \right) x^n + \cdots \\
&= 0 + u_2 x + \cdots + u_{2n} x^n + \cdots \\
&= S(x) - 1
\end{align}$$

The above we substituted the coefficients of each $$x^n$$ with $$u_{2n}$$,
according to the relations enumerated above. And from this we have:

$$\begin{align}
F(x) &= \frac{S(x) - 1}{S(x)} \\
f &= \lim_{x\nearrow 1} F(x) = 1 - \lim_{x\nearrow 1}\frac{1}{S(x)} = 1 - \frac{1}{\sum_{n=0}^{\infty} u_n}
\end{align}$$

The equation above, $$f$$ is the probability that equalization occurs and
$$\sum_{n=0}^{\infty} u_n$$ is the expected number of visits to 0. Rearranging,
we also have:

$$ \sum_{n=0}^{\infty} u_n = \frac{1}{1-f} $$

Therefore:

- Always for any $$N$$, $$\sum_{n=0}^N u_n \le \lim_{s\nearrow 1} U(s)$$
- If $$\sum_{n=0}^{\infty} u_n = \infty$$, then $$\lim_{s\nearrow 1} U(s) = \infty$$ and $$f = \sum_{n=0}^{\infty} f_n = \lim_{s\nearrow 1} F(s) = 1$$
- If $$\sum_{n=0}^{\infty} u_n < \infty$$, then $$\lim_{s\nearrow 1} U(s) < \infty$$ and $$f = \sum_{n=0}^{\infty} f_n = \lim_{s\nearrow 1} F(s) < 1$$

or the probability of returning to 0 is one iff $$\sum_{n=0}^{\infty} u_n = \infty$$.

# Case of specific dimensions

### d=1

We now apply the above result to a particular dimension. For $$d=1$$, a
symmetric random walk can have $$2^{2n}$$ possible routes in $$2n$$ steps. But
there are only $$\binom{2n}{n}$$ routes will have equal number of left and right
moves, i.e., return to 0 at step $$2n$$.  Therefore,

$$\begin{align}
u_{2n} &= \frac{\binom{2n}{n}}{2^{2n}} \\
&= \frac{(2n)!}{(n!)^2} \frac{1}{2^{2n}} \\
&\approx \frac{\sqrt{2\pi(2n)}(2n)^{2n}e^{-2n}}{(\sqrt{2\pi n}n^ne^{-n})^2} \frac{1}{2^{2n}} \\
&= \frac{1}{\sqrt{\pi n}}
\end{align}$$

The approximation above is using Stirling's formula, $$n! = \sqrt{2\pi n}n^n e^{-n}$$. And so,

$$\sum_{n=0}^{\infty} u_n = \sum_{n=0}^{\infty} u_{2n} \approx \sum_{n=0}^{\infty} (\pi n)^{-1/2} = \infty$$

The last equality can be found by considering:

$$ \sum_{n=0}^{\infty} (\pi n)^{-1/2} \approx \int_0^{\infty} (\pi n)^{-1/2} dn $$

### Asymmetric random walk

The case of asymmetric random walk on $$\mathbb{Z}$$: If we assume the
probability of going right at each step is $$p$$ and that of going left is
$$1-p$$, then we have $$u_{2n} = p^n (1-p)^n \binom{2n}{n}$$, which the above
derivation becomes:

$$\begin{align}
u_{2n} &= p^n (1-p)^n \binom{2n}{n} \\
&= p^n (1-p)^n \frac{(2n)!}{(n!)^2} \\
&\approx p^n (1-p)^n \frac{\sqrt{2\pi(2n)}(2n)^{2n}e^{-2n}}{(\sqrt{2\pi n}n^ne^{-n})^2} \\
&= (4p(1-p))^n \frac{1}{\sqrt{\pi n}}
\end{align}$$

which will converge iff $$p\ne \frac{1}{2}$$. In other words, there is a
probability greater than zero that the random walk will never return to 0.

### d=2

If $$d=2$$, at each step we may go in one of the four directions. Therefore,
there are $$4^{2n}$$ possible routes in $$2n$$ steps and for equalization, the
number of steps going up and down must equal as well as left and right must
equal, which enumerated to be

$$ \sum_{k=0}^n \binom{2n}{k,k,n-k,n-k} = \sum_{k=0}^n \frac{(2n)!}{k!k!(n-k)!(n-k)!} $$

possible routes.

Considering the multinomial expression, we have:

$$\begin{align}
(2n)! &= n!n!\frac{(2n)!}{n!n!} = n!n!\binom{2n}{n} \\
\sum_{k=0}^n {\binom{n}{k}}^2 &= \binom{2n}{n}
\end{align}$$

therefore

$$\begin{align}
\sum_{k=0}^n \binom{2n}{k,k,n-k,n-k}
&= \sum_{k=0}^n \frac{(2n)!}{k!k!(n-k)!(n-k)!} \\
&= \sum_{k=0}^n \frac{n!n!}{k!k!(n-k)!(n-k)!}\binom{2n}{n} \\
&= \sum_{k=0}^n {\binom{n}{k}}^2 \binom{2n}{n} \\
&= {\binom{2n}{n}}^2 \\
\end{align}$$

and the probability

$$\begin{align}
u_{2n}
&= \frac{1}{4^{2n}}{\binom{2n}{n}}^2 \\
&= \frac{1}{4^{2n}}\left(\frac{(2n)!}{(n!)^2}\right)^2 \\
&\approx \frac{1}{4^{2n}}\left(\frac{\sqrt{4\pi n}(2n)^{2n}e^{-2n}}{(\sqrt{2\pi n}n^ne^{-n})^2}\right)^2 \\
&= \frac{1}{\pi n}
\end{align}$$

Therefore,

$$\sum_{n=0}^{\infty} u_n = \sum_{n=0}^{\infty} u_{2n} \approx \sum_{n=0}^{\infty} \frac{1}{\pi n} = \infty$$

### d=3

Using similar argument,

$$\begin{align}
u_{2n}
&= \frac{1}{6^{2n}}\sum_{k=0}^n\sum_{j=0}^k\binom{2n}{j,j,k-j,k-j,n-k,n-k} \\
&= \frac{1}{6^{2n}}\sum_{k=0}^n\sum_{j=0}^k\frac{(2n)!}{j!j!(k-j)!(k-j)!(n-k)!(n-k)!} \\
&= \frac{1}{2^{2n}3^{2n}}\sum_{k=0}^n\sum_{j=0}^k\binom{2n}{n}\left(\frac{n!}{j!(k-j)!(n-k)!}\right)^2 \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \sum_{k=0}^n\sum_{j=0}^k \left(3^{-n}\frac{n!}{j!(k-j)!(n-k)!}\right)^2 \\
&\le \frac{1}{2^{2n}} \binom{2n}{n} \max_{\substack{j,k\\ j+k=n}}\left(3^{-n}\frac{n!}{j!(k-j)!(n-k)!}\right)
\end{align}$$

The last inequality is due to $$\sum_{j,k} a_{j,k}^2 \le \max_{j,k} a_{j,k}$$ if
$$a_{j,k}\ge 0$$ and sum to 1 (due to it is a distribution). Using Stirling's formula here,

$$\begin{align}
u_{2n}
&\le \frac{1}{2^{2n}} \binom{2n}{n} \max_{\substack{j,k\\ j+k=n}}\left(3^{-n}\frac{n!}{j!(k-j)!(n-k)!}\right) \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \left(3^{-n}\frac{n!}{(\left[\frac{n}{3}\right]!)^3}\right) \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \left(3^{-n}\frac{\sqrt{2\pi n}n^ne^{-n}}{(\sqrt{2\pi n/3}(n/3)^{n/3}e^{-n/3})^3}\right) \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \left(3^{-n}\frac{\sqrt{2\pi n}n^ne^{-n}}{2\pi n/3 \sqrt{2\pi n/3} (n/3)^n e^{-n}}\right) \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \left(3^{-n}\frac{1}{2\pi n 3^{-3/2} 3^{-n}}\right) \\
&= \frac{1}{2^{2n}} \binom{2n}{n} \frac{3^{3/2}}{2\pi n} \\
&= \frac{1}{\sqrt{\pi n}} \frac{3^{3/2}}{2\pi n} = O(n^{-3/2})
\end{align}$$

This makes the sum

$$\sum_{n=0}^{\infty} u_n < \infty$$

The probability of equalization $$f$$ is the [Pólya's Random Walk
Constants](http://mathworld.wolfram.com/PolyasRandomWalkConstants.html) and in
3D, it is found to be 0.3405373296

### d>3

On higher dimension, Polya's result is that the probability of equalization is
always less than one. Consider that the random walk is on $$d+1$$ dimension and
represent the position with a vector. If we restrict the $$d+1$$-th element to
be always 0, that becomes a random walk on $$d$$ dimension. Denote $$f^d$$ to be
the probability of equalization on a $$d$$-dimensional random walk. And also we
define $$Z_n$$ to be the event that a random walk on $$d$$ dimension is at a
position that the vector has first $$n\le d$$ elements 0. Obviously $$Z_{n+1}
\subseteq Z_n$$ so does their probability. Hence by induction, we proved

$$ f^{d+1} \le f^d $$

This is already established for $$d=1$$ and $$d=2$$.

# Reference
- Chapter 4 of "Probability. Theory and examples", 4th edition, by Durrett, Cambridge, 2010
- Chapter 12 of "Introduction to probability", 2nd edition, by Grinstead and Snell, AMS, 1997
- [Random Walks](http://www.math.ksu.edu/~cnmoore/randomwalk.pdf), by C. N. Moore, 2007
- Derek Johnston, "[An Introduction to Random Walks](https://www.math.uchicago.edu/~may/VIGRE/VIGRE2011/REUPapers/Johnston.pdf)", 2011
- [Proving that 1- and 2-d simple symmetric random walks return to the origin with probability 1](https://math.stackexchange.com/questions/536/proving-that-1-and-2-d-simple-symmetric-random-walks-return-to-the-origin-with)

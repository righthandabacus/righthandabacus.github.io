---
layout: post
title: Permutation and cycles
date: Thu, 10 May 2018 11:10:54 -0400
tags: math
---

Let $[n]$ be the set $\{1,2,...,n\}$ (we call each element a vertex) and
a permutation of $[n]$ be $\pi=[\pi(1),\pi(2),\cdots,\pi(n)]$, i.e.,
denote $\pi(x)=y$ the fact that in a permutation, position $x$ has vertex $y$.
There are $n!$ possible permutations of $[n]$.

For example, permutation of 1,2,3,4 into 3,2,4,1 has a 1-cycle of 2, i.e.
$\pi(2) = 2$, and a 3-cycle, $\pi(1) = 3, \pi(3) = 4, \pi(4) = 1$.

A cycle is such that we have a sequence of
$\pi(j) = k, \pi(k) = \ell, \cdots, \pi(p) = q, \pi(q) = j$.

## expected number of cycles

Assume all permutation is of equal probability. The expected number of cycles
in permutation of $[n]$ be $h(n)$.  Then obviously $h(1) = 1$ because there is
only one permuatation. $h(2) = 1.5$ for the only permuatation are 1,2 and 2,1
which has 2 and 1 cycles respectively.

$h(n) = \sum_{i=1}^n \frac{1}{n}$. Proof as follows. (adapted from ref #1)

Assume we have a permutation of $[n-1]$, $[\pi(1),\pi(2),\cdots,\pi(n-1)]$, we
have $n$ ways to generate a permutation of $[n]$ from it: insert $n$ before any
$\pi(i)$ or insert $n$ after $\pi(n-1)$. Result of such is either we make $n$
into one of the existing cycle (first $n-1$ ways) or we make $n$ into a 1-cycle
(last way). Therefore we have:

$$\begin{align}
h(n) &= h(n-1) \frac{n-1}{n} + (h(n-1) + 1) \frac{1}{n} \\
     &= h(n-1) + \frac{1}{n}
\end{align}$$

And by induction, $h(n) = \sum_{i=1}^n \frac{1}{i}$

Indeed, we can understand it as $h(n) = \sum_{k=1}^n h_i(n)$ where
$h_k(n) = \frac{1}{k}$ is the expected number of $k$-cycles in the permutation.

## probability of having a n-cycle/1-cycle in a permutation of [n]

$n$-cylce: By counting: $\pi(1)$ can be anything but 1, $\pi(\pi(1))$ can be
anything but 1 or $\pi(1)$, and so on. Therefore we have $(n-1)!$ such
permutations. Alternatively, there are $(n-1)!$ ways of arranging $n$ symbols
in a cycle. Since there are $n!$ possible permutations, the probability of
having $n$-cycle is $(n-1)!/n! = 1/n$.

1-cycle: This means $\pi(k)=k$, so for any particular $k$, the probability is
$1/n$. But note that, this is the probability of any particular vertex in a
1-cycle, which is different from the probability of a random permutation has
any 1-cycle.

The probability that the permutation has only 1-cycle is the probability that
the permutation is an identity mapping, $\pi(v)=v$ for all $v$, which is $1/n!$.

## probability of a vertex in a k-cycle in a permutation of [n]

If vertex $v_1$ is in a $k$-cycle, it means
$\pi(v_1)=v_2, \pi(v_2)=v_3, \cdots, \pi(v_{k-1})=v_k, \pi(v_k)=v_1$.
By counting, we have
$(n-1)(n-2)\cdots(n-k+1) \times (n-k)! = (n-1)!/(n-k)! \times (n-k)! = (n-1)!$
ways to make a $k$-cycle with $v_1$ in it in a permutation. The term after
$\times$ refers to the permutation of remaining $n-k$ vertices not in the
cycle. Therefore, given $v_1$, the probability is $(n-1)!/n! = 1/n$.

Alternatively, we can derive the probability by multiplying the probability of
correct $\pi(v_i)$ in each step (adapted from ref #2): For $\pi(v_1)$, it can be
anything but $v_1$, which has the probability of $\frac{n-1}{n}$. For $\pi(v_2)$,
it cant be anything but $v_1,v_2$ but due to the bijective nature of $\pi(\cdot)$
function, $\pi(v_2)=v_2$ has been ruled out by the fact that $\pi(v_1)=v_2$.
Therefore the probability is $\frac{n-2}{n-1}$. Similarly, for $\pi(v_i)$ up to
$i = k-1$, the probability is $\frac{n-i+1}{n-i+2}$. Finally $\pi(v_k)=v_1$ has
probability of $\frac{1}{n-k+1}$. So the overall probability is

$$ \frac{n-1}{n} \frac{n-2}{n-1} \cdots \frac{n-k+1}{n-k+2} \frac{1}{n-k+1} = \frac{1}{n}.$$

## probability of a large cycle

There are $\binom{n}{k}$ ways to pick $k$ vertices from $[n]$ and each set of
$k$ picked symbols has $(k-1)!$ ways to make a cycle (and the remaining $n-k$
vertices has $(n-k)!$ possible permutations). For $2k > n$, the probability is:

$$ \frac{\binom{n}{k} (k-1)! (n-k)!}{n!} = \frac{1}{k} $$

This is also the probability that a permutation has at least one $k$-cycle for
$k \le n$.

## expected number of k-cycle

In a permutation, there is probability $1/n$ that any vertex is in a $k$-cycle.
If we check each of the $n$ vertices whether it is in a $k$-cycle, the expected
number of affirmative vertices is $n\times (1/n) = 1$. Since if there is a
$k$-cycle, there must be $k$ vertices affirmative to such test, we have the
expected number of $k$-cycle as $1/k$.

## average length of cycle

A permutation always has $\pi(v_i) = v_j$ for each $v_i$. If we consider that as
a graph, there is always $n$ edges. Since we know that the expected number of
cycle in a permutation is $\sum_i \frac{1}{i}$, the average cycle length (i.e.,
counting number of edges, or equivalently the number of vertices), is
$n/\sum_i\frac{1}{i} \approx n/\ln n$.

## Additional properties

From ref #4:

- Probability of having $j_k$ number of $k$-cycle, for $k=1,\cdots,n$, in the permutation is $\prod_{k=1}^n \frac{1}{k^{j_k} j_k!}$
- As $n\to\infty$, the distribution of number of $k$-cycle in permutation converges to Poisson with intensity $1/k$

## Referneces

1. Math Stackexchange, [Name Drawing Puzzle](https://math.stackexchange.com/questions/165407/name-drawing-puzzle), 2012-07-01
2. <http://www.inference.org.uk/itila/cycles.pdf>
3. Kent E. Morrison, "[Random Maps and Permutations](https://www.calpoly.edu/~kmorriso/Research/randommaps.pdf)", 1998-04-14
4. Terence Tao, "[The number of cycles in a random permutation](https://terrytao.wordpress.com/2011/11/23/the-number-of-cycles-in-a-random-permutation/)", 2011-11-23


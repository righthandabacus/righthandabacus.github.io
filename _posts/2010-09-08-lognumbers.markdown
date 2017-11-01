---
layout: post
title: "Log of numbers"
date: 2010-09-08 23:38:54 -0400
lastedit: Tue, 24 Oct 2017 16:19:19 -0400
tags: math
---
It is well known to some of us: When you are dealing with numbers in computer, the floating point arithmetics makes you lost the precision. For example, adding a bunch of small numbers (e.g. a million of millionth) to a large number never gives an accurate result, unless you use some tricks.

One of the common trick is to take log of the numbers so that it preserves precision in such problems. Using log addition to replace multiplication is easy. But how to use log to replace additions?

In <http://blog.smola.org/post/987977550/log-probabilities-semirings-and-floating-point-numbers>, it gives the following formula to do additions:

$$ \log \sum_i x_i = \log\xi + \log\left[\sum_i \exp(\log x_i-\log\xi)\right] $$

where $\xi = \max\{ x_i \}$. The prove is as follows:

$$
\begin{aligned}
\log(A+B) &= \log(A+AB/A) \\
&= \log(A(1+B/A))  \\
&= \log(A) + \log(1+B/A)  \\
&= \log(A) + \log(1+e^{\log(B)-\log(A)})  \\
\end{aligned}
$$

Computationally, we only need to invoke once the `exp()` call and once the
`log()` call. The others are just simple floating-point arithmetics.


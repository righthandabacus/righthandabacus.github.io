---
layout: post
title: "Repeated Hash and Collision"
date: 2012-03-14 11:23:27 -0400
tags: code math
---
Reading an article from [Umumble](http://umumble.com/blogs/crypto/373/) but it seems not very well formatted. So I do the math here again.

Consider a hash function $f$ (e.g., hashing an arbitrary large file into MD5 checksum) that produces $n$ bit output. Since the input space is arbitrary large, we can consider the output space is surjectve, i.e., $2^n$. When we apply $f$ on the output again (first time repetition), the input space is $N=2^n$. The probability that one output is not correspond to a particular input is $\frac{N-1}{N}$, hence the probability that one output is not correspond to *any* input equals to $(\frac{N-1}{N})^N$. Consider the general case that the input space to $f$ is $M$ and its output space is still $N$. The probability that a particular output is not correspond to any input is

$$ \left(\frac{N-1}{N}\right)^M = \left(1-\frac{1}{N}\right)^M \approx e^{-M/N}, $$

where the last approximation is assuming $N$ large enough.

Denote $M_k$ to be the input space for $k$ repetitions, i.e. $M_0 = \infty$, $M_1 = N = 2^n$, $M_2 = Ne^{-1}$, then $M_{k+1}$ = $M_ke^{-M_k/N}$. Hence $M_k$ monotonically decreases (i.e. repeated hashing is bad in terms of collision count) but the rate of decrease is reduced exponentially (i.e. repeated hashing do more harm at the first couple repetitions).

To numerically verify the reduction of output space, run the following Python code:

``` python
#!/usr/bin/env python
from math import exp, log
K, N = 100000, 2**128
Mk = N
print "k\tlog2(M_k)\n1\t%.3f" % (log(Mk)/log(2))
for i in range(1,K+1):
        Mk = Mk * exp(-Mk/N)
        print "%d\t%.3f" % (i+1, log(Mk)/log(2))
```

which gives the following output ($N=2^{128}$ and up to $k=100000$):

```
k	log2(M_k)
1	128.000
2	126.557
3	126.027
4	125.659
5	125.374
....
99997	111.390
99998	111.390
99999	111.390
100000	111.390
```

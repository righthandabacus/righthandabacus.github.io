---
layout: post
title: "Integer Reciprocal and Geometric Series"
date: 2012-07-30 15:06:09 -0400
tags: math
---

Note that
$$
\frac{1}{3} = \frac{1}{4} + \frac{1}{16} + \frac{1}{64} + \frac{1}{256} + \cdots
$$
In fact, we have

$$
\begin{aligned}
\frac{1}{3} &= \sum_{k=1}^{\infty} \frac{1}{4^k}   
\frac{1}{4} &= \sum_{k=1}^{\infty} \frac{1}{5^k}   
\frac{1}{n-1} &= \sum_{k=1}^{\infty} \frac{1}{n^k}
\end{aligned}$$

This fact is useful in performing integer division. We can compute $$m$$ divided
by $$n$$ by recursion (note the summation sign above). Consider $$m/(n-1)$$, we have

$$
\begin{aligned}
m &= nA + B & \exists A,B\in\mathbb{N},\; 0\le B< n  
\textrm{therefore, }\frac{m}{n-1} &= A + \frac{A+B}{n-1}.
\end{aligned}$$

Since $$A = m \textrm{ div } n$$, $$B = m \textrm{ mod } n$$, if $$n$$ is a power of
2, we can perform the div and mod operations by bit shift.

```python
def div(m, n):
    # n+1 is 2^k assumed here
    result = 0
    A = m
    while (True):
       A = A >> k
       B = A & n
       result = add(result, A)
       if A <= n:
           break
    if A == n: result = result + 1
    return result

def add(A, B):
    result = A XOR B
    carry = (A AND B) << 1
    return add(result, carry) if carry else result
```

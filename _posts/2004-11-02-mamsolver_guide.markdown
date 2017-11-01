---
layout: post
title: "How to use MAMSolver"
date: 2004-11-02 22:20:19 +0800
tags: code business
---
# How to use MAMSolver

I run mamsolver with options:

    $ ./mamsolve 0 1 < input.txt

where the 0 and 1 options means I am solving for a CTMC QBD.

## Input
Because of I am using CTMC QBD, the input to the mamsolve is a text file of the following:
  * Line 1: Number of states in boundary portion ($n$)
  * Line 2: Number of states in repeating portion ($m$)
  * Line 3: Number of matrices to describe the repeating portion. In QBD, it is 3.
  * Line 4: Desired accuracy, note that in C, long double only gives a precision to 19 decimal places.
  * Line 5 to $4+n$ ($n$ lines), the intra-boundary transitions in rows, i.e. the matrix $B_{00}$
  * Line $5+n$ to $4+n+m$ ($m$ lines), the transition from level 1 to the boundary, i.e. matrix $B_{10}$
  * Line $5+n+m$ to $4+2n+m$ ($n$ lines), the transition from boundary to level 1, i.e. matrix $B_{01}$
  * Line $5+2n+m$ to $4+2n+2m$ ($m$ lines), the transition from level $i$ to level $i-1$ in repeating portion, i.e. matrix $A_2$
  * Line $5+2n+2m$ to $4+2n+3m$ ($m$ lines), the intra-level transition of level $i$, i.e matrix $A_1$
  * Line $5+2n+3m$ to $4+2n+4m$ ($m$ lines), the transition from level $i$ to level $i+1$, i.e. matrix $A_0$

The above notion of matrices are refer to the [tutorial paper](http://www-net.cs.umass.edu/pe2002/papers/nelson.pdf) by Nelson (different from the `README` file of the mamsolver package). The input can have (arbitrary?) blank lines and blank spaces to enhance readability.

## Output
The output of the mamsolver are the following files:
  * system load measured using the drift condition and caudal charateristic (file "drift-conditions")\\
    Actually I have no idea what it is -- I don't understand what drift condition means.
  * matrix $G$  (file `G-matrix`)\\
    It is a matrix comes out when solving M/G/1 queues. I have no idea what it represents when I ask for the solution of CTMC QBD using matrix geometric method.
  * matrix $R$  (file `R-matrix`)\\
    It is the geometric part. The probability of level i can be calculated by multiplying the probability of level $i-1$ to $R$.
  * stationary probability distribution (file `probability`)\\
    It is the probability of boundary states and level 1 states. Using these and $R$, you can find for others. It should contain $n+m$ lines.
  * queue length distribution (file `QL-dist`)\\
    A list of the queue length and its probability. If you multiply the first number (queue length) to the second number (probability) and sum for all the rows, it should become the number in the following file.
  * average queue length (file `QL-length`)\\
    A single number representing the average queue length

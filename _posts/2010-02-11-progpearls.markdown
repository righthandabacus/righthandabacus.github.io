---
layout: post
title: "Programming Pearls 2e"
subtitle: "Jon Bentley / 1999"
date: 2010-02-11 17:39:25 -0500
category: book
bibtex:
  type: book
  title: Programming Pearls
  edition: 2nd
  author: Jon Louis Bentley
  publish: Addison-Wesley
  year: 1999
  library: Dibner
  classification: QA76.6.B454 2000
  url: http://www.cs.bell-labs.com/cm/cs/pearls/
---

This is a must-read for all the programmers. Codes are in C or C++ but the idea is the fruit of this book.

Some summary as below:

Among all data structures taught in classes, we never talk about bit vectors but in fact, it is the one most compact and solves a number of problems. For example, to check if some integer already in a set.

Always there is a trade off between space and time. But there is also cases to trade between code size and time. Some code optimization technique includes: Loop unrolling, use of sentinels to reduce number of check for loop-stop conditions, replacement of one function by another for speed reduction (e.g. division is time consuming compared to multiplications), and so on. Sometimes, trade off between space and time may not work as it should be. An example from the book is, comparing a linked list to an array for insertion of an item into a sorted item, although linked list needs only one change, but pointer operation is so expensive that makes array more favorable.

Some nice (eureka!) algorithms are proposed:

1) The Aho-Hopcroft-Ullman lazy initialization: Given an array of large size, we don't want to initialize all items to zero at the beginning but they need to be initialized right before it is updated. How can we tell if an item in an array is initialized or not?

> Assume the array of data is `D`, we create two supplementary arrays of integers `X` and `Y`, and a pointer (integer index) `L`. Initially `L` is zero. To check if `D[k]` is initialized, we check `Y[X[k]]=k` and `X[k] < L`. Before using `D[k]`, we first set `Y[L]=k`, `X[k]=L`, then increment `L`. In this case, we trade the space for the time of initialization. And no initialization done in any arrays, ever.

2) How to store a sparse matrix efficiently?

> An element $v$ of position $(m,n)$ in the matrix is a 3-tuple $(m,n,v)$. If the matrix is large and it is unlikely that a row/column is completely empty, we can store using $1\times (N+1)$ array and a $2\times K$ array, where the matrix is $N\times N$ and the total number of non-empty matrix element is $K$. The method is as follows. The $1\times (N+1)$ array `Col[]` corresponds to the columns in matrix, `Col[k]` denoting the aggregate number of elements in columns before column `k` inclusive. `Col[0]` is by definition zero. Then the total number of elements in column `k` is therefore `Col[k]-Col[k-1]`. The 2D array `E[][]` sore s the row number and the element value, e.g. `E[i][0]` is the row number and `E[i][1]` is the element value. Then to look for an element in column `k` row `h`, we do a for-loop of `h` from `Col[k-1]` to `Col[k]` and check for `E[h][0]==k`.

3) How to print a random line in a file in one pass, without knowing the total number of lines in the file before hand?

> If there are $n$ lines in the file, each line is print with probability $\frac{1}{n}$. Because $\frac{1}{n}=\frac{1}{k}\frac{k}{k+1}\frac{k+1}{k+2}\cdots\frac{n-1}{n}$, we can do in this way: Mark line $k$ as the line to print with probability $\frac{1}{k}$, and print the last marked line when all $n$ lines in the file are read. Then line $k$ will be print at the end satisfies the above probability $\frac{1}{n}=\frac{1}{k}\prod_{j=k+1}^{n}(1-\frac{1}{j})$.

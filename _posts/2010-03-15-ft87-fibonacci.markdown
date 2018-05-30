---
layout: post
title: "Fredman and Tarjan (1987) Fibonacci Heaps and Their Uses in Improved Network Optimization Algorithms (JACM)"
date: 2010-03-15 00:03:42 -0400
category: paper
bibtex:
  type: article
  author: "Michael L. Fredman and Robert Endre Tarjan"
  title: "Fibonacci Heaps and Their Uses in Improved Network Optimization Algorithms"
  journal: "Journal of ACM"
  pages: "596--615"
  volume: 34
  number: 3
  month: July
  year: 1987
---
This is the paper that proposed Fibonacci Heap (aka F-heap). Detail is also found on <http://en.wikipedia.org/wiki/Fibonacci_heap> and also chapter 20 of Introduction to Algorithms 2/e.

The implementation of most operation is trivial, except decrease key, which the cascade cut is required. The sole reason for cascade cut is to maintain the bound that any node with degree n has at most $$2^n$$ descendants. Hence we need to have cascade cut to move some nodes/subtrees to become new trees in the F-heap so that, by reducing the degree at the parents, the bound maintains.

---
layout: post
title: "Banner & Orda (2007) Multipath Routing Algorithms for Congestion Minimization (TON)"
date: 2011-05-05 00:05:12 -0400
category: paper
bibtex:
  type: article
  title: "Multipath Routing Algorithms for Congestion Minimization"
  author: "Ron Banner and Ariel Orda"
  journal: "IEEE/ACM Transactions on Networking"
  volume: 15
  number: 2
  pages: "413--424"
  month: April
  year: 2007
---
Brief read.

Multipath routing is well studied, as mentioned by this paper. However, the previous work are mostly using heuristics to solve the optimization problem in multipath routing.

This paper formulates the multipath routing problem as an optimization problem to minimize congestion, which congestion is defined as the maximum utilization rate amongst all the links in the network. The problem is constrained by the flow demand and routing, as well as length restriction on the paths being used for a flow, or the number of paths being used for multipath routing.

It is proved in this paper that, the NP-hard Partition Problem can be transformed into one special case of multipath optimization problem with length restriction, therefore the general restricted multipath problem is also NP-hard.

Next, this paper proposed an algorithm to solve the multipath problem sub-optimally, by gradually relaxing the constrains on the problem.

The contribution of this paper over previous ones is that, it has no heuristics, but to slightly modify the problem to obtain a solution.

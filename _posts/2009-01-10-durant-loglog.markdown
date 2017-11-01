---
layout: post
title: "Durand and Flajolet (2003) Loglog Counting of Large Cardinalities (ESA'03)"
date: 2009-01-10 14:00:58 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Loglog Counting of Large Cardinalities"
  author: "Marianne Durand and Philippe Flajolet"
  booktitle: "Algorithms ESA 2003"
  pages: "605--617"
  publisher: "Springer"
  series: "LNCS 2832"
  howpublished: "ESA'03"
  year: 2003
---
Assume we have a multiset (a set which each item can have multiplicity $> 1$) of
elements from a universe of infinite size. The number of elements in the
multiset is finite and we define the cardinality of the multiset to be the
number of distinct elements in it.

Because the multiset is large, it is not feasible to count the cardinality by
reducing a multiset into a set and find its cardinality instead. The paper
proposed a probabilitic algorithm that use $O(\log\log n)$ amount of memory to
find the cardinality with certain error probability.

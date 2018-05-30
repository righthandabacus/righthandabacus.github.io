---
layout: post
title: "Kumar et al (2004) Space-Code Bloom Filter for Efficient Per-Flow Traffic Measurement (INFOCOM)"
date: 2008-12-23 21:12:19 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Space-Code Bloom Filter for Efficient Per-Flow Traffic Measurement"
  author: "Abhisheck Kumar and Jun (Jim) Xu and Jia Wang and Oliver Spatschek and Li (Erran) Li"
  booktitle: "Proc INFOCOM"
  year: 2004
---
Objective: Perform per-flow traffic measurement (error tolerated) in a high speed with limited memory.

The paper proposed the use of space-code bloom filter, which is a bloom filter with different group of hash functions to mark the membership. Instead of marking the same "bins" in the bloom filter every time the same packet arrives, it randomly picks one group of hash functions from the many to mark the "bins". In order to find the multiplicity of the same packet, query is done by checking how many groups matched. According to the coupon collector problem, $$n=l \ln l$$ packets arrived will cause $$l$$ groups matched.

Because the number of group and number of hash function cannot be large, the error increases as the number of packets in a flow increases. For example, if there are $$l$$ groups, at most we can count for $$n=l \ln l$$ packets. To solve the resolution problem, the paper proposed a series of space-code bloom filter with geometrically decreasing sampling probability. Hence the $$k$$-th space-code bloom filter tells the multiplicity of a higher range but lower absolution resolution then the $$(k-1)$$-th, and so that the relative error keeps constant. The paper call this multi-resolution space-code bloom filter.

The paper also described the estimation of the multiplicity (number of occurrence of a packet from the same flow) using maximum likelihood or mean value estimation.

---
layout: post
title: "Goiri et al (2010) Energy-aware Scheduling in Virtualized Datacenters (Cluster)"
date: 2011-03-14 10:45:53 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Energy-aware Scheduling in Virtualized Datacenters"
  author: "Inigo Goiri and Ferran Julia and Ramon Nou and Josep Ll. Berral and Jordi Guitart and Jordi Torres"
  booktitle: "Proc. IEEE Cluster 2010"
  pages: "58--67"
  year: 2010
---
This paper is about a dynamic job scheduling policy for data centers composed of virtual machines.

Assume we need to do scheduling to optimize for something, such as power
consumption. The way to achieve this is to consolidate virtual machines into
real hardware strategically. But consolidation may incur overhead. In this
paper, a cost value is proposed comprising different aspects of a "cost", which
includes possibility of doing a consolidation without violating requirements
(cost is infinity or zero), power efficiency (cost is a range), and others. Then
the costs of moving a VM ($N$ numbers in total) to a machine ($M$ numbers in total)
is enumerated. The costs are presented as a matrix of $N$-by-$(M+1)$ which a
"virtual machine" is included as idling jobs.

Given the matrix, a VM is moved to a machine by a uphill searching process. The
idea is that, given the cost and the current VM-machine allocation, it is known
that how much the cost raised/decreased if a VM is moved to another machine.
Then, we move the VMs one by one until we can see any further move of any VM can
only make the cost raised, then it is an "optimal" solution.

The optimality is local, as the evolution is a greedy algorithm instead of a
global optimality search.

---
layout: post
title: "Costa et al (2009) Why Should We Integrate Services, Servers, and Networking in a Data Center? (WREN)"
date: 2010-06-22 02:14:26 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Why Should We Integrate Services, Servers, and Networking in a Data Center?"
  author: "Paolo Costa and Thomas Zahn and Antony Rowstron and Greg O’Shea and and Simon Schubert"
  booktitle: "Proc WREN'09"
  month: "August 21"
  year: 2009
  address: "Barcelona, Spain"
---
This paper challenges the idea of using, say, fat-tree in DCN because in a DCN, network bisection bandwidth and low diameter are just part of the criteria. We still have to consider resilience to failure, wiring complexity, and energy consumption.

In the paper, the Borg topology is proposed (3-dimensional hypercube of k servers in each dimension). Then, the paper shows how a topology like this allows a efficient implementation of services, routing, communication, failure detection, etc. Thus in the title of the paper, the term "integrate" means the design that consider everything together as one piece, rather than planned separately.

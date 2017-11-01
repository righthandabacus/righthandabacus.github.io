---
layout: post
title: "Lu et al (2008) Counter Braids: A Novel Counter Architecture for Per-Flow Measurement (SIGMETRICS)"
date: 2009-01-07 12:28:24 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Counter Braids: A Novel Counter Architecture for Per-Flow Measurement"
  author: "Yi Lu and Andrea Montanari and Balaji Prabhakar and Sarang Dharmapurikar and Abdul Kabbani"
  booktitle: "Proc SIGMETRICS"
  year: 2008
---
Objective: Report per-flow throughput measurement using minimal memory so that it can be implemented using SRAM and applied to high-speed network.

Only SRAM is fast enough for high-speed (e.g. OC48) network application.

The method is, hash a flow into various small counters like a Bloom filter and increment those counters. These counters (level-1) will hash into various level-2 counters and when the level-1 counter overflew, the corresponding level-2 counters will be incremented. Such structure can be extended to more levels. Suppose each level has less number of counters than the lower level. The idea behind this design is that, the flow size distribution obeys power law. Thus most of the flows have small count and the high order bits in the counters are probably unused. Thus this paper propose a way to share the high order bits between counters while the total number of bits used are kept small.

The way they report the flow size (called the message passing decoder) is based on an recursive algorithm using the counter values at different layer. The flow size they report is in the format of lower bound-upper bound.

The CB cannot support instantaneous queries of flow sizes. All flow sizes are decoded together at the end of a measurement period.

To read:  
G. Cormode and S. Muthukrishnan, "An improved data stream summary: the count-min sketch and its applications", J. Algorithms, 55(1), Apr 2005.

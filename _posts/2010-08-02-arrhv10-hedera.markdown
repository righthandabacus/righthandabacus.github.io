---
layout: post
title: "Al-Fares et al (2010) Hedera: Dynamic Flow Scheduling for Data Center Networks (NSDI)"
date: 2010-08-02 10:59:58 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Hedera: Dynamic Flow Scheduling for Data Center Networks"
  author: "Mohammad Al-Fares and Sivasankar Radhakrishnan and Barath Raghavan and Nelson Huang and Amin Vahdat"
  booktitle: "Proc NSDI"
  year: 2010
---
This is about flow scheduling for multiroot networks using centralized server.

In a multiroot network, spanning tree is inefficient as it will underutilize the network capacity. A good alternative is to use ECMP. But ECMP has the problem of flow collision. For example, two big flow may overflow a link while some alternative links are idle. This is because of the inefficiency of hashing.

In Hedera, flow-based ECMP is still used. But the big flows (e.g. >1/10 of link speed) that may cause collision are rerouted. The edge switches are measuring flow sizes. If any big flows are found, their info such as source and destination are forwarded to the flow scheduling server. The server then compute the route that can minimize contention and install the computed route into the edge and aggregation switches. In the paper, authors proposed to use "global first fit" and "simulated annealing" to do the minimization.

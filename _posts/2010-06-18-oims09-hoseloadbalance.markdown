---
layout: post
title: "Oki et al (2009) Efficient Load-Balanced IP Routing Scheme Based on Shortest Paths in Hose Model (ICC)"
date: 2010-06-18 13:13:20 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Efficient Load-Balanced IP Routing Scheme Based on Shortest Paths in Hose Model"
  author: "Eiji Oki and Ayako Iwaki and Akeo Masuda and Kohei Shiomoto"
  booktitle: "Proc. ICC"
  year: 2009

---
Similar paper:

  - Eiji Oki and Ayako Iwaki, F-TPR: Fine Two-Phase IP Routing Scheme over Shortest Paths for Hose Model.
    IEEE Communications Letters, 13(4), pp.277--279, April 2009
  - Eiji Oki and Ayako Iwaki, Optimization of IP Load-Balanced Routing for Hose Model.
    In Proc. 21st IEEE Int. Conf. on Tools with Artificial Intelligence, pp.800--805, 2009. (with more detail in the mathematical model and proof)


A previous work proposed S-OSPF (Smart OSPF) which is based on a traffic
matrix, finds the optimal split ratio to load balance a network so that the
network congestion ratio is minimized. The optimal split ratio, $x_{ij}^{pq}$,
denotes the fraction of traffic that originated from node $p$ and destined at
node $q$ should pass through link $(i,j)$. The network congestion ratio is defined
as the maximum link utilization percentage. This ratio is better if lowered.
The objective in S-OSPF is to find the optimal split ratio by linear
programming, with a known traffic matrix.

In this paper, the author argued that it is unrealistic to require the
knowledge of traffic matrix because it is changing frequently and not easy to
obtain. Instead of the detailed knowledge of traffic matrix, the author
proposed that to use hose model instead. Thus it is oblivious to the traffic
detail but only the aggregated incoming/outgoing throughput. The author
extended the linear programming model to the hose model and compared the
performance (the congestion ratio) between this method, classical shortest path
(a baseline which congestion ratio = 1), two-phase routing, and MPLS-TE (which
requires a large number of tunnels created to work).

This oblivious routing cannot defeat MPLS-TE but its performance is close to
two-phase routing, sometimes better.

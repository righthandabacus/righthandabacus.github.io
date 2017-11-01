---
layout: post
title: "Kodialam et al (2008) Advances in Oblivious Routing of Internet Traffic (SIGMETRIC)"
date: 2010-04-05 12:31:19 -0400
category: paper
bibtex:
  type: incollection
  title: "Advances in Oblivious Routing of Internet Traffic"
  author: "M. Kodialam and T. V. Lakshman and Sudipta Sengupta"
  booktitle: "Performance Modeling and Engineering"
  howpublished: "Tutorial sessions of SIGMETRIC 2008"  
  chapter: 5
  pages: "125--146"
  publisher: "Springer"
  year: 2008
---
This is a survey paper on oblivious routing.

Oblivious routing is a set of multipath routes for each source-destination pair in the network.
Traffic is routed along those paths regardless of the traffic matrix. There are two models of
oblivious routing: Send from source to destination directly on a path, or use two-phase routing
by using an intermediate node.

Oblivious routing arise from the fact that, to make routing adaptive to traffic is difficult
because measuring traffic matrix is difficult and reconfiguration of routers in real-time is also
difficult. Therefore, it is better if the routing is insensitive to the traffic matrix.

To measure the performance of oblivious routing, we use the ratio of the maximum link utilization
in traffic-sensitive scheme to that of a oblivious scheme. But the computation, according to the
paper, involves linear programming of exponentially many constraints.

To make the problem more tractable, a hose model is proposed. It is to restrict the total ingress
traffic and egress traffic at any node $i$ to be no more than $R_i$ and $C_i$ respectively. Therefore,
for a matrix $T$, $\sum_j t_{ij} \le R_i$ and $\sum_j t_{ij} \le C_i$.

Two phase routing is to make every node $i$ send $\alpha_j$ amount of every flow to
node $j$ and let node $j$ to redirect the traffic to the destination. Thus every flow is routed
in two steps and therefore the name. In two-phase routing with hose model, the total traffic
between any two nodes $i$ and $j$ is $\alpha_jR_i + \alpha_iC_j$. Therefore,
it is insensitive to individual entries in the traffic matrix, and hence oblivious.

The paper says the performance of two-phase routing of hose model is at least 1/2 of the optimal, and
in practice is around 6%.

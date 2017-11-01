---
layout: post
title: "Kodialam et al (2006) Maximum Throughput Routing of Traffic in the Hose Model (INFOCOM)"
date: 2010-04-07 18:47:16 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Maximum Throughput Routing of Traffic in the Hose Model"
  author: "M. Kodialam and T. V. Lakshman and Sudipta Sengupta"
  booktitle: "Proc INFOCOM"
  year: 2006  
---
This paper established the 2-optimality of two-phase routing in hose model.

In the first half of the paper, various LP models are proposed to solve the maximum link utilization problem in oblivious routing. The max link utilization $\mu$ is defined in

$$ \sum_{i,j} t_{ij}f_e^{ij} \le \mu u_e $$

where $t_{ij}$ is the traffic matrix entry (traffic of node $i$ sending to node $j$) and $u_e$ is the link capacity of $e$, the quantity $f$ is the fraction of traffic $t_{ij}$ sending through link $e$.

The above constraint, together with the constraint defining the quantity $f$, gives a LP problem to minimize $\mu$. But the total number of constraint is infinite because the term $t_{ij}$ span along a real number segment. To solve this problem, we can use a *separation oracle*, which is a set of LP problems with $t_{ij}$ as the variables to maximize the link utilization on each link. These set of LP problems is constrained by the definition of hose model, i.e. column sum and row sum of $t_{ij}$ are upperbounded.

With the LP problems as oracle, we can put their dual into the main problem as well, so that a polynomial size LP problem of finding the minimum max lunk utilization can be found. (cf. section II)

In section III of the paper, this routing model is extended to two-phase routing with generalized traffic split ratios, i.e. the split ratio depends on the source and destination nodes $i$ and $j$. In this case, the total demand between any two nodes is not oblivious to traffic matrix, but instead, equals to

$$ d_{ij} = \sum_k \alpha_j^{ik}t_{ik} + \sum_k \alpha_i^{kj}t_{kj} $$

The provisioned bandwidth between nodes $i$ and $j$ is therefore the maximum of $d_{ij}$ over all possible routing matrices. An LP problem similar to the previous one is proposed, to minimize the max link utilization in this model. Similar technique of using the dual of the separation oracles to reduce a non-polynomial size constraint set into polynomial size constraint set is applied.

Afterwards, the paper proves the 2-optimality of the two-phase routing with generalized split ratio on hose model. The intuitive understanding of the 2-optimality is that, because such two-phase routing will give at most double of demand between any two nodes, if the optimal routing can route a traffic matrix $T$, the optimal two-phase routing can route at least the traffic matrix $T/2$.

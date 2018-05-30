---
layout: post
title: "Chen et al (2010) Generic and Automatic Address Configuration for Data Center Networks (SIGCOMM)"
date: 2010-09-20 16:18:21 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Generic and Automatic Address Configuration for Data Center Networks"
  author: "Kai Chen and Chuanxiong Guo and Haitao Wu and Jing Yuan and Zhenqian Feng and Yan Chen and Songwu Lu and Wenfei Wu"
  booktitle: "Proc SIGCOMM"
  year: 2010
---
In a data center of a large number of nodes, a large number of address configuration is needed. In order to avoid manual configuration of these addresses, this paper investigates how can we derive the correct MAC-to-IP address mapping.

The scenario is that, given a "blueprint" of the intended data center architecture, and a real network of connected nodes, we want to verify (1) whether the network is implementing the blueprint, and (2) which node in the network maps to which node in the blueprint. In this paper, it is proposed a topology collection protocol, which find out the connectivity between nodes (and their MAC addresses) over L2 connections. Then the topology is constructed as a graph $$G_p$$ and compare with a graph $$G_b$$.

First objective is the device-to-logical ID mapping, which is the MAC to IP address mapping. The problem is that, given $$G_p$$ and $$G_b$$ are both large graphs, it is not easy to find their one-to-one mapping even they are isomorphic (graph isomorphism problem). The general graph isomorphism problem is complex but in the context of data center, both graphs are "layered" (core connects to aggregation, aggregation connects to edge, etc.). So this paper proposed an alternative algorithm named O2, which is in polynomial time. The paper further proposed some heuristics that can speed up the algorithm, based on data center network properties.

The second objective is to detect for malfunctions. Malfunctions can be caused by mis-wiring, link failure, or node failure. In any case, the graph between $$G_p$$ and $$G_b$$ are different. The problem is than find the maximum common subgraph (MCS) between them and the part not included in the MCS is where the malfunction is located. The MCS algorithm in general is NP. An heuristic algoritm is proposed to make the MCS problem more tractable, by assuming the data center network's layered properties.

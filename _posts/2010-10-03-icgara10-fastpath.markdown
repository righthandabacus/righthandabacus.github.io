---
layout: post
title: "Ibáñez et al (2010) Fast Path Ethernet Switching: On-demand, Efficient Transparent Bridges for Data Center and Campus Networks (LANMAN)"
date: 2010-10-03 09:57:43 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Fast Path Ethernet Switching: On-demand, Efficient Transparent Bridges for Data Center and Campus Networks"
  author: "Guillermo Ibáñez and Juan A. Carral and Alberto García-Martínez and José M. Arco and Diego Rivera and Arturo Azcorra"
  howpublished: LANMAN
  booktitle: "Proc. 17th IEEE Workshop on Local and Metropolitan Area Networks (LANMAN)"
  year: 2010
---

This paper addresses how to use "transparent bridges" to make switching fast. Instead of spanning tree, this uses shortest paths in the topology to forward packets.

The way it operates is the following: When a host $A$ tries to send a packet to host $B$, it must first send an ARP packet to resolve for the address of host $B$. When a switch receives a ARP packet from $A$ on a particular port $p$, it inserts an entry $(A,p)$ to its forwarding table. Then, the ARP packet is forwarded to all output port as it is a broadcast packet. If later on, the switch receives the same ARP packet from another output port, it is ignored and discarded because it is a "late packet". Similarly for the ARP reply packet, except the the ARP reply is a unicast. Using the ARP request and reply, a switch can build a forwarding table for the whole network. The forwarding table is pointing to the shortest path next hop.

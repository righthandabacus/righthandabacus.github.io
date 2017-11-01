---
layout: post
title: "Abu-Libdeh et al (2010) Symbiotic Routing in Future Data Centers (SIGCOMM)"
date: 2010-07-17 11:10:21 -0400
category: paper
bibtex:
  type: inproceedings
  author: "Hussam Abu-Libdeh and Paolo Costa and Antony Rowstron and Greg O'Shea and Austin Donnelly"
  title: "An Algorithm for Nudity Detection"
  booktitle: "Proc. SIGCOMM"
  year: 2010
---
A paper from Microsoft Research UK. The focus is on a topology named CamCube, which is a 3D torus with no switch node. All nodes in the topology is connected to 6 other nodes to form a 3D lattice. Nodes are addressed by a 3D coordinate, $(x,y,z)$. Routing is done by source routing.

There is a core routing service to help finding the route between source and destination. It handles link failures so that it can route around the failures. Its routing is non-deterministic. There are usually several shortest path from a node to the destination. A packet is forwarded to a direction with least local load. This is similar to hot-potato routing with shortest-path constraint.

In the paper, authors argue that with this single routing, the performance (both application and network level) is not optimal. The paper showed various application-specific routing protocols that aimed at fulfilling the need of different applications which the default routing can't do. Though simulation, the paper showed the improvement in performance.

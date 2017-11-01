---
layout: post
title: "Mosko et al (2005) Multipath Routing in Wireless Mesh Networks (WiMesh)"
date: 2010-04-11 22:57:46 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Multipath Routing in Wireless Mesh Networks"
  author: "Marc Mosko and J.J. Garcia-Luna-Aceves"
  booktitle: "Proc. First IEEE Workshop on Wireless Mesh Networks"
  howpublished: WiMesh
  month: "26 Sep"
  year: 2005
  address: "Santa Clara CA"
---
This paper proposes an algorithm for performing multipath routing in wireless mesh network. The authors argue that, in case of multipath routing, requiring different paths to be link disjoint is not as good as allowing partially shared paths from the reliability perspective.

The paper gives algorithm to keep track on link quality in wireless network and update the forwarding table accordingly. By using such information, one can build a multipath routing scheme with traffic loads distributed using Boltzmann distribution --- send most traffic over best link while maintaining enough traffic to keep weaker links alive (and detect its healthiness).

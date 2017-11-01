---
layout: post
title: "Luebban et al (2009) Fast Rerouting for IP Multicast in Managed IPTV Networks (IWQoS)"
date: 2009-10-04 23:36:39 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Fast Rerouting for IP Multicast in Managed IPTV Networks"
  author: "Ralf Luebben and Guangzhi Li and Dongmei Wang and Robert Doverspike and Xiaoming Fu"
  booktitle: "Proc. IWQoS"
  year: 2009  
---
Assume IP network is 2-connected, one can tune the IP link weights such that the multicast routing path and the unicast routing path between any two routers are failure disjoint, provided that the unicast routing is shortest path based and the multicast routing is PIM-based. Because of the disjoint, a failure in multicast can use the unicast route to reach its next-hops without traffic overlap. Such unicast routes can be implemented as fast reroutes for the multicast node and the rerouted traffic can be encapsulated in a unicast packet destined to the appropriate router to avoid protocol modification.

OSPF specification allows one to set the link weight differently in different direction of a link, this allows the mechanism to work, since multicast tree is built using shortest path from the destination to source and the unicast path is selected using the shortest path from the source to destination.

The link weight setting algorithm is presented in the paper in INFOCOM mini-symposium 2007, "IGP Weight Setting in Multimedia IP Networks".



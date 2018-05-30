---
layout: post
title: "Kini et al (2009) Fast Recovery from Dual Link Failures in IP Networks (INFOCOM)"
date: 2012-03-27 23:38:00 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Fast Recovery from Dual Link Failures in IP Networks"
  author: "Shrinivasa Kini and Srinivasan Ramasubramanian and Amund Kvalbein and Audun F. Hansen"
  booktitle: "Proc. INFOCOM"
  year: "2009"
---
In a 3-edge connected network, the paper propose to build around any node some backup topology for fast recovery. Consider node $$n$$ in the network $$G$$, and there are several links connected $$n$$ to the other part of the topology, if we build subgraphs $$G_1$$, $$G_2$$, etc which each of them has some of the links attached to $$n$$ removed, then the spanning tree rooted at $$n$$ in $$G_1$$, $$G_2$$, etc can be used in case for fast reroute when those "removed" links are failed. The fast reroute is to replace the link connectivity, i.e. use the reroute path to reconnect $$n$$ from/to its neighbour.


---
layout: post
title: "Elmeleegy and Cox (2009) EtherProxy: Scaling Ethernet By Suppressing Broadcast Traffic (INFOCOM)"
date: 2010-10-03 10:32:45 -0400
category: paper
bibtex:
  type: inproceedings
  title: "EtherProxy: Scaling Ethernet By Suppressing Broadcast Traffic"
  author: "Khaled Elmeleegy and Alan L. Cox"
  booktitle: "Proc INFOCOM"
  year: 2009
---
Instead of allowing a Ethernet to broadcast freely, which then limits the scalability, this paper introduces a L2 proxy on Ethernet.

The L2 proxy separates Ethernet into different broadcast domains, each with small number of hosts. The different broadcast domains are connected by the EtherProxies. The proxy intercepts all the broadcast traffic passing to different domains. In Ethernet, there are two kinds of broadcasts: DHCP and ARP. The EtherProxy caches the request/reply so the total broadcast on the network is limited. Then the network can better scale. From the end-hosts' point of view, the network is a seamless big network as it can still reach all nodes using legacy L2 protocol.

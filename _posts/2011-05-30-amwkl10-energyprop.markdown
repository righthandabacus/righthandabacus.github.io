---
layout: post
title: "Abts et al (2010) Energy Proportional Datacenter Networks (ISCA)"
date: 2011-05-30 18:11:16 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Energy Proportional Datacenter Networks"
  author: "Dennis Abts and Michael R. Marty and Philip M. Wells and Peter Klausler and Hong Liu"
  booktitle: "Proc. ISCA'10"
  page: "338--347"
  month: "June 19-23"
  year: 2010
  address: "Saint-Malo, France"
---

A paper from Google describing how to make data center network energy
proportional, i.e. the energy consumption in watts is proportional to the
utilization. The current status is that, servers are more power-hungry than
network because of the over-subscription. However, server is also easier to be
energy proportional. Network equipments, because they are always-on, they use
roughly the same amount of energy regardless the utilization.

In the paper, the energy proportionality in network equipments is provided by
increasing the dynamic range of links. However, it also proposed to use
*flattened butterfly* network so that the energy proportionality can be more
efficient.

The flattened butterfly network (FBFLY, see references in paper) is modeled as
"$$k$$-ary $$n$$-flat" network. Where $$k$$ is the number of nodes in a dimension and
$$n$$ is the dimension. It provides a low diameter network compared to Clos, and
few physical links are used.

The FBFLY network baseline is a $$k$$-ary 2-flat network, which there are $$k$$
switches formed a mesh. Each switch connects to $$c$$ hosts (concentration $$c$$). To
make a 2-flat network (2-dimensional) into 3-flat network (3-dimensional), we
replicate the network for $$k$$ times and connect each switch with its counterpart
in the $$k-1$$ replicates. So in general, there are $$k^{n-1}$$ switches in $$k$$-ary
$$n$$-flat network, each switch connects to $$(n-1)(k-1)$$ other switches and $$c$$
hosts. The routing in FBFLY network is the following: Each move within the same
dimension is one-hop.

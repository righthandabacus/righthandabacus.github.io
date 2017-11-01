---
layout: post
title: "Edwards et al (2009) Diverter: A New Approach to Networking Within Virtualized Infrastructures (WREN)"
date: 2010-10-06 18:55:28 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Diverter: A New Approach to Networking Within Virtualized Infrastructures"
  author: "Aled Edwards and Anna Fischer and Antonio Lain"
  booktitle: "Proc WREN"
  year: 2009
---
This discuss the networking issue of having virtual machines in a cluster environment. The point is to make all VMs looks like and operates like a real machine in a network. However, as a virtualized environment, different VMs may arbitrarily hosted by a physical machine. Therefore, the routing rules, firewall, etc are not working as it should be in default.

The idea of this paper is that, we have a VNET module in the host OS to do some additional networking function. All VM clients are having structured IP addresses, e.g. 10.x.y.z where x is client ID, y is subnet ID, z is host ID. The physical machines, however, are connected by a flat layer-2 network. When a VM client sends a packet, the source and destination MAC addresses (of VM) are rewritten to be the corresponding physical machine. It is rewritten again at the destination to the original VM's MAC. The VNET module also does the routing logics, such as whether the destination can be reached by the source by broadcast or a gateway router is required, where is gateway is actually another VM.

In there, the ARP protocol is done over multicast to reach all physical hosts for resolution. This resolution is more frequent than a traditional network because not only the source host need resolution (for MAC rewriting) the destination physical node also need the resolution for MAC rewrite, before forwarding the packet to a VM.

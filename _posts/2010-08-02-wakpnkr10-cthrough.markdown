---
layout: post
title: "Wang et al (2010) c-Through: Part-time Optics in Data Centers (SIGCOMM)"
date: 2010-08-02 10:52:37 -0400
category: paper
bibtex:
  type: inproceedings
  title: "c-Through: Part-time Optics in Data Centers"
  author: "Guohui Wang and David G. Andersen and Michael Kaminsky and Konstantina Papagiannaki and T. S. Eugene Ng and Michael Kozuch and Michael Ryan"
  booktitle: "Proc. SIGCOMM"
  month: "Aug 30-Sep 3"
  year: 2010
  address: "New Delhi, India"
---
This paper proposed a term, HyPaC network, for hybrid of packet and circuit
switched network. The packet switched network are commodity electrical ethernet
switch and the circuit switched network is optical connections. This paper
built a HyPaC testbed, c-through.

The reason for use of HyPaC in data centers is that, packet switched network
usually oversubscribed at the core as the topology is hierarchical with ToR
switches as leaves of the tree. Therefore, it is necessary to expand the
bandwidth to mitigate congestion. Optical connections are of high bandwidth
(40Gb/s) but they are circuit switched with slow reconfiguration time
(10-20ms). The paper propose to have the ToR switches in the electrical switch
hierarchy and in parallel, they are also connected to a optical switch. As
optical switch of size 320x320 is available, there is no need for a hierarchy
of optical.

A limitation to use such optical setup is that, there is only one optical
connection per switch, therefore the connection is one-to-one. To make
effective use of the optical network, we have to

  - Estimate the cross-rack traffic demand
  - Configure the optical circuit
  - Configure ToR switch to demultiplex traffic into correct path

In c-through, the cross-rack traffic estimation is done by OS kernel. The
kernel did the measurement (as it has the first-hand socket information) and
report to a centralized server. The centralized server find the best subset of
traffic demand to maximize the utilization of optical circuits, then
reconfigure the optical switch. Afterwards, the VLAN setting in servers are
reconfigured so that packets that should go over the optical path carry a
special VLAN tag, such that the ToR switch can forward them correctly.

The paper evaluated the proposal to find that, adding the optical path, the
performance is very close to optimal.

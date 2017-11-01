---
layout: post
title: "Nechaev et al (2009) On Calibrating Enterprise Switch Measurements (IMC)"
date: 2010-06-04 18:50:30 -0400
category: paper
bibtex:
  type: inproceedings
  title: "On Calibrating Enterprise Switch Measurements"
  author: "Boris Nechaev and Mark Allman and Vern Paxson and Andrei Gurtov"
  booktitle: "Proc. IMC'09"
  month: "November 4-6"
  year: 2009
  address: "Chicago, Illinois, USA"
---
A paper I skimmed.

It is about error in measurements done in a switch. There are four properties discussed:

1. Timing. The timestamp of packets traced.
2. Loss. The packets missed by the packet capture.
3. Gain. The packets did not exist but captured. These packets are called *phantoms* by the paper. Gain occurs because switch may replicate a packet when forwarding it. For example, when a packet flood to more than one port, and the measurement is done on multiple output ports, more than one event are recorded for one packet forwarding event.
4. Layout. Identification of topological components, such as whether traffic is intra- or inter-subnet; or the broadcast domain of an IP subnet; or detecting hidden switches.

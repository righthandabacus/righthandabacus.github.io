---
layout: post
title: "Mérindol et al (2008) Improving Load Balancing with Multipath Routing (ICCCN)"
date: 2011-07-13 01:43:39 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Improving Load Balancing with Multipath Routing"
  author: "Pascal Mérindol and Jean-Jacques Pansiot and Stéphane Cateloin"
  booktitle: "Proc. ICCCN"
  pages: "1--8"
  year: "2008"
---
Brief read.

There are two kinds of multipath routing protocols: First family uses labeling
or source routing mechanisms and the second family makes use of hop by hop
routing protocols.

The former rely on path computation algorithms to find the paths and then
position them using some signaling protocols such as RSVP-TE. They allow the
adaptation of load of each route depending on realtime measurements, e.g.
[TeXCP](/2009-10-21-kkdc05-te). However, this can create a large number of
tunnels, and the measurement takes time.

The latter does not have these problems, but we have to guarantee the packets
will go loop-free. The paper is to propose an algorithm that can ensure the
routes are loop free.

---
layout: post
title: "Sarolahti, Allman and Floyd (2007) Determining an appropriate sending rate over an underutilized network path (ComNet 51(7))"
date: 2009-01-10 21:34:29 -0500
category: paper
bibtex:
  type: article
  title: Determining an appropriate sending rate over an underutilized network path (ComNet 51(7))
  author: Pasi Saralahti and Mark Allman and Sally Floyd
  journal: Computer Networks
  volume: 51
  number: 7
  year: 2007
---
Propose to use a SYN packet in TCP (or alike) to advertise a desired sending
rate of $X$, and let the routers respond for or against this rate, or
counter-propose another rate $X'$. The whole idea (a.k.a. QuickStart) is to
allow a faster slow-start to catch-up the available network throughput. Once
the router approves the rate of $X$, the sender can have a jump on the window
size upon receipt of the approval message. Router would only approves the rate
when it is underutilized. Simulation shows that, approval of QuickStart is just
a small fraction of the overall when it is moderately congested. Also,
QuickStart can only help the medium-sized flows, i.e. not the short-lived that
cannot go through the slow start process and also not the large transfers that
last for a significantly long time.

Other existing proposals are: (1) send a small number of packets to probe for
network bandwidth (SwiftStart); (2) leveraging the capacity found by previous
or concurrent sessions (FastStart). Certainly, this is useful only if history
is available.

Packet pair is the technique in SwiftStart to determine the bandwidth in the
early phase of TCP. But the SIGCOMM paper of Allman & Paxson (1999) suggested
that it is difficult.




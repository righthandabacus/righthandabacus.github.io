---
layout: post
title: "Kandula et al (2007) Dynamic Load Balancing without Packet Reordering (CCR)"
date: 2009-10-13 15:06:22 -0400
category: paper
bibtex:
  type: article
  title: "Dynamic Load Balancing without Packet Reordering"
  author: "Srikanth Kandula and Dina Katabi and Shantanu Sinha and Arthur Berger"
  journal: "ACM SIGCOMM Computer Communication Review"
  volume: 37
  number: 2
  pages: "53--62"
  month: April
  year: 2007  
---
Packets out of order came from rerouting a flow from a slow path to a fast path. And the amount of packets out of order is proportional to the delay differential.

This paper proposes "FLARE", the Flowlet Aware Routing Engine. Flowlet is defined as the part of flow that exists as a burst, and separated from the subsequent and prior flowlet for a delay differential. If we treat a flowlet as an atomic unit and reroute is done between flowlets, there would be no reordering because the sooner-sent flowlet must arrive before the later-sent one.

FLARE do periodic estimate of delay differentials by sending ping message on different paths. And the paper is to argue that by rerouting flowlets, one can achieve load balancing or better, fitting load into different paths arbitrarily. There are equations given for the bound of the probability of a path's deficit when the flowlet-based splitting is done. And it proved that flowlet-based splitting is always giving better balancing then flow-based splitting.

The paper also states the reason for flowlet's presence:

  - TCP sender tends to send a whole congestion window in one burst and then wait idle for the rest of its RTT
  - The interrupt coalescing in OSes tends to handle multiple pending interrupts at a time to reduce overhead and this makes the arrival of packets appear to be back-to-back.
  - Silence suppression in VoIP
  - User think time in HTTP

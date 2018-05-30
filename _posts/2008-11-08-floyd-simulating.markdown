---
layout: post
title: "Floyd and Paxson (2001) Difficulties in Simulating the Internet (TON)"
date: 2008-11-08 03:18:18 -0500
category: paper
bibtex:
  type: article
  title: "Difficulties in Simulating the Internet"
  author: "Floyd and Paxson"
  journal: "Trans Networking"
  year: 2001
---
Difficulties of simulation: Heterogenity of topology, protocol, link properties. Also the changing nature of the internet, e.g. routes changing all the time, trend of applications, topology, router features.

Invariants of internet:

  - Diurnal patterns of activity: Daily patterns of activity
  - Self-similarity of packet arrivals
  - Poisson session arrivals: Session means high-level human activities
  - Log-normal connection sizes (size in bytes/duration in seconds of connection)
  - Heavy-tailed distributions: Pareto distribution with shape $$\alpha<2$$
     - Not clear how log-normal and heavy-tail are related
  - For telnet-like session, the packet distribution is Pareto

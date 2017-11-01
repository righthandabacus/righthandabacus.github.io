---
layout: post
title: "He et al (2009) End-to-End versus Hop-by-Hop State Refresh in Soft State Signaling Protocols (Comm Ltrs(13))"
date: 2009-09-15 12:55:34 -0400
category: paper
bibtex:
  type: article
  title: "End-to-End versus Hop-by-Hop State Refresh in Soft State Signaling Protocols"
  author: "Jianhua He and Xiaoming Fu and Zuoyin Tang and Hsiao-Hwa Chen"
  journal: "IEEE Communications Letters"
  volume: 13
  number: 4
  pages: "268--270"
  month: April
  year: 2009  
---
A 3-page paper.

It models a soft-state system, i.e. some state signal with an expiry timer to be installed on routers. Signal is from a source to destination and passing through a number of intermediate routers. Each router have to remember the state. Two refresh approach is described: End-to-end and Hop-by-hop. In end-to-end, only the source can send refresh and all routers pass on the refresh request. In hop-by-hop, each router remember the (independent) expiry time and send refresh on their own. Markov chain models are developed for the E2E and HbH approaches and a formula is provided for (1) the probability of inconsistency, i.e. when a router has a different understanding to the state than the source node; and (2) the message rate, i.e. the number of state signaling message flowing on the network per unit time.

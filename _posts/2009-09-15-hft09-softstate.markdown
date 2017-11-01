---
layout: post
title: "He et al (2009) End-to-End Versus Hop-by-Hop Soft State Refresh for Multi-hop Signaling Systems (ICNP)"
date: 2009-09-15 13:02:57 -0400
category: paper
bibtex:
  type: inproceedings
  title: "End-to-End Versus Hop-by-Hop Soft State Refresh for Multi-hop Signaling Systems"
  author: "Jianhua He and Xiaoming Fu and Zuoyin Tang"
  booktitle: "Proc ICNP"
  year: 2009
---
An extended paper of [He et al (2009)](/2009-09-15-hftc09-softstate)

It models a soft-state system, i.e. some state signal with an expiry timer to be
installed on routers. Signal is from a source to destination and passing through
a number of intermediate routers. Each router have to remember the state. Two
refresh approach is described: End-to-end and Hop-by-hop. In end-to-end, only
the source can send refresh and all routers pass on the refresh request. In
hop-by-hop, each router remember the (independent) expiry time and send refresh
on their own. Markov chain models are developed for the E2E and HbH approaches
and a formula is provided for (1) the probability of inconsistency, i.e. when a
router has a different understanding to the state than the source node; and (2)
the message rate, i.e. the number of state signaling message flowing on the
network per unit time.

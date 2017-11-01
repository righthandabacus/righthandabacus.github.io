---
layout: post
title: "Goodman and Ambrose (1995) Stability of Traffic Patterns in Broadband Networks (JNSM)"
date: 2010-04-09 14:44:21 -0400
category: paper
bibtex:
  type: article
  title: "Stability of Traffic Patterns in Broadband Networks"
  author: "R. M. Goodman and B. E. Ambrose"
  journal: "Journal of Network and Systems Management, Special Issue on Routing in Broadband Networks"
  howpublished: "J Network Sys Management"
  volume: 3
  number: 4
  pages: "371--380"
  month: December
  year: 1995
---
This paper modelled traffic as a time-delayed feedback control system.

The paper assumed a simple model of two networking devices, whose input is $x_1$ and $x_2$ respectively. The input will influent the queue length $q_1$ and $q_2$. Their relationship is modeled as a feedback system: Queue length variable $q_1$ and $q_2$ increases the input $x_2$ and $x_1$ respectively (with time delay $\tau$) because when a queue is too long, rerouting will be triggered. Then the system can be modelled as

$$
T(s)[X_1 - Ke^{-s\tau}Q_1+Ke^{-s\tau}Q_2] = Q_1  \\
T(s)[X_1 - Ke^{-s\tau}Q_2+Ke^{-s\tau}Q_1] = Q_2
$$

The block diagram is presented in the paper.

The paper then set $x_1$ to zero and eliminate $Q_2$ in the above equation, then it deduced the notation of $Q_1/X_2$ and deduced that the rerouting network is stable iff $Ke^{-s\tau}T(s) \ne -\tfrac{1}{2}$. By setting $T(s)$ to be $1/s$, i.e. assume queue length is the integration, it found the bound for $K$ (amplifying factor of the feedback signal) to stabilize the network.

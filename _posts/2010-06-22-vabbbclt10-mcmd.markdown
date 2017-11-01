---
layout: post
title: "Vigfusson et al (2010) Dr. Multicast: Rx for Data Center Communication Scalability (EuroSys)"
date: 2010-06-22 02:04:34 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Dr. Multicast: Rx for Data Center Communication Scalability"
  author: "Ymir Vigfusson and Hussam Abu-Libdeh and Mahesh Balakrishnan and Ken Birman and Robert Burgess and Gregory Chockler and Haoyuan Li and Yoav Tock"
  booktitle: "Proc. EuroSys'10"
  month: April 13-16
  year: 2010
  address: Paris
---

IP Multicast is not used in data center but it should be beneficial. This is because of several reasons:

  - Limited scalability of switch/router: Experiments found that as the number of multicast group joined,
    the packet loss rate increases exponentially.
  - Potential of broadcast storms: triggers loss, security concern
  - Disruptive to non-multicast applications
  - Administrator's control of multicast use is limited

In this paper, Dr. Multicast (MCMD) is proposed to allow the use of multicast
in data center, by providing policy enforcement to admin, transparency to
legacy multicast API, high performance, and robustness. What MCMD does is to
provide a library to overload standard socket interface so that multicast
requests can be intercepted; and an agent to implement the policy and multicast
mapping. The library communicates with the agent. The agent controls the
operation of multicast. One of the important role of the agent, besides
administrative features, is to optimize the use of resources:

  - minimize the use multicast addresses by grouping similar multicast groups into the same address
  - minimize redundant sender transmissions by avoiding multiple unicasts
  - minimize receiver filtering, which filtering happens when unsolicited traffic sent to a host

This optimization is merely a *channelization problem*, which is to "allocate a
fixed number of multicast addresses to collections of groups to minimize both
sender transmission costs and receiver filtering costs such that subscribers
receive all messages they are interested at least once"

The paper formulates the optimization problem (NP-complete) and provided heuristic algorithm.

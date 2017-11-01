---
layout: post
title: "Shieh et al (2011) Sharing the Data Center Network (NSDI)"
date: 2011-07-10 01:11:10 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Sharing the Data Center Network"
  author: "Alan Shieh and Srikanth Kandula and Albert Greenberg and Changhoon Kim and Bikas Saha"
  booktitle: "Proc. NSDI"
  year: "2011"
---
This paper describes "Seawall", a mechanism to be used in a multi-tenant data center for network bandwidth control.

The problem it addressed is that, if we rely on TCP to do congestion control on the network, we may see unfairness between users as the fair-share is based on connection, hence the bandwidth is prioritized to the users of high fan-in connections, e.g. reduce tasks in map-reduce applications. This makes the performance bad as this may leave CPU idle. In other words, network causes poor scheduling of jobs.

Seawall is designed in terms of "entities". An entity is a traffic source that is confined to a *single node*, e.g. a VM, a process, but not a tenant which can go across multiple nodes. For each entity, a network weight is assigned. The objective is to share the link bandwidth to entities in proportion to the network weight of the current link users.

Seawall is an end-to-end solution. It puts a shim layer to the hypervisors to force all traffic to go through it. It encapsulates packets to create logical tunnels between nodes and perform congestion control based on tunnels. The packets on a tunnel are stamped with a sequence number to facilitate loss detection. To prevent traffic enters network without the shim layer, 802.1x authentication is suggested.

The control is based on feedback (e.g. ECN, loss of packets) on a regular interval T. The receivers feedback congestion information as well as other control messages (e.g. block a traffic) to the senders. The way they can prevent high fan-in users get more bandwidth share is to do AIMD-like bandwidth sharing (adjusting allocated rate explicitly) on entities instead of tunnels. Therefore, a user sending multiple flows is treated as one entity in bandwidth control. After the rate on a user is determined, the rate is then divided to each tunnel as the next step. Hence the "fairness" can be done on entity level, but not flow level.

Remark: References contains works on improving map-reduce systems.

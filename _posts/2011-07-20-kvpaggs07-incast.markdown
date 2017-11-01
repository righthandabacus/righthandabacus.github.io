---
layout: post
title: "Krevat et al (2007) On Application-level Approaches to Avoiding TCP Throughput Collapse in Cluster-based Storage Systems (SC)"
date: 2011-07-20 01:14:47 -0400
category: paper
bibtex:
  type: inproceedings
  title: "On Application-level Approaches to Avoiding TCP Throughput Collapse in Cluster-based Storage Systems"
  author: "Elie Krevat and Vijay Vasudevan and Amar Phanishayee and David G. Andersen and Gregory R. Ganger and Garth A. Gibson and Srinivasan Seshan"
  booktitle: "ACM Supercomputing"
  month: "Nov"
  year: "2007"
  address: "Reno, NV"
---
A short paper that addresses the approaches to be taken by the application layer to mitigate the incast scenario.

TCP incast is a throughput collapse caused by small switch buffer sizes supporting a communication pattern of concurrent flood of traffic from many servers. This results in packet loss and TCP RTO in one ore more flows. The RTO is in the order of hundreds of milliseconds, which is orders of magnitude greater than the data fetch times (RTT), and this significantly degrade the overall throughput. The environment studied in this paper is the storage cluster. Data blocks are striped over several servers, each one stores a fragment of the data block, known as Server Request Unit (SRU). A client requesting a data block sends to all the servers containing the data, i.e. synchronized read.

TCP incast can be mitigated by (1) larger buffer space, but it is expensive to use large high speed memory; (2) using NewReno or SACK instead of Reno, but it only delayed the throughput collapse to more servers; (3) drastically reducing the RTO timer, but needs high resolution (microsecond) timer. Ethernet flow control can also mitigate the incast, but it is not available in the market at the time of writing.

Application-level solution to incast exists:

  - Increase the SRU size: Request more data from fewer servers
  - Limiting the number of servers in a synchronized data transfer
  - Throttling data transfer: By advertising a smaller TCP rwnd. But may have the adverse effect of underutilizing the client's link capacity
  - Staggering data transfer: Client can limit the number of synchronous transfer by staggering the server responses so that only a subset of servers are sending data at any one time. Servers can also skew their responses by making random delay to response the request.
  - Scheduling the data transfer globally. For example, use a controller to issue tokens for clients to send requests to servers, then we can limit the number of synchronized reads.

Further to read:

  - RFC3042 Limited transmit in TCP
  - M. Wadekar. Enhanced Ethernet for Data Center: Reliable, Channelized and Robust. In 15th IEEE Workshop on Local and Metropolitan Area Networks, June 2007


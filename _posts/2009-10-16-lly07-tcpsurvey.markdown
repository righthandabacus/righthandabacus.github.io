---
layout: post
title: "Leung et al (2007) An Overview of Packet Reordering in TCP: Problems, Solutions, and Challenges (Tran. Parallel & Dist Sys)"
date: 2009-10-16 16:12:58 -0400
category: paper
bibtex:
  type: article
  title: "An Overview of Packet Reordering in TCP: Problems, Solutions, and Challenges"
  author: "Ka-Cheong Leung and Victor O. K. Li and Daiqin Yang" 
  journal: "IEEE Transactions on Parallel and Distributed Systems"
  volume: 18
  number: 4
  pages: "522--535"
  month: April
  year: 2007  
---

A survey of different approaches to solve the packet reordering problem related to TCP.

The causes of packet reordering are:

  - Packet-level multipath routing
  - Route Fluttering: oscillates among a set of available routes
  - Parallelism in Modern High-Speed Routers, e.g. packet striping and use of multiple ASICs in parallel
  - Link-Layer Retransmissions, esp in wireless networks
  - Router Forwarding Lulls: Some routers pause its forwarding activity for buffered packets when it processes a routing update

The impact of packet reordering to TCP are:

  - Spurious retransmisions
  - Keeping cwnd small
  - Loss of self-clocking, i.e. induces bursts
  - Interferes RTT and RTO estimation

The paper classify TCP reordering solutions into "ordinal approach" and "temporal approach". The former is to adjust the TCP algorithms and the latter is to take time into account to solve the problem. There are several algorithms under the category of ordinal approach:

  - Eifel algorithm: Use timestamp to help identify spurious retransmission
    and then restore the ssthresh and cwnd
  - TCP-DOOR (Detection of Out-of-Order and Response): Extension of Eifel,
    which tailor for ad hoc networks that changes routes frequently. When
    out-of-order is detected, because in ad hoc network it is likely to have
    multiple route changes in a short time, it ceases the congestion control
    reaction for certain time. It also restore the congestion response state
    after it found the network stabilised again.
  - DSACK is to exploit the SACK option in TCP so that duplicated transmission
    are reported by the receiver to the sender, and thus the sender can know
    about the spurious retransmission. This DSACK option helps detecting
    packet reordering and subsequent action can be taken.
  - Lee-Park-Choi Sender Algorithm propose to increase the dupthresh
    logarithmically with the number of paths used
  - Blanton-Allman algorithm proposes to change dupthresh dynamically, in 3
    ways: (1) increase dupthresh by some constant every time a spurious retx
    is detected, (2) update dupthresh by taking the average of current dupthresh
    and the current length of reordering, (3) use EWMA to compute the reordering
    length
  - RR-TCP improves over the Blanton-Allman algorithm such that a FFR Avoidance
    ratio is determined and such ratio in turn determines the value of dupthresh,
    so that the dupthresh can change for the same ratio. RR-TCP also propose to
    avoid Karn's algorithm but instead, use timestamp to enhance the RTT estimation
  - Leung-Ma algorithm extends Blanton-Allman algorithm by using EWMA plus the
    mean deviation of reordering length to set the dupthresh and found to have
    similar performance as RR-TCP but lower computational complexity
  - RN-TCP (Reorder Notifying) asks the router to get involved so that when a
    packet is dropped, the sender is notified and thus TCP can avoid spurious
    retransmission

The temporal approach mentioned in the paper are:

  - Lee-Park-Choi Receiver Algorithm to delay ACK for reordered packets but ACK
    immediately for retx packets
  - TCP-DCR (Delayed Congestion Response) to hold off the congestion response for
    a time interval after the first dup ACK is received but send one new packet
    upon each dup ACK to maintain self-clocking
  - TCP-PR is for network with persistent packet reordering, it retransmit only
    after a RTO, but the RTO is computed using non-smoothed, exponentially weighted
    *maximum* RTT so that RTT spikes can promptly reflected in the estimated
    RTT. The new RTT/RTO estimators are effective to tell if the reordering is
    caused by path delays

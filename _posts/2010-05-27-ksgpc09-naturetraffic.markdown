---
layout: post
title: "Kandula et al (2009) The Nature of Datacenter Traffic: Measurements & Analysis (IMC)"
date: 2010-05-27 12:23:44 -0400
category: paper
bibtex:
  type: inproceedings
  title: "The Nature of Datacenter Traffic: Measurements & Analysis"
  author: "Srikanth Kandula and Sudipta Sengupta, Albert Greenberg, Parveen Patel, Ronnie Chaiken"
  booktitle: "Proc. IMC'09"
  month: "November 4-6"
  year: 2009
  address: "Chicago, Illinois, USA"
---
A paper from Microsoft on data center traffic patterns.

It measures traffic from socket level. From the data, the following are found:

  - In general, data center traffic is more regular than ISP networks, due to tighter coupling between
    different components
  - Because data center applications prefer placing jobs with high traffic exchange close to each other for
    higher bandwidth, there is a work-seeks-bandwidth pattern, i.e. high intra-rack or intra-server traffic
  - There are frequent one-to-all or all-to-one traffic, correspond to the map and reduce operation. This is
    a scatter-gather pattern.
  - The traffic between servers are heavy-tailed. Servers within the same rack are more likely to exchange
    more traffic
  - Median number of correspondance for a server is: 2 other intra-rack server, and 4 servers outside the
    rack
  - If define congestion as 70% of link speed, among the 150 inter-switch links, 86% of links observe
    congestion lasting more than 10 sec in a day and 15% links observe congestion lasting more than 100 sec
    in a day. Most congestion is short-lived
  - If a job sends traffic over a congested link, they are 110% more likely to fail to read the input
  - Reduce jobs are responsible for a fair amount of traffic. Extract jobs in Dryad as well, which converts
    raw data into input stream
  - Defining a flow as five-tuple flow, most flows are short-lived (80% flows are less than 10s), and very
    few are long running (less than 0.1% last longer than 200s)
  - Traffic changes quickly
  - Several times a day, all used links run close to capacity
  - TCP incast is not observed. Number of contending flows are small as well. Reasons could be,
    (1) the number of open connections are limited, (2) local communications such as intra-server or
    intra-rack are preferred.
  - A potential problem: Due to small BDP, window of TCP are small. A few packet drop will easily make
    TCP timeout.

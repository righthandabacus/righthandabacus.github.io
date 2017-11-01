---
layout: post
title: "Benson et al (2009) Understanding Data Center Traffic Characteristics (WREN)"
date: 2011-04-16 10:30:15 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Understanding Data Center Traffic Characteristics"
  author: "Theophilus Benson and Ashok Anand and Aditya Akella and Ming Zhang"
  booktitle: "Proc. WREN'09"
  month: "Aug 21"
  year: 2009
  address: "Barcelona, Spain"
---
This is about an analysis of data collected in 19 data centers. The data center is supporting a wide range of applications, such as search, streaming, map-reduce, etc. Two set of data are collected: (1) SNMP data of every network device in a 5-minute interval for 10 days; and (2) TcpDump packet traces of 5 switches in a data center.

The data center architecture is tiered (with 2-3 layers) with core busier than the edge. Findings according to the paper:

  - At any moment, only less than 60% of core and edge links, and 73% aggregation links are used
  - Utilization according to 5-min SNMP is higher in core, due to multiplexing
     - Since the DCN in examination has 4x aggregation links than core links, thus it is not a fat-tree
  - Links are not likely idle for long time. Over the 10 day period, only less than 5% links
    can idle for more than 2 hours accumulatively
  - Packet sizes in the network are bimodel: peak at around 40B (TCP ACK?) and 1470B (Ethernet limit)
  - Core links observe the least lost, edge the greatest: Traffic is bursty in edge and aggregation
  - Large number of links are unused in any 5-min period, but exact set of idle links
    is constantly changing
  - Traffic at microscopic view reveals a ON-OFF pattern
  - The length of ON period, length of OFF period, and packet interarrival times during ON period
    all shows a lognormal distribution


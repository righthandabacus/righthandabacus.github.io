---
layout: post
title: "Nechaev et al (2010) A Preliminary Analysis of TCP Performance in an Enterprise Network (INM)"
date: 2010-05-29 10:15:49 -0400
category: paper
bibtex:
  type: inproceedings
  title: "A Preliminary Analysis of TCP Performance in an Enterprise Network"
  author: "Boris Nechaev and Mark Allman and Vern Paxson and Andrei Gurtov"
  howpublished: INM
  booktitle: "Proc. Internet Network Management Workshop/Workshop on Research on Enterprise Networking"
  month: April
  year: 2010
---
A paper reporting the measurement of internal traffic in LBNL from Oct 2005 to Mar 2006. The measurement is done on many individual switch ports instead of using a central measurement point, so that more traffic can be captured. The finding is with the incomplete connections erased.

What learned are:

  - The network is healthy, as a IMC 2008 paper (ref 3) claims. Namely, it has a bit more than 66%
    successfully established connections
  - The intra- and inter-subnet traffic are 56% and 44% respectively. Thus using a central point for
    measurement is not accurate because this only captures the inter-subnet
  - Most applications are either sending a lot of bytes or creating a lot of connections, but not both.
  - Media transfer size is $<$ 2KB, but a few connections are extremely large ($>$ 5GB)
  - nearly 60% connections have the responder sending more data than the originator (up to 1.2M times more)
  - over 30% connections have the originator send more than the responder (up to 1.2B times more)
  - Heavytail: Top 15 flows send 57% of bytes. Top 160 connections send 90% of traffic, amongst 532K
    connections. Moreover, median rates are 2 Kbps and 10 Kbps for originator and responder respectively
    but 50% of connections can send faster than 100 Kbps.
  - Amongst 292M packets, only 583 has checksum error
  - Reordering occur in 484 connections. Packet out of order at a rate of 0.0025%
  - Intra-subnet transfer is in an order of magnitude faster than inter-subnet.

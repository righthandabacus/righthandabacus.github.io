---
layout: post
title: "Koponen et al (2010) Onix: A Distributed Control Platform for Large-scale Production Networks (OSDI)"
date: 2010-10-04 16:09:42 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Onix: A Distributed Control Platform for Large-scale Production Networks"
  author: "Teemu Koponen and Martin Casado and Natasha Gude and Jeremy Stribling and Leon Poutievski and Min Zhu and Rajiv Ramanathan and Yuichiro Iwata and Hiroaki Inoue and Takayuki Hama and Scott Shenker"
  booktitle: "Proc. 9th OSDI"
  year: 2010  
---
This paper describes the implementation of a network operating system called Onix. Takes the idea of NOX, it provides API for network-wide control.

In Onix, the element of network control is in terms of NIB (network information base) objects. They are stored in (1) a transactional database if slow access is permitted or (2) a DHT for real-time access. Onix addresses the issue of scalability and reliability by managing the NIB. For example, it allows partitioning of work by allowing a control to keep only a subset of NIB. It also allow aggregation of many NIB objects in a cluster as one NIB object in another server.

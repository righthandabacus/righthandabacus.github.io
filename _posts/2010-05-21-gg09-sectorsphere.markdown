---
layout: post
title: "Gu and Grossman (2009) Lessons Learned From a Year's Worth of Benchmarks of Large Data Clouds (MTAGS)"
date: 2010-05-21 15:14:15 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Lessons Learned From a Year's Worth of Benchmarks of Large Data Clouds"
  author: "Yunhong Gu and Robert L Grossman"
  booktitle: "Proc MTAGS'09"
  month: "Nov 19"
  year: 2009
  address: "Portland, Oregon, USA"
---
An alternative implementation (Sector/Sphere) of MapReduce computation in U. Illinois Chicago.

The hardware architecture is racks of computers with terabytes of local hard disk. It is designed to work with multiple sites instead of single site.

# Sector

Sector is a distributed storage system with fault tolerance. The fault tolerance is realized by replication. Replicas are placed according to network topology, so as to place replicas as far as possible to prevent failure due to site power loss. The computation can therefore select the nearest replica.

Sector does not split file into blocks but keep a whole file (or several files) in one block. This is to improve performance because file is atomic to processing. The transport protocol used by Sector is UDT (UDP-based, by the same author).

Because storage is local to computation, the scheduling can strategically assign computation and storage to minimize network load.

# Sphere

The computation is by a user-defined function (UDF). UDF is processing a data segment, which can be a record, a file, up to a directory. The UDF is loaded to all worker (slave) nodes. The UDF processing can be specified with a bucket ID, so that the output is stored into the ID. An bucket ID can hold multiple output records.

The load balancing of sphere is done by splitting jobs into relatively small segments. This works well even with heterogeneous computation power.

Hot spot can occur when multiple source is sending to the same destination. It is avoided by a decentralized approach: When a node want to send data segment to another node, it asks for how many nodes are sending at the same time. If too much, the send will be deferred.

# Evaluation

The paper evaluated the performance of sector/sphere against Hadoop. The following are found:

  - Sector/Sphere compute faster than Hadoop
  - Neither can fully utilize the CPU as the application is I/O bound
  - Hadoop use more CPU and memory, because of the Java VM
  - Sector has higher network I/O, of 60Gb/s over 120 nodes

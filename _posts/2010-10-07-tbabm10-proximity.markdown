---
layout: post
title: "Tiwana et al (2010) Location, Location, Location! Modeling Data Proximity in the Cloud (HotNets)"
date: 2010-10-07 16:31:33 -0400
category: paper
bibtex:
  type: inproceedings
  title: Location, Location, Location! Modeling Data Proximity in the Cloud
  author: "Birjodh Tiwana and Mahesh Balakrishnan and Marcos K. Aguilera and Hitesh Ballani and Z. Morley Mao"
  booktitle: "Proc HotNets"
  year: 2010
---
In a cloud computing environment, the response time of an application is
correlated to the computation node's proximity to the data.

According to the paper, the current practice is to use the closest server to
handle a user's job. Then, by predicting the future access patterns based on
history, move the data to closer to the computation nodes.

This paper do the reverse: Expose the information of the location of the data
to the application, and let the application to decide where to launch the
computation. Different storage technology, however, has different
*replication topologies*. A replication topology is a term in the paper to define how a
read/write action involves different nodes. For example, if the storage is
mirrored, the read involves any copy of the mirror but write involves sending
the write instruction to all mirrors. These actions can be formulated as a
directed acyclic graph (DAG). The paper suggests that, to have a middle layer
called *Contour* between the storage system and the application. The storage
system can return the replication topology of a particular file when Contour
requests. This topology includes the exact location of nodes involved. Contour
system provides function to the application so that it can tell which are the
nodes that an application should launch the computation so that the computation
is closest to the data, or so that the computation can satisfy certain latency
constraints.

---
layout: post
title: "Isard et al (2009) Quincy: Fair Scheduling for Distributed Computing Clusters (SOSP)"
date: 2010-09-06 22:20:56 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Quincy: Fair Scheduling for Distributed Computing Clusters"
  author: "Michael Isard and Vijayan Prabhakaran and Jon Currey and Udi Wieder and Kunal Talwar and Andrew Goldberg"
  booktitle: "Proc. SOSP"
  year: 2009  
---
A paper from Microsoft Research Silicon Valley.

Consider a cluster, with jobs submitted to the cluster are run by some computers. A job comprises of a number of "work" (sub-job) and there is a centralized scheduler to assign work to computers. Each job has a "root" node to coordinate the result from each work and assemble the return data. A job is not finished until all works are finished.

The problem of such system is that, (1) some job may be starved when a work is not scheduled to run in a long period of time, (2) some work runs faster on certain computers, e.g. locality of raw data. An orthodox solution is to use queue scheduler. A queue for each computer (and also for each rack and the whole cluster). A work can join the queue of preferred computer, rack, and cluster. Whenever a computer becomes idle, a work is dispatched from a queue, with preference to its own queue, then the rack's queue, then the cluster's queue. When a work is dispatched, it is deleted from *all* queues in the cluster. In this way, if a computer's queue is idle, it can take over a work from other computer (by dispatching from rack's queue, etc). So that, firstly, work's preference to certain computers can be maintained, and secondly, keeps system utilization high. Furthermore, we can have some variation on this scheduling system. For example, block a job when it has more than $$j$$ work running, etc.

To make the scheduler more flexible, this paper proposed a graph-based approach. Each computer, rack, and the cluster is represented by a node in a graph. There are also a node for each job to represent "unscheduled" work, and a "sink" node. Each work is then a "source" node. We draw directed edges from each rack to its computers, the cluster to each rack, and each computer and "unscheduled" node to the sink. Then the source node can have multiple directed edges to the nodes it preferred. Usually, a work (source node) connects to the preferred computer, the preferred rack, the cluster, and the "unscheduled" node. Each edge carries a weight, which is the cost to run a work on this direction. Once the graph is complete, the scheduling problem is to find the min cost max flow from source to sink. The max flow is fixed by each source sends one unit of flow and each edge has capacity of one.

Using this approach, different scheduling criteria (e.g. different fairness, preemption) can be done by adjusting cost on edges.

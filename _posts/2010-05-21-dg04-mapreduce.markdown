---
layout: post
title: "Dean and Ghemawat (2004) MapReduce: Simplified Data Processing on Large Clusters (OSDI)"
date: 2010-05-21 15:43:29 -0400
category: paper
bibtex:
  type: inproceedings
  title: "MapReduce: Simplified Data Processing on Large Clusters"
  author: "Jeffrey Dean and Sanjay Ghemawat"
  howpublished: "Proc. OSDI'04"
  booktitle: "Proceedings of 6th Symposium on Operating Systems Design and Implementation (OSDI'04)" 
  pages: "137--149"
  year: 2004

---
The Google paper on MapReduce.

Map and Reduce are two functional operations modeled after Lisp. It is an abstracted model for computation. Map operation converts an input key/value pair into intermediate key/value pair. Reduce operation combines all intermediate output of the same key into final value. The input key/value are in the different domain than the output key/value. But the intermediate key/value domain are the same as the output.

The architecture is built from commodity PC with Ethernet switch. Users submit jobs to a scheduling system. The execution is the following:

  - User send job to the MapReduce library
  - The library, partitions job into $$M$$ splits (typically 16-64MB per split)
  - The program is sent to all workers, and one copy to the master
  - The master assigns map role or reduce role to each worker
  - The "map" workers read input split, filter through the Map function, and buffer the output
  - The buffered output is write to disk periodically, then pass the disk location to the master
  - Master notifies each of the $$R$$ "reduce" workers to read from the disk, then the intermediate data
    is sorted by the key to aggregate the values of the same key. Afterward,
    the key/value pairs are filtered through the Reduce function
  - When all MapReduce job are done, master notifies user about the completion.

The master node holds status information about nodes and jobs. Each map/reduce task has a corresponding state (idle, in-progress, completed) and the responsible worker node.

Fault tolerance is maintained by having the master ping workers periodically to learn their healthiness. Lost data due to failure is replaced by re-executing jobs.

There are some tuning for the performance: Input is splited to small pieces. Usually the size $$M$$ and $$R$$ are much larger than the worker machines. Moreover, when the MapReduce job near completion, the master schedules multiple backup executions for the remaining in-progress tasks. When any one of them completes, the task completes. This is to avoid stragglers, which a slow node delays the completion of the whole job. Furthermore, a combiner, which is a local reduce operation can be supplied to pre-process the intermediate output of the "map" worker before sending to the "reduce" worker over the network.

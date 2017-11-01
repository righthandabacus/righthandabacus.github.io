---
layout: post
title: "Phanishayee et al (2008) Measurement and Analysis of TCP Throughput Collapse in Cluster-based Storage Systems (FAST)"
date: 2011-07-20 01:16:27 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Measurement and Analysis of TCP Throughput Collapse in Cluster-based Storage Systems"
  author: "Amar Phanishayee and Elie Krevat and Vijay Vasudevan and David G. Andersen and Gregory R. Ganger and Garth A. Gibson and Srinivasan Seshan"
  howpublished: "FAST'08"
  booktitle: "Proc. 6th USENIX Conference on File and Storage Technologies (FAST'08)"
  month: "Feb"
  year: "2008"
  address: "San Jose, CA"
---
Describes the incast problem in storage networks. The incast problem is first described in the following two literatures:

```
@inproceedings{
  author = {G. A. Gibson and D. F. Nagle and K. Amiri and J. Butler and F. W. Chang and H. Gobioff and C. Hardin and E. Riedel and D. Rochberg and J. Zelenka},
  title = {A Cost-Effective, High-Bandwidth Storage Architecture},
  booktitle = {Proc. 8th Int. Conf. on Architectural Support for Programming Languages and Operating Systems (ASPLOS)},
  address = {San Jose, CA},
  month = {Oct},
  year = 1998,
}
@inproceedings{
  author = {D. Nagle and D. Serenyi and A. Matthews},
  title = {The Panasas ActiveScale Storage Cluster: Delivering Scalable High Bandwidth Storage},
  booktitle = {Proc 2004 ACM/IEEE Conf on Supercomputing (SC'04)},
  address = {Washington DC},
  year = 2004,
}
```

Incast is caused by multiple senders overwhelming a fixed-size switch buffer. This is an important problem in storage network because, for example, parallel NFS (pNFS) is implemented as a component in NFS v4.1 to support parallel data transfer and data striping across multiple servers.

This paper reproduces incast in a setup using different switches:

  - HP Procurve 2828 Ethernet switches in a tree hierarchy, connect up to 64 servers
  - Force10 S50 switch connects 48 machines
  - Force10 E1200 switch with 672 ports with at least 1MB output buffer per port, connecting 87 servers to 1 client

It found that incast occurs in the first two but not the last one, due to the large buffer in E1200 switch. The per-port buffer size in the experiment is not unknown because most switches allocate output buffer from a memory pool dynamically. The role of buffer in incast can be further confirmed with QoS enabled. As QoS enabled, the output buffer would be partitioned to support different classes of services, results in a smaller effective size of output queues. Hence the incast is more severe.

The paper also validated the result with simulation in ns2. It found that TCP RTO is the primary cause of incast. While only 2% of traffic are retransmitted packets, the degradation in goodput is due to underutilization during timeout. Moreover, the servers do not receive next job until the client received all the responses.

Three types of timeout are observed during incast:

  - Full window loss: The entire window of data is lost and thus no feedback is available
  - Last packet loss: The last packet of the SRU is dropped and no further data available to trigger the recovery
  - Lost retransmit: The retransmitted packet is also dropped

Base on these, TCP-level solutions are proposed:

  - use NewReno to catch partial ACKs and recover from multiple loss in the same window
  - use limited transmit to ensure enough packets are sent to trigger the 3 DUPACK recovery, we can also reduce the DUPACK threshold from 3 to 1, then fast retransmit is triggered on every duplicated ACK
  - disabling exponential slow start: Because the flows are mostly short flows, using more conservative linear increase of window can alleviate the situation
  - reducing timeout from the popular value of min RTO of 200ms to typical RTT of gigabit ethernet of 100us or even 10us for Infiniband

Reducing timeout can lead to premature timeouts, which causes spurious retransmission and initiates slow-start from smallest window size, even if no packets are really lost. Moreover, small timeout needs fine-grained timer. BSD provides only coarse-grained software timer of 200ms and 500ms. Linux clock has a granularity of 1 to 10ms. Also, a small min RTO may be harmful for wide area networks as it causes premature timeout.

Ethernet flow control can also be a solution. Ethernet can issue pause frames to pause the forwarding of data for a designated period of time. But using Ethernet flow control can subject to head-of-line blocking problems.

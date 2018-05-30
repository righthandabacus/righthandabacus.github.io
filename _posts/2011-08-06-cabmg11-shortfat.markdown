---
layout: post
title: "Crisan et al (2011) Short and Fat: TCP Performance in CEE Datacenter Networks (HotI)"
date: 2011-08-06 23:16:59 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Short and Fat: TCP Performance in CEE Datacenter Networks"
  author: "Daniel Crisan and Andreea S. Anghel and Robert Birke and Cyriel Minkenberg and Mitch Gusat"
  booktitle: "Proc. Hot Interconnects"
  month: "Aug"
  year: "2011"
---

# Properties of datacenter networks

CEE (Converged Enhanced Ethernet) is designed to support up to 1M nodes in a
single L2 domain with multipathing across 10-100Gbps links. The typical RTT
ranged from 1-2us to tens of us, but upon congestion, e2e delays can reach tens
of ms. The 802.1Qbb PFC divides traffic into 8 priorities according to the IEEE
802.1p class of service field and perform 802.1x PAUSE flow control. It has
potential for deadlock in certain topologies, and exposure to saturation tree
congestion. Its configuration involves the max and min thresholds,
$$Q_{\textrm{max-th}}$$ and $$Q_{\textrm{min-th}}$$.

Datacenter communications is predominantly TCP with some RDMA, SCTP, and UDP.

# Set up

The paper compares performance of ECN/RED (RFC3168) against QCN/PFC in TCP
communication or TCP/UDP communication. The TCP used are Vegas, NewReno and
CUBIC. Vegas represents the class of delay-probing TCPs, including Compound TCP
and Adaptive Reno. NewReno is the standard as in RFC3782. CUBIC represents the
RTT-independent TCPs.

The study is using OMNet++ simulation on fat tree topology. The TCP code is
adapted from FreeBSD. Traffic pattern is according to [Benson et al
(2010)](/2011-04-26-bam10-traffic "Network Traffic Characteristics of Data
Centers in the Wild") and [Benson et al (2009)](/2011-04-16-baaz09-dctraffic
"Understanding Data Center Traffic Characteristics"), an on-off pattern with
scatter/gather communication. The scatter/gather communication arrives at the
HLA (high-level aggregators) and pass on to MLA randomly which then pass on to
servers in the same rack. Query size is 20KB with varying inter-arrival time.
The background traffic matches the inter/intra-rack traffic ratio of 3:7.
Besides scatter/gather (named "commercial traffic pattern"), this also evaluates
with scientific traffic, namely the MPI applications in NAS Parallel Benchmark:
BT, CG, FT, IS, MG and some application pattern gathered in Barcelona
Supercomputing Center.

Property of TCP in FreeBSD:

  - Connection cache: cwnd and RTT estimation are inherited from one connection to the next
  - Adaptive buffer: Rx and Tx buffers are increased in response to an RTT increase
  - Kernel quanta: 1ms in FreeBSD, but 1us is used in experiment
  - Default minRTO = 2ms, default RTO = 3s (i.e. lost SYN will drastically penalise the flow completion time), set default RTO to 10ms in experiment
  - default RTT variance = 200ms, set to 20ms in experiment

Simulation parameters:

  - TCP buffer size = 128 Kb
  - TCP max buffer size = 256 KB
  - TCP default RTO = 10 ms
  - TCP min RTO = 2 ms
  - TCP RTO variance = 20ms
  - TCP TX delay = 9.5 us
  - TCP RX delay = 24 us
  - TCP timer quanta = 1 us
  - TCP reassembly queue = 200 segment
  - ECN-RED min thresh = 25.6 KB
  - ECN-RED max thresh = 76.8 KB
  - ECN-RED Wq = 0.002
  - ECN-RED P_max = 0.02
  - QCN Qeq = 20 KB or 66 KB
  - QCN Wd = 2
  - QCN Gd = 0.5
  - QCN CM timer = 15 ms
  - QCN sample interval = 150 KB
  - QCN byte count limit = 150 KB
  - QCN fast recovery thresh = 5
  - QCN min rate = 100 Kb/s
  - QCN active increase = 5 Mb/s
  - QCN hyperactive increase = 50 Mb/s
  - QCN min decrease factor = 0.5
  - QCN extra fast recovery = enabled
  - PFC min thresh = 80 KB
  - PFC max thresh = 97 KB
  - Network link speed = 10 Gb/s
  - Network frame size = 1500 B
  - Network adaptive buffer size = 512 KB
  - Network delay = 500 ns
  - Network switch buffer size per port = 100 Kb
  - Network switch delay = 100 ns

# Result from simulation

  - Queue dynamic: without PFC = sawtooth, with PFC = step function
  - ECN-RED: Disabling PFC can lower ECN-RED performance. As queue fills, TCP will receive feedback, but too late to avoid loss. As ECN treated as loss, the duplicated ACK received later is ignored as TCP is already in recovery mode waiting for retransmission timeout. In other words, the additional feedback of ECN worsen the case
  - QCN with Qed=66KB: Unfairness causes a single flow to monopolise more than 40% capacity. The QCN per se is capable of avoiding all losses but the "winner" one
  - TCP vegas does not reveal significant impact in flow completion time but successfully limits the queue length. PFC is therefore playing a secondary role since there is no overflow
  - CUBIC w/o PFC perform worse than New Reno and Vegas due to its aggressive increase in congestion window and RTT independence generate more losses than NewReno, and penalising the completion time
  - PFC reduces flow completion time with the exception of QCN/20KB, it avoids TCP stalls waiting for retransmission.
  - DCN queue dynamic move very fast, queueing delay can increase 100-1000 fold within milliseconds. The original RTO estimator reacts too slow
  - QCN/66KB can tolerate more intrinsic burstiness of TCP (bursty data -> bursty ACK -> bursty data again)
  - ECN-RED outperforms QCN due to (1) ECN feedback is averaged whereas QCN feedback is based on instantaneous queue length; (2) RED are processed directly at transport layer; (3) RED generate feedback only for data packets, so does not delay SYN unnecessarily
  - With UDP, most drop is from UDP as TCP can retreat. And with UDP only QCN rate limiter can restore the fairness between TCP and UDP
  - Scientific workload have congestive event sustained and involve all the end-nodes, as it is a synchronised computation-communication cycle
  - Commercial workloads exhibit only sparse transient congestive events
  - WIth scientific workloads, the best performer is ECN-RED with PFC. Without PFC, QCN/20 is the best

# Conclusion

  - Delay-probing TCP (e.g. Vegas) performs the best given we can have high resolution timers
  - CUBIC TCP suffers from aggressively and slow convergence of congestion windows
  - Whether RTT independence is harmful in CEE networks with a wide dynamic range of queueing delays (sub-us to tens of ms) remains unanswered
  - PFC consistently improves the performance across all tests
  - Due to queueing-dominated RTT, L4 CUBIC and L2 QCN are suffered from their RTT independence
  - For scientific workloads, QCN 20 w/o PFC improves performance. Since HPC applications exhibit alternating phases of computation and communication; during communication, typically all nodes start communicating quasi-simultaneously. The aggressive Qed set point in QCN 20 effectively mitigates such congestive events
  - Mistuned QCN can severely degrade performance: Commercial traffic with QCN 20 w/o PFC degrades performance 131% to 311%
  - RED handles transient congestion episodes generated by commercial applications better than QCN, due to QCN's burst sensitivity; a properly configured RED is less sensitive to burstiness
  - QCN elicits further investigation in (1) lack of adaptivity and (2) fairness

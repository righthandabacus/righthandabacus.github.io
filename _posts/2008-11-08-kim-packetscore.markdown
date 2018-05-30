---
layout: post
title: "Kim, Lau, Chuah, Chao (2004) PacketScore: Statistics-based Overload Control against DDoS Attacks (INFOCOM)"
date: 2008-11-08 03:14:25 -0500
category: paper
bibtex:
  type: inproceedings
  title: "PacketScore: Statistics-based Overload Control against DDoS Attacks"
  author: "Yoohwan Kim and Wing Cheong Lau and Mooi Choo Chuah and H. Jonathan Chao"
  booktitle: "Proc INFOCOM"
  year: 2004
---
DDD: Detect for the victim, Differentiate traffic into attack and legitimate, Discard suspicious packets according to legitimate score and congestion level

The packet scoring is done by Bayesian estimation, the metric is CLP, conditional legitimate probability. Score is continuous rather than just binary accept/reject and this supports prioritisation. By combining scoring to congestion level, the acceptance of traffic by victim is adaptive to network capacity. The dynamic nature is its advantage over rule-based approach.

Traffic statistics can be collected by BFLBA: Bloom filter/Leaky bucket arrays.

Details are given in chapter 3: Certain traffic pattern are nominal to a host, espectially the composition of TCP/IP parameters such as protocols/ports. This pattern is inherently stable during normal operations. The disproportional increase in relative frequency of some attribute is a signal to attack. The legitimacy decreases as the disproportionality increases.

List of attributes includes: (1) protocol, (2) packet size, (3) server port numbers, (4) src/dst IP prefixes, (5) TTL, (6) header length, (7) TCP flags, (8) IP fragmentation bit, (9) correctness of checksums; also their joint distribution.

Let us assume attributes $$A, B, C, ...$$ are focused and we denote the joint probability mass function $$P_n(A,B,C,...)$$ for legitimate packets and $$P_a(A,B,C,...)$$ for attack packets. The $$CLP(p)$$ of a packet $$p$$ is defined as the probability that $$p$$ is legitimate given the attribute $$A,B,C,...$$

$$
\begin{aligned}
CLP(p) &= \frac{N_n JP_n(a,b,c,...)}{N_n JP_n(a,b,c,...) + N_a JP_a(a,b,c,...)} \\
&= \frac{N_n JP_n(a,b,c,...)}{N_m JP_m(a,b,c,...)}
\end{aligned}
$$

Where the terms $$N_n$$, $$N_a$$ and $$N_m$$ denotes the number of legitimate, attack and their sum respectively. Note the ratio $$N_n/N_m$$ equals to the ratio of traffic intensity between legitimate and overall traffic. The p.m.f. $$JP_n()$$ is the nominal profile. Also note that, if all attributes are independent, the formula of $$CLP(p)$$ is closed product-form. The logarithm of $$CLP(p)$$ is a series of addition, which is easy to implement in hardware.

The 3D-R routers are those residing at the perimeter and performs DDD. It manages the nominal profile for each of its egress port. It also generates 'scorebook' from sample traffic of a long enough time (compare to interarrival time of packets). The scorebook is constructed from the histogram of statistics filtered for iceberg elements. That is, small instances of attribute value is treated zero in computing $$CLP(p)$$.

After the score is computed, the 3D-R routers know about (1) the total traffic intensity to a host and (2) the score of a packet. It then finds the threshold to drop traffic to maintain the host below targetted utilization. The threshold is computed by a load-shedding algorithm that takes current and target utilization and current aggregate arrival rate as input. Afterwards, the 3D-R routers discard packets with score less than the threshold.


## Food for thought

Machine learning approach?

Independence in attribute distribution makes sense?

Load-shedding algorithm: Determines how much traffic to deliver to host. Can this be adaptive rather than formula-based? For example, adjust for request/reply ratio in addition to traffic load.

Effect of profile update frequency and the efficiency/accuracy?


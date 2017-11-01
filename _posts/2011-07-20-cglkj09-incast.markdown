---
layout: post
title: "Chen et al (2009) Understanding TCP Incast Throughput Collapse in Datacenter Networks (WREN)"
date: 2011-07-20 01:19:54 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Understanding TCP Incast Throughput Collapse in Datacenter Networks"
  author: "Yanpei Chen and Rean Griffith and Junda Liu and Randy H. Katz and Anthony D. Joseph"
  booktitle: "Proc. WREN"
  month: "Aug"
  year: "2009"
  address: "Barcelona, Spain"
---
Instead of storage networks, this paper studies the incast problem in data center running MapReduce.

In MapReduce model, the incast shall be fixed-fragment, i.e. for S senders, each request block equals to (256KB*S) bytes. The authors built a testbed using Nortel 5500 switch, connections are in single layer. The switch has a maximum of 128KB buffer per port, drawing from the pool of 768KB buffer sharing between 48 ports. Below are the findings from their experiment:

  - Randomizing the RTO timer for different senders does not help incast: Whenever the switch buffer gets full, all senders see packet drop at the same time
  - In Linux, the delayed ACK threshold is 40ms, disabling delayed ACK supposed to improve for RTO $<$ 40ms [??]
  - Using a hires RTO timer of 1ms or 200us gives the same performance as that of lores RTO of 1ms without delayed ACK, and peak performance is at RTO of 1ms with delayed ACK
  - Without delayed ACK, the TCP cwnd is over-drived, exhibits larger fluctuations and higher average, results in a more severe goodput collapse
  - Regardless of delayed ACK, the average smoothed RTT is around 2ms. When delayed ACK is off, there are more spikes in the smoothed RTT value.

A quantitative model is presented in this paper: By definition, the goodput of each sender is given by $D/(L+Rr)$ where $D$ is the total data sent, $L$ is the ideal transfer time without RTO, $r$ is the RTO value, and $R$ is the number of RTO events. The total goodput is this quantity multiply by $S$, the number of senders. According to the author's measurement, the number of RTO events following the following equation *by curve-fitting*:

$$ R=\begin{cases}3.5S & : S\le 10 \\ 35 & : S > 10\end{cases} $$

The transfer time $L$ accounts for the link bandwidth as well as the inter-packet wait time:

$$ L=\frac{D}{\textrm{bandwidth}}+\frac{D}{MSS}I $$

where MSS=1500 bytes in Ethernet and $I$ is the inter-packet wait time, which empirically found to be (in milliseconds)

$$ I=\begin{cases}4.5S & : S\le 10 \\ 4.5 & : S > 10\end{cases} $$

The equations does not fit the empirical data well, although it shows the trend. There are some refinements to explain the details that made the difference. Most important is that, upon congestion, the senders enter TCP RTO state, which expires at each sender following a distribution: $P(t)=1/T$ for $t$ in $[d, d+T]$ or 0 otherwise. The $d$ is the feedback delay, which is small in data center. $T$ is the width of the uniform distribution, as a result of the last ACK to a sender is paced out by the single output queue. The $T$ increases as the number of senders increases but upper-bounded by the finite buffer in network.

Because of that, it explains the U-shape curve for goodput against number of servers: Initial goodput drop is due to increasing RTO events. Then it increases because the increased $T$, paced out the RTO to make its impact less severe. Then the goodput decreases again because the $T$ becomes comparable to RTO. There would be interference between senders that are retransmitting and the senders that are transmitting the packets for the first time.

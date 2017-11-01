---
layout: post
title: "Alizadeh et al (2010) Data Center TCP (DCTCP) (SIGCOMM)"
date: 2012-01-28 01:12:11 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Data Center TCP (DCTCP)"
  author: "Mohammad Alizadeh and Albert Greenberg and David A. Maltz and Jitu Padhye and Parveen Patel and Balaji Prabhakar and Sudipta Sengupta and Murari Sridharan"
  booktitle: "Proc. SIGCOMM"
  month: "Aug"
  year: "2010"
  address: "New Delhi, India"
---
Title in draft: *DCTCP: Efficient Packet Transport for the Commoditized Data Center*

A paper to propose a new TCP variant, DCTCP, for use in data center.

In data center, according to the measurement of the paper, there are three types of traffic. They are:
  - Query traffic, of 2KB to 20KB in size
  - Time-sensitive short messages, of 100KB to 1MB in size
  - Throughput-sensitive long flows, of 1MB to 100Mb in size

The structure of the DCN is as follows: There are 6000 servers in a cluster, with 44 servers in a rack and 150 racks. In each rack, there is a ToR switch of 1 Gbps $\times$ 48 ports and two 10 Gbps ports for uplink. Switch buffer is 4MB, shared amongst all the ports. The application is a *soft-realtime application*, such as a web search. The application frameworks is a "partition-aggregate" type. When a query arrives, it is expected to respond in 230-300 ms, which is known as the all-up SLA. The query may create many sub-queries, typically in 1-4 iterations but may up to 20 iterations. The worker server are expected to complete the sub-query in 10-100 ms. Late responses are discarded. Therefore, to maintain response quality, 99.9% of workers are required to meet the deadline. The query comes from external, arrives at the cluster-wide high-level aggregator (HLA). The HLA partition the query and send to rack-wide mid-level aggregators (MLA). The MLA is then send to the other 43 servers in the rack. Each query from MLA is around 1.6KB in size and the response from servers to the MLA is around 1.6KB to 2KB in size.

Besides the query traffic, there are two background traffic: The update flow to copy data between worker servers. Each flow is between 5KB to 50MB in size. Also the control message to track worker states, in size of 50KB to 1MB. This control message is time-sensitive. The query traffic is approximately Poisson but the background traffic is heavy-tailed with high variance. The background traffic also occurs periodically, as the workers periodically poll for file updates.

From a server's point of view, the mean number of concurrent connection is 36 (because MLA send queries to 43 servers) but the 99.9 percentile of concurrent connections is 1600. If consider only large flows that sending more than 1 MB, the mean and 75th percentile are only 1 and 2 respectively. These large flows are those lasting for several RTTs.

The healthiness of the cluster requires (1) low latency for short flows, so that response time for query is quick; (2) high burst tolerance, to reduce loss rate; (3) high utilization for long flows, for performance reasons. To achieve these goals, we need to keep the switch buffer occupancy persistently low while maintaining high throughput for long flows.

The paper reported that, there are several impairments in the cluster:

**(1) In-cast problems for query traffic**

Default min RTO is 300ms. And the query response is only 2 packets. There is no chance for a 3 dupack and hence any loss cannot be recovered unless retransmission timeout. The in-cast problem arises when a large number of query responses are synchronized, so that a high burst overflows the shallow buffered switch. The solution proposed by the paper is to impose application-level jittering to desynchronize (with mean 10ms) the response.

**(2) Queue build up**

Even if no packet loss, if the query response has a large latency (longer than the SLA), the query is voided. This arises when a big flow used up the buffer so that the query response took a long queueing delay. It is measured that the empty-queue RTT is 100 us for intra-rack traffic and 250 us for inter-rack traffic. The measurement shows that there are 10% packets experiences 1-14ms of queueing.

**(3) Buffer pressure**

Because switch is shared-memory, if there is a big flow using a port, the buffer for another port is reduced. In this case, packet drop is easy. This creates similar problem as in-cast.

The DCTCP design is aimed to keep buffer utilization low. In a shallow buffer environment, Vegas-like delay-based method would not work because of the large noise in measurement. The paper proposed to use ECN/RED. It uses a binary deterministic marking instead of the traditional probabilistic marking of RED. A packet is marked whenever the queue length is above threshold K. Window adjustment is once per RTT. Therefore, we can convert the congestion signal into multi-bit instead of binary, by calculating the fraction of marked packets in the whole window.

The use of ECN is mainly targeting big flows: As long as the big flows' traffic uses only a small buffer, due to ECN/RED, the large buffer in switches can handle the bursty traffic due to in-cast. A majority space of the buffer is virtually reserved for the in-cast traffic.

The receiver operation is as RFC3168: It echos ECN to sender until received the confirmation from the sender by way of CWR flag. The sender operation is as follows: It keeps a EWMA of the fraction of marked packets:

$$
\alpha = (1-g)\alpha+gF 
$$

where F is the fraction of marked packets in the last window and $\alpha$ is its EWMA. The cwnd is then adjusted by

$$
\textrm{cwnd} = \textrm{cwnd}\times(1-\alpha/2) 
$$

so that the max window reduction is 0.5.

## Analysis

Assume in the incast scenario of N synchronized flows, the queue length at time t is given by

$$
Q(t)=NW(t)-C×RTT. \textrm{[Should W(t) be the time integral?]}
$$

Consider a flow whose window increases by one MSS per RTT. The number of MSS it sent when its window increased from $W_1$ to $W_2$ is given by

$$
S(W_1,W_2)=(W_2-W_1)\frac{W_1+W_2}{2}=\frac{W_2^2-W_1^2}{2}. 
$$

A switch's marking threshold is K. So the critical window size is

$$
W^{\ast} = (C×RTT+K)/N.
$$

As the window size becomes $W^{\ast}+1$, packets are being marked and the window will be decreased. Therefore, considering the whole cycle of window increase and decrease, we have the EWMA value

$$
\begin{aligned}
\alpha &=\frac{S(W^{\ast}, W^{\ast}+1)}{S((W^{\ast}+1)(1-\alpha/2),W^{\ast}+1)}  \\
&=\frac{(W^{\ast}+1)^2-(W^{\ast})^2}{(W^{\ast}+1)^2-(W^{\ast}+1)^2(1-\alpha/2)^2}  \\
&=\frac{2W^{\ast}+1}{(W^{\ast}+1)^2(\alpha-\alpha^2/4)}  \\
\alpha^2(1-\alpha/4)&=\frac{2W^{\ast}+1}{(W^{\ast}+1)^2}
\end{aligned} 
$$

For $W^{\ast} \gg 1$, we have $\alpha^2(1-\alpha/4)\approx 2/W^{\ast} $. For $\alpha\approx 0$, we have $\alpha^2\approx 2/W^{\ast} $ or $\alpha\approx\sqrt{2/W^{\ast}}$.

The amplitude of oscillation of a single flow's window is therefore

$$
D = (W^{\ast}+1)-(W^{\ast}+1)(1-\alpha/2) = \frac{\alpha}{2}(W^{\ast}+1)
$$

The amplitude of oscillation of the queue is the N times multiple:

$$
\begin{align}
A &=ND=\frac{N\alpha}{2}(W^{\ast}+1) \\
&\approx\frac{N\sqrt{2/W^{\ast}}}{2}W^{\ast} = \frac{N}{2}\sqrt{2W^{\ast}} \\
&=\frac{1}{2}\sqrt{2N(C\times RTT+K)}
\end{align}
$$

and the period of oscillation is D round trip times:

$$
\begin{align}
T_C &= D \approx \frac{\alpha}{2}W^{\ast} \approx \frac{1}{2}\sqrt{2W^{\ast}}  \\
&=\frac{1}{2}\sqrt{2(C\times RTT + K)/N}
\end{align} 
$$


Because the max window size is $W^{\ast}+1$, the max queue length is

$$
Q_\max = NW(t)-C×RTT = N(W^{\ast}+1)-C×RTT = K+N
$$

The marking threshold, i.e. the min queue length, is therefore one amplitude less:

$$
\begin{align}
Q_{\min} &= Q_{\max} - A  \\
&= K+N - \frac{1}{2}\sqrt{2N(C\times RTT+K)}
\end{align}
$$

To find the marking threshold, we minimize $Q_\min$ over all N. By differentiation we have

$$
\begin{align}
\frac{dQ_\min}{dN} & = 0  \\
1-\frac{1}{2\sqrt{2N}}\sqrt{C\times RTT+K} &=0   \\
\sqrt{C\times RTT+K} &=2\sqrt{2N}  \\
C\times RTT+K &= 8N
\end{align} 
$$

On the other hand, the min queue length have to be non-negative to prevent underflow, therefore,

$$
\begin{align}
K+N - \frac{1}{2}\sqrt{2N(C\times RTT+K)} &> 0  \\
2(K+N) &> \sqrt{2N(C\times RTT+K)}  \\
4(K^2 + 2KN+N^2) &> 2N(C\times RTT) + 2NK  \\
2K^2+2NK+2N(N-C\times RTT) &> 0  
\end{align} 
$$

This yields the threshold of K > (C×RTT)/7 [how?]

---
layout: post
title: "ACM SIGCOMM CCR 35:2 2005 Invited Editorials: Making Router Buffers Much Smaller"
date: 2009-10-05 23:01:04 -0400
category: paper
bibtex:
  type: article
  title: "Making Router Buffers Much Smaller"
  author: "Wischik and McKeon and Raina and Towsley and Enachescu"
  journal: "ACM SIGCOMM CCR"
  volume: 35
  number: 2
  year: 2005
---
This is a series of 3 papers in the July 2005 issue of SIGCOMM Computer Communication Review, on the topic of small buffer in routers.

## Wischik and McKeown (2005) Buffer Sizes for Core Routers (CCR 35:2 pp.75-78)

The first paper, "Buffer Sizes for Core Routers" by Damon Wischik and Nick
McKeown, describes that buffer is to accommodate transient bursts but it also
introduces delay and jitter. A rule of thumb in router design is to provide
buffer for a RTT's worth of data. Normal RTT is about several hundred
milliseconds but this series of paper argues that a few microsecond's worth is
enough. Several points raised: (1) core networks does not run at 100%
utilization, so keeping high utilization is not an important goal. (2) Larger
buffers do not always give higher utilization, especially in the case of
desynchronized flows.

The paper "Buffer Sizing for Congested Internet Links" in INFOCOM 2005 gives
the formula that, when the TCP flows are synchronized and the flows are in
different RTTs, then the buffer size needed to prevent link idle is $C$ times
the harmonic mean of those RTTs, where $C$ is the link speed. This paper
claims that, when the flows are desynchronized, the total window sizes of the
flows can be approximated by a normal distribution
$W\sim\mathrm{Normal}(Nw,N\sigma^2_w)$, where $N$ is the number of
flow, $w$ is the average window size of a flow, and $\sigma^2_w$
is its variance. It turns out that, $w$ varies between 2/3 to 4/3 of its means
and its variance is $\sigma^2_w=\frac{(4w/3-2w/3)^2}{12}=\alpha w$
and $\alpha\approx 0.192$. Since window size over RTT, $\tau$, is the data rate, assume the
aggregate traffic sends at line speed, we have
$\sigma^2_W=N\sigma^2_w=(\alpha C \tau)^2/N$, and the 99.9% CI for
$W$ is $C\tau\pm 3.3\alpha\frac{C\tau}{\sqrt{N}}$. This means to
accommodate 0.1% loss rate, the buffer needed is
$3.3\alpha\frac{C\tau}{\sqrt{N}}$, if the TCP flows are desynchronized.
We can further reduce the buffer size if we allow the aggregate traffic send at
a rate below line speed, i.e. lower the utilization.

If the TCP flows are desynchronized, and providing a large buffer, then each
flow will have a large RTT due to queueing delay and decreases the throughput
and keep the loss probability low. This is because the TCP sending rate
satisfies $x = \frac{h}{\tau\sqrt{p}}$ and the aggregate rate satisfies
$(1-p)\sum x = C$. While this works, the large RTT can cause damage to
real time traffic.

## Raina, Towsley and Wischik (2005) Control Theory for Buffer Sizing (CCR 35:2 pp.79-82)

The second paper, "Control Theory for Buffer Sizing" by Gaurav Raina, Don Towsley and Damon Wischik, did a deeper analysis.

It points out that, stability of TCP flow actually means desynchronization. If
TCP flows are desynchronized, the fluctuation is not drastic and thus the
dynamic behaviour is stable. Using a control theory approach, it first models
the TCP window size w(t) as a differential equation:  
$\dfrac{dw(t)}{dt}=\dfrac{1}{\tau}-\frac{w(t)}{2}[x(t-\tau)p(t-\tau)]$  
Then, in case of large buffer with AQM, the loss probability is modeled as a
function of queue length $L_{AQM}(q)$ and the total rate of packet arrival at
queue is modeled as y(t). The following equation is derived:  

$$
\begin{aligned}
\dfrac{dq(t)}{dt} &= [y(t)(1-p(t))-C]^+ \\
p(t) &= L_{AQM}(q(t))
\end{aligned}
$$

In case of small buffer of size $B$ with droptail service, the loss probability
is $p(t)=L_B(y(t))$ and $L_B(y) \approx (1-C/y)^+$. In other words, the queue
size fluctuates quickly and TCP cannot control its size, but TCP can control
its distribution. Given we have the equations, we can solve them numerically or
analytically with initial conditions specified. Detail derivation is in a
Cambridge TechReport "Control Theory and Instability Analysis of TCP" by Raina.

The modeling tells that, when buffer is small, the sending rate has small
oscillations but when buffer is large, it has larger oscillations. Small buffer
leads to small queue and faster fluctuation which is less likely to cause
synchronization. My thought is: Slower queue fluctuation causes sync because it
has a longer time for different flows to agree on the same network situation
and do the same reaction.

## Enachescu et al (2005) Routers with Very Small Buffers (CCR 35:2 pp.83-89)

The third paper, "Routers with Very Small Buffer" by Mihaela Enachescu, Yashar
Ganjali, Ashish Goel, Nick McKeown, and Tim Roughgarden, did simulation with
ns2 to study on small buffer routers.

The major idea concluded is that, small buffer is viable given that we allow to
limit the utilization below link speed. It is found that the achieved
utilization is independent of the level of congestion. Moreover, the key for
small buffer to be success is to limit bursts of TCP packets. This can be
achieved by (1) slower access link compared to core network's links and (2) TCP
pacing. By doing so, the arrival pattern of packets to the core switches is
close to Poisson and the "averaging out" effect can happen.

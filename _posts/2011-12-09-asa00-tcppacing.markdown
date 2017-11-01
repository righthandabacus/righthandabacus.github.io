---
layout: post
title: "Aggarwal et al (2000) Understanding the Performance of TCP Pacing (INFOCOM)"
date: 2011-12-09 01:26:27 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Understanding the Performance of TCP Pacing"
  author: "Amit Aggarwal and Stefan Savage and Thomas Anderson"
  booktitle: "Proc. INFOCOM"
  pages: "1157--1165"
  month: "Mar"
  year: "2000"
---
A paper to investigate the pros and cons of TCP pacing.

A non-pacing TCP sends bursty traffic. The reasons for the burstiness are as follows:

  - During slow start, two packets are sent for every ACK received. The ACK are sent by the data
    receiver bottleneck rate, consecutively. Thus the data packets will be sent in double of the
    bottleneck rate.
      - If router buffer size is much less than the BDP, the sender will encounter a loss too early and
        fall out of slow start
  - Upon loss recovery, when a packet's arrival fills a hole in the receive buffer, the ACK sent will trigger
    more than one packet to be sent, which is a burst.
  - If the ACK is sent under a clocking, the ACKs are paced out. But on a FIFO router, the ACK will be
    queued at the buffer and by the time they are delivered, the pacing between ACKs could be
    compressed.
  - Multiplexing at the router can cause the data packets be clustered together (similar reason as
    ACK compression)

The paper evaluates the effect of TCP pacing using simulation. The pacing is done by implementing a timer that fires at the rate RTT/cwnd. Data could be sent only upon the timer fires, so a leaky bucket is implemented thereof.

The findings are as follows.

When there is only a single flow, a non-paced TCP could encounter loss earlier. For a TCP undergoing slow-start, the first loss occurs when cwnd reaches twice of the bottleneck buffer size. A paced TCP does not depend on the buffer size, but on the rate. It has loss only when it can saturate the bottleneck link. This remains true for small-buffer environments. If the bottleneck buffer is larger than the BDP, pacing will perform slightly worse. This is because the feedback loop of paced TCP lags behind for one RTT.

When multiple flows sharing a bottleneck, paced TCP may perform worse. This is due to synchronization effect of pacing. Paced TCP will make everyone increase the rate at the same time, and when the bottleneck is congested, everyone is at a high rate, thus everyone will see a packet drop. This makes every TCP slow down and the network become underutilized. This is an effect of paced TCP see the congestion too late. For Reno-type TCP without pacing, the arrival is in bursts. Thus some flows will overflow the buffer and back-off, but the flow that arrive later may not cause overflow. Thus the flow that backed off make room for other flows to ramp up. In other words, non-paced TCP maintain higher bottleneck utilization by sacrificing fairness. Among those non-paced TCP, some see multiple losses while others has none.

The underutilization of paced TCP can be lifted when those TCP flows are in different RTT. In this case, they are no longer synchronized, thus synchronized back-off does not occur.

The performance difference between paced TCP and non-paced TCP depends on the flow size. For short-lived flows, they finish before congestion occurs. Paced TCP will have slightly lower performance due to delayed response. When small buffer is used, and non-paced TCP just overflow the buffer, the paced TCP still have room, thus perform better. But as the flow size increases, the paced TCP will show synchronized loss, and perform worse than non-paced. If the flow size is large enough, TCP of both type will look similar, as the synchronized loss becomes insignificant.

If the network is mixed with paced and non-paced TCP, the paced TCP will see higher probability of packet loss, since every packet has uniformly random probability of loss. But non-paced TCP sends in a burst. With a large burst, the average packet loss rate is lower, although each individual packet sees different probability.

## Further reading

- [18] S. Keshav. A Control Theoretic Approach to Flow Control. In Proceedings of the ACM SIGCOMM ’91 Conference on Communications Architectures and Protocols, September 1991.  
- [24] P. P. Mishra and H. R. Kanakia. A hop-by-hop rate-based congestion control scheme. In Proceedings of the ACM SIGCOMM ’92 Conference on Communications Architectures and Protocols, August 1992.

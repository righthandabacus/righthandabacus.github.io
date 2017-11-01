---
layout: post
title: "Prakash et al (2011) The TCP Outcast Problem: Exposing Unfairness in Data Center Networks"
date: 2011-12-19 23:25:11 -0500
category: paper
bibtex:
  type: techreport
  title: "The TCP Outcast Problem: Exposing Unfairness in Data Center Networks"
  author: "Pawan Prakash and Advait Dixit and Y. Charlie Hu and Ramana Kompella"
  number: "CSD TR #11-019"
  institution: "Purdue University"
  month: "Oct"
  year: 2011
---
This paper describes TCP outcast problem, which arises in the following setting:

In a many-to-one communication, flows from sender arrives at different input port of the bottleneck switch competing for the same output port buffer space. Then there would be "port blackout", i.e. arrival from one of the input port lost severely. The port blackout cause uneven packet loss rate between input ports, and creates unfairness issue between different flow's throughput.

The root cause of port blackout is the synchronized arrival of packets. When arrival is regular and deterministic, and the buffer running drop-tail discipline is full, the first arriving packet will be dropped due to exhausted buffer. The next arriving packet, supposedly from another port, can join the buffer as after some time, the buffer depleted one packet. Then the next one, coming from the first input port, will see the exhausted buffer again. As this regular pattern proceeds, the first input port has a blackout.

The paper found that, introducing RED to buffer management solves the blackout problem. This, however, keeps RTT bias on throughput fairness, i.e. throughput is inversely proportional to RTT. Stochastic Fair Queueing (SFQ) may be used if per-flow throughput fairness is needed without the RTT bias.

TCP pacing, however, cannot solve the blackout problem. This is because in extreme cases, pacing has no effect, although it means to space out arrivals. Consider the case described above, the arrival pattern is exactly paced out already.

The theme of this paper is to promote equal-length routing in regular multi-root network. Simply speaking, all route is of equal length (i.e. must pass through a core switch) instead of performing shortest-path routing. The authors claim that throughput unfairness can be avoided in this way because packets from different flows (note, not about input ports here) will have equal loss probability.

Further reading:

  - [1] A. Aggarwal, S. Savage, and T. E. Anderson. Understanding the Performance of TCP Pacing. In INFOCOM, 2000.
  - [11] A. Ford, C. Raiciu, and M. Handley. TCP extensions for multipath operation with multiple addresses. Internet-draft, IETF, Oct. 2009.
  - [12] S. Gamage, A. Kangarlou, R. R. Kompella, and D. Xu. Opportunistic Flooding to Improve TCP Transmit Performance in Virtualized Clouds. In Proceedings of the 2nd ACM Symposium on Cloud Computing (SOCC’11), 2011.
  - [15] A. Kangarlou, S. Gamage, R. R. Kompella, and D. Xu. vSnoop: Improving TCP Throughput in Virtualized Environments via Acknowledgement Offload. In Proceedings of the ACM/IEEE International Conference for High Performance Computing, Networking, Storage and Analysis, SC ’10, 2010.
  - [16] G. Marfia, C. Palazzi, G. Pau, M. Gerla, M. Y. Sanadidi, and M. Roccetti. TCP-Libra: Exploring RTT Fairness for TCP. Technical report, UCLA Computer Science Department, 2005.


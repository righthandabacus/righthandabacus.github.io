---
layout: post
title: "Lee et al (2002) Improving TCP Performance in Multipath Packet Forwarding Networks (JCN)"
date: 2009-10-16 16:00:12 -0400
category: paper
bibtex:
  type: article
  title: "Improving TCP Performance in Multipath Packet Forwarding Networks"
  author: "Youngseok Lee and Ilkyu Park and Yanghee Choi"
  journal: "J. Comm. and Networks"
  volume: 4
  number: 2
  pages: "148--157"
  month: June
  year: 2002
---
This paper deals with TCP over multipath networks. It suggest to modify TCP to enhance its performance over multipath, namely, increase the dupthresh to $D(1+\lfloor\log_2N\rfloor)$ where $D$ is the number (=3) of dup ACKs to trigger fast retransmission by default. Furthermore, the receiver delay ACK the packets out-of-order by ACK only every two segments or after a timer expiration but ACK immediately for every retransmitted packets.

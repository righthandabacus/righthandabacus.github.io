---
layout: post
title: "Ma and Leung (2004) Improving TCP Reordering Robustness in Multipath Networks (LCN)"
date: 2009-10-15 03:48:55 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Improving TCP Reordering Robustness in Multipath Networks"
  author: "Changming Ma and Ka-Cheong Leung"
  howpublished: LCN
  booktitle: "Proc. 29th Annual IEEE Int. Conf. on Local Computer Networks"
  year: 2004
---

Extended paper:
Changming Ma and Ka-Cheong Leung, "Improving TCP Robustness under Reordering Network Environment", in Proc. GLOBECOM 2004

Paper of similar idea:
Ka-Cheong Leung and Changming Ma, "Enhancing TCP Performance to Persistent Packet Reordering", in Journal of Communications and Networks, Vol.7 No.3 pp.385-393, September 2005

A short paper that proposed the idea of resetting the dupthresh to a higher value to prevent mistaking packet out of order as loss in TCP. The paper assumes a multipath network so that all packets are routed in one of the many different paths and therefore packet out of order is a common scenario.

The paper assumes the use of Duplicated Selective ACK (DSACK) to detect false retransmissions and then base on this information, calculate the average and mean deviation of the number of reordered packets. The dupthresh is then set to the sum of the average and the mean deviation. Upon retransmission timeout, dupthresh is decreased by multiplying a factor in $$(0,1)$$ to the average and mean deviation.

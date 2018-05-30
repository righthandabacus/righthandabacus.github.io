---
layout: post
title: "Blanton and Allman (2002) On Making TCP More Robust to Packet Reordering (CCR)"
date: 2009-10-14 16:23:46 -0400
category: paper
bibtex:
  type: article
  title: "On Making TCP More Robust to Packet Reordering"
  author: "Ethan Blanton and Mark Allman"
  journal: "ACM SIGCOMM Computer Communication Review"
  volume: 32
  number: 1
  pages: "20--30"
  month: Jan
  year: 2002
---
Packet reordering is a negative effect to TCP performance because:

  - Causing spurious retransmit
  - The sender perceiving reordering as loss and reduces its rate
  - Interrupts TCP's ACK clock and cause the transmission to be bursty
  - Probihits TCP from sampling RTT

The paper did simulation to evaluate the effect of reordering to TCP throughput. Packet reordering is done in ns2 by random queue swaps, i.e. swaping random packets on a queue. The paper lists several proposal to detect spurious retransmissions:

  - Eifel algorithm that uses TCP timestamp option to detect. It is
    robust up to one `cwnd`'s worth of lost acknowledgements
  - DSACK option
  - Check if an ACK is returned in less than 3/4 of the min RTT
    observed so far. ACK return in such a short time is claimed
    as a spurious retransmission

The paper studies the impact of reordering. It found that, the throughput decrease will level off when the number of reordering in a single incident is large enough. The paper proposes to "undo" the effect of congestion control decision due to spurious retransmissions. Before a `cwnd` decrease, record its value in `cwnd_prev` and once it is confirmed that the retransmission is spurious, set the `ssthresh` to `cwnd_prev` so that the `cwnd` can climb back to the original size exponentially.

Furthermore, we can avoid making spurious retransmission by adjusting the threshold for duplicated acknowledgement reaction (`dupthresh`). Instead of the default of 3, this value can be varying. Retransmission occurs only when the total duplicated ACK equals to dupthresh. There are several ways to adjust `dupthresh`:

  - Constant increase by 1 for every spurious retx
  - Constant increase by $$N$$, the number of packets reordered, for every spurious retx
  - Set `dupthresh` to a EWMA of number of packets reordered
  - Delay the retx by a timer, if packets arrived before timer fires, do no retx

In addition to adjusting `dupthresh`, the paper proposes to use limited transmit together with dupthresh to prevent burst, i.e. send a new packet for every other ACK so that when the spurious retransmission is found, we do not send a large burst to the network.

The simulation found that, EWMA perform the worst but other way of adjusting `dupthresh` are similar.


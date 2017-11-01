---
layout: post
title: "Vasudevan et al (2009) A (In)Cast of Thousands: Scaling Datacenter TCP to Kiloservers and Gigabits (CMU TR)"
date: 2011-07-08 16:50:44 -0400
category: paper
bibtex:
  type: techreport
  title: "A (In)Cast of Thousands: Scaling Datacenter TCP to Kiloservers and Gigabits"
  author: "Vijay Vasudevan and Amar Phanishayee and Hiral Shah and Elie Krevat and David G. Andersen and Gregory R. Ganger and Garth A. Gibson"
  institution: CMU
  number: CMU-PDL-09-101
  month: February
  year: 2009
---
A tech report to study the incast behaviour in data center. Incast is due to
RTO in synchronized read. To solve the problem, set the RTOmin to 200us can
help. The default value of 200ms gives a poor goodput.

The Jacobson formula is RTO=SRTT + 4×RTTVAR, and subject to a RTTmin. In data
center, the RTTmin is too large. If we disregards RTTmin, we have more spurious
retransmissions, as confirmed by years of study on the effect of RTTmin. But
with F-RTO, for example, which halves the cwnd but stay in congestion avoidance
to skip the slow start when spurious retransmission is detected, the effect of
spurious retransmission is less harmful than 10 years ago.

On the other hand, a too-short RTO may experience unnecessary timeout if the
other side has delayed ACK enabled, which usually delay the ACK to the second
incoming packet or for 40ms.

This tech report found that, in a SAN, the RTT distribution has a mode at
400us, in the range of 100-700us. Setting RTTmin to 200us can effectively
eliminate incast. It also found that, by disabling delayed ACK, the performance
can be better, as the retransmission timeout caused by small RTT with delayed
ACK makes the window goes through slow start again.

It also proposed to randomize RTO to desynchronizes the retransmission.

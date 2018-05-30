---
layout: post
title: "Feng and Xu (2008) TCP-Friendly CBR-Like Rate Control (ICNP)"
date: 2008-10-29 20:08:08 -0400
category: paper
bibtex:
  type: inproceedings
  title: "TCP-Friendly CBR-Like Rate Control"
  author: "Feng and Xu"
  booktitle: "Proc ICNP"
  year: 2008
---
Extend of my previous work. Consider *stochastically TCP-friendliness* that defined as:

$$ X^\textrm{TCP} \le_U X^\pi \textrm{, if } E[u(X^\textrm{TCP})]\le E[u(X^\pi)] \forall u\in U $$

where $$\pi$$ denotes the control for UDP traffic and $$X$$ are the rates of a set
of sessions; $$U$$ is a set of utility function and the comparison $$\le$$ is
component-wise to the vector $$X$$.

If $$U$$ is defined as the set of all increasing functions ($$U=st$$), then the
stochastic ordering $$X^\textrm{TCP} \le_{st} X^\pi$$ is the usual stochastic
order, which means

$$ X^\textrm{TCP} \le_{st} X^\pi \Leftrightarrow Pr[X^\textrm{TCP} \le x] \ge Pr[X^\pi \le x] $$

The protocol in this paper, TCP-Friendly CBR-Like Rate Control or TFCBR, is not
to apply admission control but to smooth the rate of sending to a longer time
scale, say, minutes. There are two parameters in this scheme, namely, $$\alpha$$
the fraction of TCP rate to send by the TFCBR; and $$\beta$$ the time constant.
The protocol checks TCP rate for previous $$\beta$$ seconds using equation and
adapt to the $$\alpha$$ of that rate. Thus it is like a moving average and it
claims that, by appropriately adjusting the parameters, it can be stochastically
TCP-friendly.

The paper also points out, $$\alpha$$ cannot be greater than 1 or otherwise it is
not TCP-friendly at all.

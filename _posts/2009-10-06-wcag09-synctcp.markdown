---
layout: post
title: "Wu et al (2009) Sync-TCP: A New Approach to High Speed Congestion Control (ICNP)"
date: 2009-10-06 14:58:11 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Sync-TCP: A New Approach to High Speed Congestion Control"
  author: "Xiuchao Wu, Mun Choon Chan, Akkihebbal L. Ananda, and Chetan Ganjihal"
  booktitle: "Proc ICNP"
  affiliation: NUS
  year: 2009
---
In high speed network, this paper argues that having TCP flows synchronized is good for fairness.

Legacy TCP cannot send data fast enough and the TCP with high speed congestion
control are proposed to utilize the abundant bandwidth. In this paper, two HSCC
TCP are studied, the compound TCP and the CUBIC TCP.

Compound TCP has two CC algorithms running concurrently: the AIMD and a
delay-based HSCC. The former gives a value of cwnd and the latter gives a value
of dwnd. The actual window used by CTCP is win=cwnd+dwnd. The dwnd is governed
by the following set of equations:

$$
\begin{align}
\Delta &= \dfrac{win}{srtt}(srtt-brtt) \\
dwnd_{i+1} &= \begin{cases}
  dwnd_i+(\alpha*win_i|k-1)^+ & \Delta<\gamma \\
  (dwnd-\zeta*\Delta)^+ & \Delta\ge\gamma \\
  (win_i*(1-\beta)-cwnd/2)^+ & \textrm{loss}
\end{cases}
\end{align}
$$

The first equation computes the actual sending rate times the queueing delay (=
backlog?) and the second equation adjusts the dwnd according to its value. CTCP
increases the window similar to HSTCP. The behaviour of CTCP is similar to
MIAD. The number of packets that CTCP tries to maintain in the queue fluctuates
between $\phi$ and $\gamma$, where $\phi$ is determined by $\zeta$.

CUBIC is aimed to improve RTT fairness. The window size is governed by an equation:  

$$ W=C(t-\sqrt[3]{W_{\max}*\beta/C})^3+W_{\max} $$

Thus W increases in decreasing step size to $W_{\max}$. But experimental
evaluation identified CUBIC has slow convergence and prolonged unfairness.

The paper did an experiment with CUBIC and CTCP and found that, when mixed with
legacy TCP and UDP (VoIP) flows, those HSCC TCP flows are too aggressive, which
reduces the throughput of legacy flows and impose long delay. Sync-TCP, which
proposed in this paper, is found to provide best performance to the legacy
flows.

The design of Sync-TCP are as follows:

  - RTT is sampled once per $T_{\textrm{sample}}$ rather than every ACK, so that
    RTT noise caused by delayed ACK is avoided
  - Pacing: $T_{\textrm{pace}} < T_{\textrm{sample}} < \textrm{RTT}$
  - srtt is updated as $ srtt_{i+1}=(1-\dfrac{T_{sample}}{T_{win}}) srtt_i + \dfrac{T_{sample}}{T_{win}} rtt_i, $
    where $T_{\textrm{win}}$ is a global constant approximately equals to the average RTT.
    The min RTT ever seen is called the $brtt$ and the min RTT ever seen since last
    cwnd reduction is $brtt_{\textrm{epoch}}$.
  - Sync-TCP calculates the queueing delay, $qd=srtt-brtt$, and a congestion is
    defined as the case that qd greater than certain threshold $Th_{qd}$
  - When congestion is detected, cwnd is freezed for $T_{\textrm{wait}}$ before it is
    reduced so that other flows can learn about the same congestion in such
    period. Similarly, when cwnd has to be increased after the reduction, it also
    wait for $T_{\textrm{wait}}$ to let the bottleneck to empty the backlog.
  - When cwnd is to be reduced, the decrease factor is $\beta = 1-\dfrac{\lambda*qd}{srtt}$
    and it is bounded in $[0.125,0.95]$. The constant $\lambda$ is adjusted so
    that the queue can be emptied.
  - If no congestion is detected, cwnd increases by $\alpha$ every $T_{\textrm{win}}$, a
    fixed length period. This makes all flows increase their rate by the same
    amount to maintain fairness. The increase factor is determined by
    $\alpha=\max(1,(1+t+\dfrac{t|4}{32}*\dfrac{Th_{qd}-qd}{Th_{qd}})$, where $t$ is
    the time elapsed since cwnd is begin to increase. Thus the increase was slow at
    the beginning and fast later on when the high-order term dominates. The faction
    of $Th_{qd}$ is to slow down the increase as the queueing delay builds up.

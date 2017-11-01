---
layout: post
title: "Estan & Varghese (2002) New Directions in Traffic Measurement and Accounting (SIGCOMM and UCSD CD TR 699)"
date: 2008-11-13 16:24:27 -0500
category: paper
bibtex:
  type: techreport
  title: "New Directions in Traffic Measurement and Accounting"
  author: "Cristian Estan and George Varghese"
  institution: "UCSD"
  howpublished: "SIGCOMM'02"
  number: "CD TR 699"
  year: 2002
---
Generalised flow: Identified by a function of the header field values, so that it is not constrained to 5-tuples.

Large flow: Those that send more than a given threshold during a given measurement interval.

Goal: Report the large flows with their flow ID and sizes at the end of measurement. Because, say, a network of bandwidth $C$ can have 10 flows at threshold $0.1C$, thus the theoretical minimum amount of memory required is inversely proportional to the threshold, i.e. $O(1/\theta)$.

To measure an algorithm, we have four metrics: (1) the memory consumption, (2) false positives, (3) false negatives, (4) error in traffic intensity estimates.

Chapter 3 (Solution): Assume the processor is fast enough to perform some processing (look up and modify counter) per packet arrival, but the memory is limited. If we record a new flow on memory by expelling the smallest flow (even with LRU policy), we may miss a large flow due to its small count at an instant.
Random sampling can avoid this problem but the error is high (proportional to $1/\sqrt{M}$ where $M$ is the amount of memory).

# Sample and Hold

Sample and hold algorithm is proposed in this paper, it claims the error proportional to $1/M$. It sample every packet with a probability and create an entry in the counter. After an entry is created, *every* subsequent packet of the same flow is counted.

Analysis of the sample and hold is as follows: Assume we want to find the flows consumed more than a faction $1/s$ of capacity $C$. Theoretically, there are at most $s$ such flows. Now we over-sample by a factor of $t$, so we keep st counters. We take the sampling probability (per byte) to be $p=st/C$.

For a flow sending more than $C/s$ bytes, the probability that such flow is never sampled has the probability less than $(1-\frac{st}{C})^{C/s} \approx e^{-t}$. Using similar logic, we can also see that the flow is counted before a fraction $p$ of the total traffic sent is $1-e^{-pt}$. Thus the probability of having a relative error of $p$ in underestimating the traffic load is given by $1-e^{-pt}$.

# Multistage filter

Using parallel hash functions to hash the packets' flow ID and count for the traffic intensities, it can reduce the false positives exponentially because a heavy flow is identified only if all the hash counters report heavy.

# Tuning of Algorithm

Given that heavy flows are identified at certain accuracy, the result can be refined by certain 'tunes'.

Firstly, because heavy flows are likely to stay longer than small flows, we are easier to confirm them by comparing the identified flows across sampling cycles. We can preserve both heavy flow entries and newly added flow entries in a cycle to the next sampling cycle but reset the count. In other words, small flows in two consecutive cycles are confirmed as small.

Secondly, besides the preservation above, we can see it suitable to remove the extremely small flows before proceeding to the next cycle even they are added in the current cycle because they are very unlikely to be heavy. To do this, we may set another threshold that the flows smaller than this are removed.

Thirdly, if we count on the hashed flow ID, we can reduce the false positives by shielding the already-known heavy flows. Thus given that a heavy flow is known, it is not hashed and not counted (at the hash counter). Thus the counter can tell whether there is another flow of the same hashed value and whether it is heavy.

Fourthly, perform conservative update on counters of multistage filters. Instead of adding the byte count to all hashed values, the lowest counter is added the number of bytes whereas the other counters only add up to the new value of the lowest counter. If the other counter is already at a higher value than the new value, its count doesn't change.

Finally, no change of counter in the multistage filter if it already found in the flow memory. This can avoid unnecessary increase in the counter.

Chapter 4 (Evaluation): Consider the sample and hold here. Assume the sampling probability is $p$ and a flow has size $T$. The probability that the flow will not be sampled is $(1-p)^T$. As discussed previously, if $T$ equals to the threshold $s$, such probability is equal to $\exp(-t)$ where $t$ is the oversampling ratio. For a flow of certain size, it get sampled on byte $x$ has the probability $(1-p)^x p$, which $E[x]=1/p$ and $\sigma_x=\frac{\sqrt{1-p}}{p}$. So if the counter tells the flow is of size $c$, the best real size estimate shall be $c+1/p$.

There is also a calculation about the memory required for sample and hold. The total number of bytes sampled would be $n=pCT$ where p is the sampling probability, $C$ is the link capacity and $T$ is the sampling period. If there are $m$ flows, and the bytes are uniformly random to belong to any flow, the probability of a flow did not sampled is $p=(1-1/n)^m$ and the probability of $k$ flows being sampled is then $mCk (1-p)^k p^{(m-k)}$, which is a binomial distribution. By approximating the binomial distribution with normal distribution, we can have an equation about the number of flow counters needed depending on the overflow probability.

So what if the 'uniform randomness' is not a valid assumption? Since we know, the flow size distribution is more like Zipf or Pareto.

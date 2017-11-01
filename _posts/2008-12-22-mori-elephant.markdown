---
layout: post
title: "Mori et al (2004) Identifying Elephant Flows Through Periodically Sampled Packets (IMC'04)"
date: 2008-12-22 15:05:25 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Identifying Elephant Flows Through Periodically Sampled Packets"
  author: "Tatsuya Mori and Masato Uchida and Pyoichi Kawahara and Jianping Pan and Shigeki Goto"
  booktitle: "Proc IMC'04"
  year: 2004
---
Attacking the elephant and mice phenomenon (the vital few and trivial many rule) in a high speed Internet, try to determine the elephant flows. The elephant flows in this paper is defined as those who consumes more than 0.1% of link bandwidth.

Since the network is high speed, processing every packet is not possible. Sampling is required but it may affect the accuracy of identifying elephants. This paper studies the trade-off.

Let there are $N$ packets traversing the network in a unit time and we sample $n$ of them, so that the sampling frequency is $f=n/N$. Assume for a flow $j$, there are $X_j$ packets and we sampled $Y_j$. The distribution of $Y_j$ is hyper-geometric:  

$$ \Pr[Y_j=y|X_j=x]=\binom{y}{x}\binom{N-x}{n-y}/\binom{N}{n} $$

and we can use Bayesian Theorem to obtain:  

$$ \Pr[X_j\ge x|Y_j\ge y]=\frac{\Pr[Y_j\ge y|X_j\ge x]\Pr[X_j\ge x]}{\Pr[Y_j\ge y]}=\frac{\Pr[Y_j\ge y|X_j\ge x]\Pr[X_j\ge x]}{\sum_k \Pr[Y_j\ge y|X_j=k]\Pr[X_j=k]} $$

Because of the Bayesian Theorem and sampling, there are false positive and false negatives. Experiment tells that, for a small $f$ (~1/10000), the false positive rate is very small (~5%) while the false negative rate could be quite high (~77%).

The above method requires the knowledge of a priori distribution of flow size, which can be determined by previous stage measurement (empirical distribution) or assuming Pareto (theoretical distribution).

To read:

  - Space-code Bloom filter (SCBF) for extracting per-flow statics of traffic in high speed networks. (INFOCOM 2004 paper by Kumar, Xu, Wang, Spatschek, Li)
  - PMA project of NLANR provides packet trace data for experiment

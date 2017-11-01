---
layout: post
title: "Piatek et al (2007) Do incentives build robustness in BitTorrent? (NSDI'07)"
date: 2008-11-24 11:17:21 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Do incentives build robustness in BitTorrent?"
  author: "Michael Piatek and Tomas Isdal and Thomas Anderson and Arvind Krishnamurthy and Arun Venkataramani"
  howpublished: "NSDI'07"
  booktitle: "Proc. Network Systems Design and Implementation"
  year: 2007
---
  - Free riders is a problem of P2P.
  - In BT, peer list are unstructured and random, nothing to do for new
    arrival or departure of peers.
  - The size of the active swarms is proportional to square root of
    upload capacity.

Analysis of the reference implementation of BT: Optimistic unchoking explores 2 peers every 30 sec and also expected to be explored by 2 peers every 30 sec. The time to find a better peer is determined by two factors: the convergence time of the peer's upload speed become steady and the size of active set. If a peer has high upload capacity, it appears to have a low upload rate when it is not yet reaching the steady state. Thus it will be peered with low capacity peers.

Reciprocation between two peers, P and Q, are determined by the rate P sends to Q and the rates other peers sends to Q. Q will pick only the fastest peers every 10-sec round. If a peer P uploads at certain equal split rate, the reciprocation is highly assured and the even higher upload rate is altruistic.

[more analysis in the later part of section 3]

The analysis suggested that the performance for low capacity peers is disproportionately high, which is unfair to the high capacity peers and the high capacity peers can exploit this by masquerading as multiple low capacity peers.  BitTyrant is doing the other way to improve performance: (1) reciprocation base on the peer's reciprocation bandwidth rather than the upload speed; (2) maximise the reciprocating peers until the marginal benefit of additional peer is outweighed by the cost of reduced reciprocation probability from other peers; (3) do not use equal split, instead, reduce the upload rate to a peer as long as it still reciprocate, so that saved bandwidth can use to connect more peers.

Active set size is a trade-off. A large active set can increase the chance of reciprocation on one hand and decrease the equal split size on the other hand. Fig 8 of the paper shows the effect.

BitTyrant resizes its active set and varies the sending rate per connection: it maintains the estimates of the download/upload ratio of a peer and rank the peers by this ratio. It unchokes the peers until the total upload estimates exceeds its upload capacity. This maximises the amount of download with least amount of upload.

The upload rate is determined as follows: A BitTyrant client uploads at equal split initially and then decreases the upload rate by 10% if the reciprocation lasts for 3 rounds. If it fails to reciprocate after unchoking in the last round, increases the upload rate by 20%. 

The reciprocation bandwidth is estimated by measuring the download speed from a peer over a longer period. In case nothing are downloaded so far, the download speed is approximated by the block announcements from that peer. BitTyrant measures the total download bandwidth from a peer and estimate the bandwidth through divides it by the estimated active set size.

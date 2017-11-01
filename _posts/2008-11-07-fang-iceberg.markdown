---
layout: post
title: "Fang et al (1999) Computing Iceberg Queries Efficiently (VLDB'98)"
date: 2008-11-07 21:32:49 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Computing Iceberg Queries Efficiently"
  author: "Min Fang and Narayanan Shivakumar and Hector Garcia-Molina and Rajeev Motwani and Jeffrey D. Ullman"
  booktitle: "Proc. Internaational Conference on Very Large Databases (VLDB'98)"
  address: "New York"
  month: August
  year: 1998
---
Find the elements in a set-with-duplicates for top-$K$ frequencies. Two approaches are proposed: sampling and coarse counting. Sampling is to take $s$ samples from a pool of $N$ and count for the frequencies in $s$. The result is then scaled by $N/s$. Afterwards, report those with scaled frequency larger than the threshold. However, this approach usually produce both false-positives and false-negatives and the elimination of them are non-trivial.

Coarse counting is use an array of $m$ counters and a hash function $h$ that maps an into into one of the $m$ possible counters. The procedure is to scan the pool for every item $v$, and increment the corresponding counter $h(v)$. Afterwards, scan the pool once again for those items with the counter $C[h(v)]$ greater than threshold and count for its frequency (individually). Note that in the second count, no false-negative can happen and after the second count, we can filter out the false-positives as well.

Chapter 4 discuss some more complex techniques. DEFER-COUNT is to firstly sample $s < n$ items and select the $f < s$ most frequent targets. Record these $f$ targets in $F$. Then do coarse counting but skip all those already in $F$. Then append the heavy items into $F$ and remove false positives by second scan. This is to push down the noise floor created by the already-known heavy targets in the coarse counting so that other unknown heavy targets can be easier to spot out. But this requires to choose the value of $s$ and $f$, as well as it splits the main memory into sample set and counting buckets.

MULTI-LEVEL is another algorithm. It takes $s$ sample and for every sample $v$, increment the count $C[h(v)]$ with hash function $h$. Afterwards, mark the $i$-th bucket as potentially heavy if $C[i]\frac{n}{s}$ greater than threshold $T$. Then for each potentially heavy bucket, allocate some auxiliary buckets. For the second scan, initialize the old counter to zero and increment as usual. But for potentially heavy ones, perform a second hash function $h_2(.)$ in addition to $h(.)$ and count using the auxiliary bucket. This algorithm does not store explicitly the potentially heavy items. Although memory is splitted between primary and auxiliary buckets.

MULTI-STAGE algorithm handles memory more efficiently. It does pre-scan just like MULTI-LEVEL but instead of allocate aux buckets for every potentially heavy buckets, it allocates a common pool for auxiliary buckets. In the scan, it increments $C[h(v)]$ for all non-heavy $v$ but increment the aux bucket $B[h_2(v)]$ instead if $v$ is heavy. Because a common pool can be larget than local pool, we are less likely to have heavy elements collide with light elements in the aux buckets. Which this intuition tells its superiority over MULTI-LEVEL.

The above are the 'hybrid' algorithms for the iceberg query. Some subtle improvement (Ch.5-6) also proposed in the paper (such as the use of multiple hash functions) and some case studies (Ch.7) are done.


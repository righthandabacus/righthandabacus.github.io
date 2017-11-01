---
layout: post
title: "Kim et al (2009) Revisiting Route Caching: The World Should Be Flat (PAM)"
date: 2010-10-06 19:42:07 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Revisiting Route Caching: The World Should Be Flat"
  author: "Changhoon Kim and Matthew Caesar and Alexandre Gerber and Jennifer Rexford" 
  howpublished: PAM
  booktitle: "Proc. 10th International Conference on Passive and Active Network Measurement"
  pages: "3--12"
  year: 2009  
---
This paper measures the effectiveness of caching in routing in Tier-1 ISP.

It takes the traffic data from (1) access router supporting 20K DSL subscribers for 8 days, resulting in 65K packets/second; and (2) in-bound traffic of 60 points-of-presents routers in the ISP for 15 hours, resulting in 100K packets/second per router. Then the data are analyzed in how effective if cache is used in routing. The route caching model is uni-class caching: Instead of caching variable-length CIDR entries doing longest prefix matching, it convert an CIDR entry into multiple fix-length prefix matching entries. Then in the cache, the fix-length prefix and the route decision is remembered.

In the network, the route size of 9M entries but it shows a very skewed access rate. For 97% of traffic, only 1/10 of route entries is used, and around 60% of entries are never accessed in the traced traffic. Moreover, the route shows a high temporal locality. The working set, defined as the number of route prefixes accessed over a fixed length of time, is roughly constant over the different time of day.

In terms of cache strategy, it is found that LRU (least-recently used) behaves better than LFU (least-frequently used) in terms of reducing cache miss rate. This is also explained by temporal locality.

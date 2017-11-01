---
layout: post
title: "Hayashi et al (2011) Improving Fairness of Quantized Congestion Notification for Data Center Ethernet Networks (ICDCS)"
date: 2011-08-09 16:54:14 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Improving Fairness of Quantized Congestion Notification for Data Center Ethernet Networks"
  author: "Yuki Hayashi and Hayato Itsumi and Miki Yamamoto"
  howpublished: "ICDCS"
  booktitle: "Proc. 31st International Conference on Distribution Computing Systems Workshops"
  pages: "20--25"
  month: "Jun"
  year: "2011"
---
There are fairness issue in QCN, which leads to unequal bandwidth allocation. The fairness problem is caused by probabilistic feedback to reduce control overhead. Excessively large or small amount of feedback to RP may cause imbalanced bandwidth consumption. Moreover, increase/decrease of rate depends on current rate. This is also a reason for unfairness.

The decrease of rate in RP is due to QCN feedback. Which undergoes the multiplicative decrease in rate. For rate increase, it is driven by a byte counter, unless the flow does not send much bytes in a period. The byte counter (BC) increase by 1 for every 150KB (value of BC_LIMIT) sent. After a rate decrease, the first five increment in BC is the fast recovery, which the current rate is approaching the target rate in a binary search fashion, i.e.

    CR = (CR + TR)/2

Afterwards, it undergoes active increase and then hyperactive increase every time the BC incremented. Which the TR is increased each time and CR still undergoes binary increase, i.e.

    TR = TR + R_AI
    CR = (CR + TR)/2

Therefore, a high rate flow increase the rate more frequently than low rate flow. And thus the unfair bandwidth distribution between these two flows maintained.

Simulation shows that, imbalanced number of feedback sent to parallel flows. A slow flow get less feedback and a fast flow get more. This will reduce the rate of a fast flow more. However, slow flow also undergoes less active increases. This is the cause for unfairness.

This paper proposes to use

    BC_LIMIT = CR * 24e-5

instead of BC_LIMIT=150KB. This makes the BC increment proportional to the current sending rate, or in other words, in equal time interval regardless of the rate. Simulation proves that this solves the unfairness problem.

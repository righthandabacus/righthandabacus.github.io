---
layout: post
title: "Dukkipati et al (2010) An Argument for Increasing TCP’s Initial Congestion Window (CCR)"
date: 2011-05-09 22:45:57 -0400
category: paper
bibtex:
  type: article
  title: "An Argument for Increasing TCP’s Initial Congestion Window"
  author: "Nandita Dukkipati and Tiziana Refice and Yuchung Cheng and Jerry Chu and Tom Herbert and Amit Agarwal and Arvind Jain and Natalia Sutin"
  journal: "ACM SIGCOMM Computer Communication Review"
  volume: 40
  number: 3
  pages: "27--33"
  month: July
  year: 2010
---
Traditional congestion window in TCP is set initially to 3 segments, approximately 4KB for 1500 byte MTU. This paper argues that, it is not optimal for modern time network for a number of reasons:

Firstly, latency can be reduced with larger initial cwnd, especially for short-lived flows that can complete its transfer within slow start. A formula from Cardwell, Savage and Anderson in 2000, "Modeling TCP Latency", shows that initial cwnd size is related to the latency in such case. Because of the latency, some browsers are using multiple concurrent connections to download a webpage (i.e. the page, and related resources such as images) to have a faster perceived rendering time. If this latency can be reduced, more advanced technology such as SPDY protocol (instead of HTTP/1.1) can be used to mitigate the scenario of too many concurrent connections.

Secondly, a short-lived flow with small cwnd can hardly compete for bandwidth with "bulk data traffic". Having a larger initial cwnd can make the short-lived flow get a fairer share. Moreover, in case of loss, a larger cwnd allows fast recovery, whereas a small cwnd usually leads to recovery by retransmission timeout.

The argument of this paper is concerned about web traffic. All the data are supported by the statistics of web applications. This is why the paper claims that a larger initial cwnd is good because the Internet is dominated by short-lived flows, the web flows.

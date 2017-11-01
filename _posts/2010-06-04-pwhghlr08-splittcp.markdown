---
layout: post
title: "Pathak et al (2010) Measuring and Optimizing TCP Splitting for Cloud Services (PAM)"
date: 2010-06-04 18:53:54 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Measuring and Optimizing TCP Splitting for Cloud Services"
  author: "Abhinav Pathak and Y. Angela Wang and Cheng Huang and Albert Greenberg and Y. Charlie Hu and Jin Li and and Keith W. Ross"
  booktitle: "Proc. Passive and Active Measurement Conf"
  howpublished: "PAM"
  year: 2010
  address: Zurich
---
Split TCP: Put a proxy in between the client and the server so that the client's request is handled by the proxy and the proxy is holding persistent connections to the server. In this case, 3-way handshake only happen between client and the proxy and the proxy can enjoy an established connection to the server with slow-start avoided. Thus the proxy-server has a short response time and high bandwidth and because the client-proxy distance is short, the client can also has a short response time and better performance.

This paper is doing empirical evaluation on the performance of split TCP, compared to that without.

---
layout: post
title: "Shor et al (2000) Application of control theory to modeling and analysis of computer systems (RESCCE)"
date: 2009-10-05 00:41:37 -0400
category: paper
bibtex:
  type: inproceedings
  title: Application of control theory to modeling and analysis of computer systems
  author: Molly H. Shor and Kang Li and Jonathan Walpole and David C. Steere and Calton Pu  
  howpublished: RESCCE
  booktitle: Proceedings of the Japan-USA-Vietnam RESCCE Workshop
  year: 2000
---

A paper tries to understand TCP as a control system (without rigorous modeling).

It says TCP is a control system with delay and the steady state behaviour is that, it increases its rate until too much and then drop rapidly and increase again. If we plot the *limit cycle* of TCP in 3D with axes of perceived RTT, allocated service rate, and cwnd size, it is a triangle with vertices at the points of (1) max cwnd size which yields max RTT, (2) min cwnd right after multiplicative decrease but the RTT is not minimal because of backlog and too low rate, (3) min RTT at middle of cwnd and middle of service rate. The limit cycle of TCP travels between these three points, while the optimal is at point 3 but the AIMD control makes TCP keep traveling.

The paper also found by experiment that, the limit cycle is an attractor, i.e. wherever the current state of TCP, it eventually goes back to the limit cycle. The limit cycle, however, is determined by other network factors.

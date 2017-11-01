---
layout: post
title: "Guo et al (2009) BCube: A High Performance, Server-centric Network Architecture for Modular Data Centers (SIGCOMM)"
date: 2011-05-30 20:06:03 -0400
category: paper
bibtex:
  type: inproceedings
  title: "BCube: A High Performance, Server-centric Network Architecture for Modular Data Centers"
  author: "Chuanxiong Guo and Guohan Lu and Dan Li and Haitao Wu and Xuan Zhang and Yunfeng Shi and Chen Tian and Yongguang Zhang and Songwu Lu"
  booktitle: "Proc. SIGCOMM'09"
  pages: "63--74"
  month: "August 17–21"
  year: 2009
  address: "Barcelona, Spain"
---
BCube is a recursive structure so that:

  - BCube($0$) is a $n$-port switch connects $n$ servers
  - BCube($k$) consists of $n$ units of BCube($k-1$) and $n^k$ units of $n$-port switch,
    in total $n^{k+1}$ servers and $(k+1)n^k$ switches

Building BCube($k$) from BCube($k-1$) are as follows: We number the BCube($k-1$) from $1$ to $n$ and in each BCube($k-1$), number the server from 1 to $n^k$. Then, considering the $n$ switches in the layer $k$, we connect the $i$-th server in $j$-th BCube($k-1$) to the $j$-th port of the $i$-th switch.

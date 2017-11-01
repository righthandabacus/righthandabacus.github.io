---
layout: post
title: "ns3: ASCII trace to stdout"
date: 2011-08-01 22:00:00 -0500
tags: code
---

The way demonstrated in examples to put ASCII trace into a file:
```c++
  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("tcp-star-server.tr"));
```
To output to stdout (or other existing std::ostream object), use the following instead:
```c++
  pointToPoint.EnableAsciiAll (Create<OutputStreamWrapper>(&std::cout));
```

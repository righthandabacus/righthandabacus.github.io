---
layout: post
title: "ns3: Memory footprint"
date: 2009-12-21 11:18:19 -0500
tags: code
---

To reduce the memory footprint of NS3, try to reduce the number of objects created. For example:

  - Avoid creating packets that are deemed to queue up for a long time, use a counter instead and create it when it is ready to send
  - Make better use of ScheduleNow calls to avoid the call stack build up


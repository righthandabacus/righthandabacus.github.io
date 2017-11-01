---
layout: post
title: "Spring et al (2000) Receiver Based Management of Low Bandwidth Access Links (INFOCOM)"
date: 2011-07-08 22:33:13 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Receiver Based Management of Low Bandwidth Access Links"
  author: "Neil T. Spring and Maureen Chesire and Mark Berryman and Vivek Sahasranaman and Thomas Anderson and Brian Bershad"
  booktitle: "Proc. INFOCOM"
  year: "2000"
---
This paper proposed to use rwnd at TCP receiver to do congestion control in case of interactive flows contending with bulk transfer flows. The paper argues that, it is a *cooperative control* for the receiver to limit congestion at access link and the sender limits congestion in the rest of the network. It promotes the practice of shrinking the receiver's buffer (rwnd) of the long-lived transfers to reduce the queueing delay of interactive applications.

The strategy is as follows:

Firstly, classify flows into four classes, in descending priority: Interactive, Short-lived bulk transfer, Long-lived bulk transfer, and Idle. The classification is initially due to port number, and subsequently by its behaviour. For example, sending 2KB of data makes an interactive flow became short-lived bulk transfer; and 8KB data make it further became long-lived. Idle flows are those idled for 30 seconds.

Then whenever a connection is established, destroyed, or reclassified, the buffer sizes are recalculated for each connection. The buffer (rwnd) allocation is as follows:

  - Idle connection has 1 MSS. When it is out of idle, we can expand its rwnd by reallocation as it goes through slow-start
  - Long bulk transfer with no higher priority flows share the buffer equally subject to the average BDP plus the network queue size. This queue size is subject to not having packet drop, but long delay is tolerated.
  - If the long bulk transfer is contending with short-lived bulk transfer, set their rwnd to one MSS and wait for the short-lived flows to finish
  - For short-lived transfers, if there are interactive flows, share the buffer of BDP plus delay-tolerable queue length among them. So that interactive flows do not see a long queue.
  - If the short-lived transfers do not have interactive flows, a longer queue is tolerated and the rwnd can set to be larger
  - The interactive flows should not be sensitive to rwnd size due to its low throughput. But for safe-guarding against any of them become bulk transfer, the rwnd should not be too big to create long queue

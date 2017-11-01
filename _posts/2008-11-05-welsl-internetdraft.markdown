---
layout: post
title: "Welzl, Papadimitriou, Scharf, Briscoe (2008) Open Research Issues in Internet Congestion Control (Internet Draft)"
date: 2008-11-05 02:02:54 -0400
category: paper
bibtex:
  type: misc
  title: Open Research Issues in Internet Congestion Control
  author: "Welzl and Papadimitriou and Scharf and Briscoe"
  howpublished: "Internet Draft"
  note: "Dated 5 Aug 2008, Expires 4 February 2009"
  year: 2008
  filename: "draft-irtf-iccrg-welzl-congestion-control-open-research-02.txt"
---
In chapter 1 of the draft, an overview of the history of congestion control research of TCP/IP is presented.

Chapter 2: 
The traditional model for congestion control (V. Jacobson) assumed things are static. However, neither the available bandwidth nor delay are static in reality. In the high BDP environment, AIMD works too conservative but the existing proposals may not be fair, or may not work well in all scenarios/dynamics.

Some fundamental facts about control theoretic congestion control: (1) Only output related information are fedback, (2) a low pass filter on feedback information is needed to get only those last long enough to affect the output, (3) control freq should equals to feedback frequency for stability reasons, thus reaction iff feedback is received.

The goal of congestion control is which of the following: TCP-friendliness? or Fairness? These two are not synonyms.

Chapter 3:
Router support on congestion control is a dilemma. Now we know that congestion control cannot be completely solved without router support. But duplication of functions at end-host and routers make things complicated and unstable. Moreover, reliance on routers may raise scalability problems.

Another problem is the way to handle corruptions. This happens especially in wireless links. While noise can cause corruption of packets, congestion can also make several packets collide.

Instead of packet congestion, we can also consider bit congestion. For example, can we react to congestion by modifying the packet size? Or, can we treat different packet sizes differently for congestion control reasons? A further question is whether we weight a loss signal according to the packet size. For example, loss of a jumbo packet should react at a greater degree. This is raised by Bob Briscoe. DCCP also studying (Congestion Control ID 4) the scenario of small packets stream.

Further questions: Can multidomain congestion control be realized? Can we implement low-precedence elastic flows?


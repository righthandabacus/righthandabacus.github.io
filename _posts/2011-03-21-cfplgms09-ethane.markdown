---
layout: post
title: "Casado et al (2009) Rethinking Enterprise Network Control (TON)"
date: 2011-03-21 19:23:56 -0400
category: paper
bibtex:
  type: article
  title: "Rethinking Enterprise Network Control"
  author: "Martín Casado and Michael J. Freedman and Justin Pettit and Jianying Luo and Natasha Gude and Nick McKeown and Scott Shenker"
  journal: "Trans on Networking"
  volume: 17
  number: 4
  pages: "1270--1283"
  month: Aug
  year: 2009
---
This is a journal version of SIGCOMM 2007 paper, that proposed the Ethane design.

Ethane is a mandatory call-setup design. No communication is allowed without explicit permission. The network consists of Ethane switches and a controller. The controller decides whether a packet is allowed to be forwarded in the network, and how. Essentially, the architecture is as follows:

  - All entities in the network (users, switches, hosts, etc) are authenticated by the controller, so that
    the controller can associate an entity with its location (ingress port of a switch), or track its movement
  - Switches are in a spanning tree rooted at the controller. There is a secure channel between each
    switch and the controller
  - When a flow enters the network the first time, it is forwarded to the controller
  - If the controller permits the flow, the packet is forwarded back to the entry switch. The intermediate
    switches along its path has the forwarding table installed for this flow. So that subsequent packets
    are forwarded according to this flow table.
  - Policy can be enforced by the controller, by setting up a path including the way point (e.g. pass
    though a middlebox)
  - Flow entry in switches expires after time or can be revoked by controller

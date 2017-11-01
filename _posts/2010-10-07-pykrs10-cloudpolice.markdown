---
layout: post
title: "Popa et al (2010) CloudPolice: Taking Access Control out of the Network (HotNets)"
date: 2010-10-07 16:16:49 -0400
category: paper
bibtex:
  type: inproceedings
  title: "CloudPolice: Taking Access Control out of the Network"
  author: "Lucian Popa and Minlan Yu and Steven Y. Ko and Sylvia Ratnasamy and Ion Stoica"
  booktitle: "Proc. HotNets"
  year: 2010
---
In a virtualized cloud environment, instead of enforcing access control inside the network using middleboxes, this paper advocates to put the access control inside the physical servers.

The access control cares about whether a VM or a flow can reach a particular destination. This is in terms of not only security, but also rate limiting and fair-sharing of resources. This paper suggests that the access control should be implemented in the hypervisors of the end hosts. Doing so can enforce the access control at the *source* so that it prevents unauthorized traffic from abusing the network.

Installing access control into hypervisors can be done in two extreme ways: use a centralized server to hold all the access control policies and the hypervisor query the server for every time; or have a full copy of the access control policies in every hypervisors. The former is easy to update but the later is faster to read. The paper suggests a blend between these two: have a partial copy in each hypervisor, only the policies that related to its hosting VMs are stored.

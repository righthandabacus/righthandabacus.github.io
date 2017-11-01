---
layout: post
title: "Al-Qudah et al (2009) Efficient Application Placement in a Dynamic Hosting Platform (WWW)"
date: 2010-06-07 01:01:26 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Efficient Application Placement in a Dynamic Hosting Platform"
  author: "Zakaria Al-Qudah and Hussein A. Alzoubi and Mark Allman and Michael Rabinovich and Vincenzo Liberatore"
  booktitle: "Proc. WWW"
  month: "Apr 20-24"
  year: 2009
  address: "Madrid, Spain"
---
[PDF](https://drive.google.com/open?id=0B6DoI_vm0OLfbHpiTmNzRFdTV2M)

This paper proposes a way to run a web-hosting cluster. Specifically, the paper claims the following can give a good performance:

  - Instead of running a web on a dedicated server, or start an application on demand, it proposes to run
    every application on every machine, but put the unused application in standby mode (i.e. suspend to
    disk)
  - To run a suspended application, OS need to load the image from disk to memory. If the OS is doing memory
    paging, that will become the bottleneck in moving applications. Doing process swapping, however, is
    easier. Thus for active tasks, memory paging is used for efficiency reason. For application placement
    events, process swapping is done.

Memory paging hurts performance because paging is on-demand. If a process is resumed, the OS need to alternate between CPU-bound and I/O-bound processes, as loading a page from disk happens iff page fault occurs. This creates much scheduling overhead. The scenario worsens if loading a page needs to free up existing memory, as more overhead is created.

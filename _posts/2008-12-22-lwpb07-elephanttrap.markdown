---
layout: post
title: "Lu et al (2007) ElephantTrap: A low cost device for identifying large flows (IEEE SHPI)"
date: 2008-12-22 18:33:08 -0500
category: paper
bibtex:
  type: inproceedings
  title: "ElephantTrap: A low cost device for identifying large flows"
  author: "Yi Lu and Mei Wang and Balaji Prabhakar and Flavio Bonomi"
  howpublished: "SHPI"
  booktitle: "Proc 15th IEEE Symposium on High-Performance Interconnects"
  year: 2007
---
An algorithm is proposed to detect elephant flows. It assumes the flow rates are under Pareto distribution and the algorithm is as follows:

  - Sample packet at probability $p$
  - For each sampled packet, if the flow already exists in the
    counter array, increment the counter
  - If the flow doesn't exists in the counter array, evict a
    counter if possible and put this flow into the counter,
    initialize to zero
  - A eviction pointer scans the counter array, whenever a new
    flow comes, it does following:
    - If the current counter greater than threshold, cut half
    - Repeat this until you see a counter below threshold,
      which you clear the counter for that new flow

In the paper, equation is proposed to find the relation between the sampling probability $p$ and eviction cycle $t$.

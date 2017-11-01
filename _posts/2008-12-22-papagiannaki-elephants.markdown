---
layout: post
title: "Papagiannaki et al (2001) On the Feasibility of Identifying Elephants in Internet Backbone Traffic (Sprint TR)"
date: 2008-12-22 13:56:53 -0500
category: paper
bibtex:
  type: techreport
  title: "On the Feasibility of Identifying Elephants in Internet Backbone Traffic"
  author: "K. Papagiannaki and N. Taft and S. Bhattacharya and P. Thiran and K. Salamatian and C. Diot"
  insitution: "Sprint Labs"
  howpublished: "Sprint Tech Report"
  number: "ATL Technical Report TR01-ATL-110918"
  month: November
  year: 2001
---
Objective: Identify elephant flows from all the flows, given the elephants and
mice phenomenon (or mass-count disparity in statistics term) happens in the
Internet.

Elephants and mice are defined according to their average bandwidth rather than
file size or duration. Because of the power law heavy tail property, a flow is
charactierised as an elephant when it is located at the tail of the
*flow bandwidth distribution*. A threshold is determined so that the bandwidth higher
than the threshold is said to be elephant. The threshold can be determined
initially in two ways:

1. AEST methodology, which assumes flow bandwidth is Pareto,
   $P[X>x] \sim cx^{-\alpha}$, and therefore set the threshold
   $\hat x = \min_x(\dfrac{d\log F(x)}{d\log x} - \alpha)$.
2. Constant load, set the threshold to $\hat x$ such that
   $\int_{\hat x}^\infty xf(x)dx=R\%$. The threshold is determined continuously
   and we run an auto-regressive filter on
   $\hat x$: $v(n) = (1-\beta)v(n)+\beta\hat x(n)$, and decided a flow of
   bandwidth $x(n)$ is elephant if $x(n)>v(n)$.

However, because flows are bursty, the decision can be improved by introducing
*latent heat*, which defined as: $L(n)=\sum_{t=n-k}^{n} x(t)-v(t)$. The new
decision rule is that $L(n)>0$ for elephant. This improvement is to avoid
reclassification for short-term fluctuations and let the membership of
elephants change less frequently.

To read: AEST approach to determine the Pareto distribution parameters. See
Crovella and Taqqu, "Estimating the Heavy Tail Index from Scaling Properties"
(1999)

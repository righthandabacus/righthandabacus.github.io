---
layout: post
title: "Clévenot et al (2005) Stochastic fluid models for cache clusters (PE)"
date: 2010-06-25 12:04:30 -0400
category: paper
bibtex:
  type: article
  title: "Stochastic fluid models for cache clusters"
  author: "Florence Clévenot and Philippe Nain and Keith W. Ross"
  journal: "Elsevier Performance Evaluation"
  volume: 59
  number: 1
  pages: "1--18"
  year: 2005
---
This paper models the hit rate of a cluster of cache servers.

The trivial way to use cache cluster is to partition hashing, i.e. hash an object ID into server and store the object in that particular server. This model fails if a server goes up/down, which changes the range of hash function. In this case, 50% of the storage location are changed after the membership change.

Another approach is CARP, a refined hashing technique. It uses a hash function $h(o,j)$ to do the mapping. Among all servers $j$, the hash function is computed and the object is stored in server $j^\ast = argmax h(o,j)$. This is the "winning hashing" approach. When the number of server changes, say, from $j$ to $j+1$, only $1/(j+1)$ objects are misplaced. Similarly, when the number of server changes from $j+1$ to $j$, only $1/(j+1)$ objects are lost but no object misplaced.

The paper models the cache cluster as follows:

  - $c$ = total number of objects in the universe
  - $N$ = total number of servers, including running and not running
  - $(i,x)$ = state of the system, having $i$ servers caching $x$ objects
  - $\lambda$ = up rate, $i$ increases at rate $(N-i)\lambda$
  - $\mu$ = down rate, $i$ decreases at rate $i\mu$
  - $p(i,x)$ = hit probability = $x/c$
  - $\sigma$ = request arrival rate, i.e. $x$ increases at rate $(1-p(i,x))\sigma$
  - $\theta$ = expiration rate, i.e. $x$ decreases at rate $\theta x$

Then, ignoring the node up/down events, i.e. a constant $i$, we have  

$$ \dfrac{\partial x}{\partial t} = \big(1-p(i,x)\big)\sigma - \theta x = (1-\dfrac{x}{c})\sigma - \theta x = \sigma - (\dfrac{\sigma}{c}+\theta)x $$

which yields

$$ x(t)=\eta + (x_n-\eta)\exp(-\dfrac{\sigma(t-T_n)}{\eta}) $$

with $\alpha=\dfrac{\theta c}{\sigma}$, $\eta=\dfrac{c}{1+\alpha}$, and $x_n=x(T_n)$.


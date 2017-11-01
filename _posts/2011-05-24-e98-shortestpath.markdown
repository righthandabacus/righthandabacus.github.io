---
layout: post
title: "Eppstein (1998) Finding the k shortest paths (SIAM J Computing)"
date: 2011-05-24 01:27:05 -0400
category: paper
bibtex:
  type: article
  title: "Finding the k shortest paths"
  author: "David Eppstein"
  journal: "SIAM J. Computing"
  volume: 28
  number: 2
  pages: "652--673"
  year: 1998  
---
An algorithm to find the $k$ shortest paths from node $s$ to node $t$ is presented.

In a digraph $G$, we derive a subgraph $T$ which is the shortest path tree
(spanning tree) from every node to node $t$. Then any edge $e$ not in $T$ is
called the sidetrack edges. A path from $s$ to $t$, not necessarily the shortest
path, can be uniquely identified by the set of sidetracked edges it contains:
Given a set of sidetracked edges, a path is derived as follows: We begin from
node $s$, traverse along the path in $T$. Whenever a sidetracked edge is
encountered, we traverse it and switched to another branch, until node $t$ is
reached.

For every edge $e$, we define a quantity
$\delta(e)=\ell(e)+d(e_{\textrm{tail}},t)-d(e_{\textrm{head}},t)$ to denote the
extra distance traversed by sidetracking to edge $e$ [note: the paper has this
equation wrong]. Here, $\ell(e)$ is the length of $e$, and $d(a,b)$ is the
shortest distance between nodes $a$ and $b$ according to tree $T$. If edge $e$
is in $T$, $\delta(e)=0$.  Then, the path $p$ joining $s$ to $t$ has the
distance of $\ell(p)=d(s,t)+\sum_{e\in p}\delta(e)$

The focus of this paper is to derive the complexity for enumeration of the k
paths of minimum distance, by using a heap data structure.

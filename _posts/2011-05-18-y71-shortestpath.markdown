---
layout: post
title: "Yen (1971) Finding the K Shortest Loopless Paths in a Network (Management Sci)"
date: 2011-05-18 12:00:27 -0400
category: paper
bibtex:
  type: article
  title: Finding the K Shortest Loopless Paths in a Network
  author: "Jin Y. Yen"
  journal: Management Science
  volume: 17
  number: 11
  series: "Theory Series"
  pages: "712--716"
  month: Jul
  year: 1971
---
This paper gives a survey on the previous algorithms to find $$k$$ shortest loopless paths in a network and proposed a new one.

# Survey

    @techreport{bkh,
        title = {An Algorithm (The rh Best Path Algorithm) for Finding and Ranking Paths Through a Network},
        author = {F. Bock and H. Kantner and J. Haynes},
        institution = {Armour Research Foundation},
        address = {Chicago Illinois},
        month = {Nov 15},
        year = 2007,
    }

This enumerates *all* possible paths from the origin to the sink, then sorts
from these the K paths that have the shortest lengths.

    @article{p,
        title = {The kth best route through a network},
        author = {M. Pollack},
        journal = {Opns. Res.},
        volume = 9,
        number = 4,
        pages = 578,
        year = 1961,
    }

This finds the $$K$$-th shortest path by first obtaining the $$K-1$$ shortest paths.
Then the distance of each arc in each of the 1st, 2nd, ..., $$(K-1)$$st shortest
paths is set, in turn, to infinity. The shortest-path problem is solved   for
each such case. The best of these resulting shortest paths is the desired K-th
shortest path.

    @article{ckr,
        title = {Computing the N Best Loopless Paths in a Network},
        author = {S. Clarke and A. Krikorian and J. Rausan},
        journal = {J. of SIAM},
        volume = 11,
        number = 4,
        pages = {1096--1102},
        month = Dec,
        year = 1963,
    }

This finds the shortest path first, then finds the $$K$$ shortest path from all
paths that branch out from the shortest path.

    @techreport{s,
        title = {The k Shortest Routes and the k Shortest Chains in a Graph},
        author = {M. Sakarovitch},
        institution = {Opns. Res. Center, University of California, Berkeley},
        number = {ORC-32},
        month = Oct,
        year = 1966,
    }

This finds $$H>K$$ shortest paths that may contain loops, then the $$H$$ paths are scanned for the $$K$$ shortest paths that contain no loops.

# Algorithm

The algorithm proposed in this paper is as follows:

```
A[1] := Shortest path from S to T
for k := 2 to K do
    for i := 1 to length(A[1])-1 do
        nodeA = A[k-1].node(i)
        for j := 1 to k-2
            nodeB = A[j].node(i)
            if (nodeA == nodeB) then
                distance(nodeA,nodeB) = infinity
            endif
        endfor
        S[i] = The shortest path from nodeA to T according to the current distance values
        R[i] = The path in A[k-1] from S to nodeA
        B[i] = R[i] + S[i]
    endfor
    A[k] = min length paths amongst all B[i]
    restore distance(nodeA,nodeB) to original value if modified
endfor
```

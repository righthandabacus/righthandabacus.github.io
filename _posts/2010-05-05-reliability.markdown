---
layout: post
title: "The Combinatorics of Network Reliability"
subtitle: "Charles J. Colbourn / 1987"
date: 2010-05-05 15:12:25 -0400
category: book
bibtex:
  type: book
  title: The combinatorics of network reliability
  series: International Series of Monographs on Computer Science
  author: Charles J. Colbourn
  publish: Oxford University Press
  year: 1987
  code: TK 5105.5 C57 1987
---

This is a book I know when I was reading a paper that cited it. I did not read it from cover to cover but something interesting inside.

First, it talk about how to find the probability of operation of a network. It
is not as easy as it may seem because, in a network of $$N$$ edges, there are $$2^N$$
subsets of them. To enumerate all of them and compute their aggregate
operational probability, and tell if each subset maintains connectivity of the
whole network or between two specified nodes, it is clumsy.

The book referred a *factor theorem*. It says, the reliability measure of a network $$G$$, denoted $$Rel(G)$$, is

$$  Rel(G) = p_e Rel(G.e) + (1-p_e) Rel(G-e) $$

where $$G.e$$ denotes the graph $$G$$ with edge $$e$$ contracted and $$G-e$$ denotes the graph $$G$$ with edge $$e$$ removed. By using the factor theorem, we can have the following algorithm to find all spanning trees on a network:

```C
Graph G=(V,E)
Root node s∈V
X = {s}  /* Nodes covered by this spanning tree */
T = ∅  /* Edge in spanning tree */
C = E  /* Candidate edges for the spanning tree */
function Tree_Gen(X, T, C)
{
    while(true) {
        if (X == V) {
            output T;
            return;
        };
        find e = {x,y}∈C such that x∈X, y∈V\X;
        if (e is not a bridge in G’=(V,T∪C)) {
            break; /* We branch on this edge */
        }
        X = X∪{y};
        T = T∪{e};
    };
    /* Branching */
    Tree_Gen(X∪{y}, T∪{e}, C\{e});
    Tree_Gen(X, T, C\{e});
}
```

Similarly, we can enumerate all loop-free paths from node $$s$$ to node $$t$$:

```C
Graph G=(V,E)
Nodes s,t∈V
X = {s}  /* Nodes traversed in this path */
P = ∅  /* Edges in this path */
C = E  /* Candidate edges for the path */
x = s  /* Last node on the path P */
function Path_Gen(X, x, P, C)
{
    if (x == t) {
        output P;
        return;
    };
    if (no path from x to t through nodes V\X possible) {
        return;
    };
    find e = {x,y}∈C such that y∈V\X, e∈C;
    /* Branching */
    Path_Gen(X∪{y}, y, P∪{e}, C\{e});
    Path_Gen(X, x, P, C\{e});
}
```

These algorithms are the basics to find the reliability probability. Now consider a network of Wheatstone bridge:

     v1 -----(e4)------ t
     | \                |
     |   \              |
     |     \            |
    (e1)     (e3)      (e5)
     |           \      |
     |             \    |
     |               \  |
     s -----(e2)------ v2

The connectivity between $$s$$ and $$t$$ is maintained by four paths:

  - $$P_1$$ = {e1, e4}
  - $$P_2$$ = {e1, e3, e5}
  - $$P_3$$ = {e2, e3, e4}
  - $$P_4$$ = {e2, e5}

Let event $$E_k$$ represents path $$P_k$$ is operational. And denote the operational probability of link $$e$$ by $$p$$. Then we have the following table

| Event                                 | Probability    | Inclusion-Exclusion |
|-------------------------------------- |----------------|-----|
| $$E_1$$                               | p1 p4          |  +  |
| $$E_2$$                               | p1 p3 p5       |  +  |
| $$E_3$$                               | p2 p3 p4       |  +  |
| $$E_4$$                               | p2 p5          |  +  |
| $$E_1$$ ∧ $$E_2$$                     | p1 p3 p4 p5    |  −  |
| $$E_1$$ ∧ $$E_3$$                     | p1 p2 p3 p4    |  −  |
| $$E_1$$ ∧ $$E_4$$                     | p1 p2 p4 p5    |  −  |
| $$E_2$$ ∧ $$E_3$$                     | p1 p2 p3 p4 p5 |  −  |
| $$E_2$$ ∧ $$E_4$$                     | p1 p2 p3 p5    |  −  |
| $$E_3$$ ∧ $$E_4$$                     | p2 p3 p4 p5    |  −  |
| $$E_1$$ ∧ $$E_2$$ ∧ $$E_3$$           | p1 p2 p3 p4 p5 |  +  |
| $$E_1$$ ∧ $$E_2$$ ∧ $$E_4$$           | p1 p2 p3 p4 p5 |  +  |
| $$E_1$$ ∧ $$E_3$$ ∧ $$E_4$$           | p1 p2 p3 p4 p5 |  +  |
| $$E_2$$ ∧ $$E_3$$ ∧ $$E_4$$           | p1 p2 p3 p4 p5 |  +  |
| $$E_1$$ ∧ $$E_2$$ ∧ $$E_3$$ ∧ $$E_3$$ | p1 p2 p3 p4 p5 |  −  |

Then the reliability is these sum, i.e.

    p1p4 + p1p3p5 + p2p3p4 + p2p5
    - p1p3p4p5 - p1p2p3p4 - p1p2p4p5 - p1p2p3p5 - p2p3p4p5
    + 2(p1p2p3p4p5)

Note, we must first enumerate the path-edge relationship to compute by the inclusion-exclusion method, because the reliability of two paths are not always independent events due to edge-sharing.

In case all links have equal operational probability, we have reliability polynomial. Take the same network as an example, to have *all* nodes connected, it requires at least one of the following pathset is operational:

  e1e2e4, e1e2e5, e1e3e4, e1e3e5, e1e4e5, e2e3e4, e2e3e5, e2e4e5,
  e1e2e3e4, e1e2e3e5, e1e2e4e5, e1e3e4e5, e2e3e4e5,
  e1e2e3e4e5

By counting, we have eight 3-link pathset, five 4-link pathset, and one 5-link pathset. The reliability polynomial in this 5-link network is therefore
$$8p^3(1-p)^2 + 5p^4(1-p)^1 + 1p^5(1-p)^0$$.

In general, a reliability polynomial is in the form $$\sum_{k=0}^n N_kp^k(1-p)^{n-k}$$

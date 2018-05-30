---
layout: post
title: "Lam et al (1991) The Cache Performance and Optimization of Blocked Algorithms (ASPLOS)"
date: 2010-09-29 18:55:16 -0400
category: paper
bibtex:
  type: inproceedings
  title: "The Cache Performance and Optimization of Blocked Algorithms"
  author: "Monica S. Lam and Edward E. Rothberg and Michael E. Wolf"
  booktitle: "Proceedings of the 4th International Conference on Architectural Support for Programming Languages and Operating Systems"
  pages: "63--74"
  howpublished: ASPLOS
  year: 1991
---
Blocking/Tiling is a well-known way to make program faster by leveraging the properties of memory hierarchy.

Take matrix multiplication as an example, multiplying two NxN matrices with the following code (assuming row-major):

```pascal
for i := 1 to N do
    for k := 1 to N do
        r := A[i,k];
        for j := 1 to N do
            C[i,j] += r * B[k,j];
```

This code is performing good for several reasons:

  - The innermost loop accesses consecutive data in `B` and `C`,
    which utilizes the cache prefetch mechanism.
  - The same row of `C` is reused in each iteration of the middle loop

And if the matrices are small, `B` can store entirely in the cache, then the
code can be fast. If the matrices are large, so that the cache cannot even hold
a row of data (`C`), the cache is never reused. In this case (worst case), there
would be $$2N^3+N^2$$ elements read in $$N^3$$ iterations.

The blocked code is like the following:

```pascal
for kk := 1 to N step b do        {* N/b blocks *}
    for jj := 1 to N step b do    {* N/b blocks *}
        for i := 1 to N do        {* each col in C *}
            for k := kk to min(kk+b-1, N) do  {* computation on a block in B *}
                r := A[i,k];      {* loop over a vector in A *}
                for j := jj to min(jj+b-1,N) do
                    C[i,j] += r * B[k,j];     {* vector in C = vector in A x block in B *}
```

This code would be efficient if the block of `B` of size bxb can reside inside the
cache, so that it can be reused throughout the loop on `i`. Then the memory read
would be $$2N^3/b+N^2$$, reduced by a factor of $$b$$ at most. The optimal value (by
Hong & Kung, 1981) of blocking factor $$b$$ is roughly square root of $$c$$, where $$c$$ is
the size of the cache.

In practice of running these algorithms, the performance is not as good as
expected because of the interference misses in the cache --- because the cache
is set associative.

The cache misses is higher than expected because there can be interferences:
there could be *cross interference* when two different variables conflicts each
other in cache, or *self interference* when elements of the same array variable
conflicts with itself in cache.

The paper elaborated that, the intrinsic cache miss is $$N^3(2/b+4b/c)$$, which is
the best performance that can be achieved. So minimizing this yields blocking
factor of $$b=\sqrt{c/2}$$. In case of a-way set associative cache is used, the
optimal block size would be (heuristic) $$b=\sqrt{c(a-1)/2a}$$

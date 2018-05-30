---
layout: post
title: "Schwartzian Transformation"
date: 2009-04-04 00:16:56 -0400
tags: code
---

Assume you have a vector A and you going to sort it according to a function
`f(x)`. If you sort it naively, whenever you are going to compare two items, you
call the function. Because the best sorting algorithm we know nowadays is
$$O(n \log n)$$, you call such function a lot.

Schwartzian transformation is, simply speaking, create a 2D array of (key,value)
pair by running the function `f(x)` against each item. Then do the sorting and
remove the key after finished. This makes less call to `f(x)`.

Seems trivial, but what is beautiful about Schwartzian transform is in perl:

``` perl
@sorted =
  map  { $_->[0] }
  sort { $a->[1] <=> $b->[1] }
  map  { [$_, f($_)] }
  @unsorted;
```

All these can be done in one line, just as the `sort` call.


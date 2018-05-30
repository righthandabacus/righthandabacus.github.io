---
layout: post
title: "Opposite of a Bloom Filter"
date: 2012-07-16 18:54:10 -0400
tags: math
---
Bloom filter gives a $$O(1)$$-efficient way to test for set memberships, but with false positives and no false negatives, i.e. it will tell you $$x\in S$$ while actually it is not, but not vice versa.

To get a data structure for similar set membership testing, but with false negatives only, is an interesting problem. Google it you will see the (only?) solution as a direct-mapped cache. Use a fix-size hash table and a hash function, and assume the item is hashable and comparable, then

``` python
class cache:
    size = 100
    table = [None]*size
    def add(item):
        table[hash(item) % size] = item
    def isMember(item):
        return table[hash(item) % size] == item
```

It has false negative because the item in the hash table could be erased.

Reference:  
<http://somethingsimilar.com/2012/05/21/the-opposite-of-a-bloom-filter/>

---
layout: post
title: "destutter"
date: 2011-12-11 00:43:29 -0500 -0500
tags: code
---

I read across Yaron Minsky's article in ACM Queue, <http://queue.acm.org/detail.cfm?id=2038036>

He used an example, destutter, to show the benefit of OCaml. The destutter simple keeps only one item in a list when there are many consecutive items there. Such as the input of `[1,1,1,2,2,3,3,4]` becomes `[1,2,3,4]`.

For this, he wrote

```python
def destutter(list): 
    l = [] 
    for i in range(len(list)): 
        if i + 1 >= len(list) or list[i] != list[i+1]: 
             l.append(list[i]) 
    return l 
```

It works, but not compact. At 2011, we can use Python 2.5's if-else construct to do the same thing. Taking a functional perspective, I would do in this way:

```python
destutter = lambda l : l if len(l) < 2 else destutter(l[0:1]+l[2:]) if l[0]==l[1] else l[0:1]+destutter(l[1:])
```

That's just one line, and you can test it in the following:

```python
a = [1,1,2,3,3,4]
b = [1,1,1,2,2,3,3,3,4,5]

destutter = lambda l : l if len(l) < 2 else destutter(l[0:1]+l[2:]) if l[0]==l[1] else l[0:1]+destutter(l[1:])

print str(a) + " becomes " + str(destutter(a))
print str(b) + " becomes " + str(destutter(b))
```

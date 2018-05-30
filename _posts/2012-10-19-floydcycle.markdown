---
layout: post
title: "Floyd's cycle-finding algorithm"
date: 2012-10-19 14:16:40 -0400
tags: math
---
A well-known interview question: How to determine if a linked-list has a loop?
The answer is to have two pointers scanning the linked-list. If the linked-list
doesn't contain any loop, we should be able to find the end of the linked-list.
If the linked-list does contain a loop, then with one pointer advancing one node
at a time and the other advancing two nodes at a time, they eventually collide.

But why? Let us represent the linked list as a function $$f(i)$$, which $$i$$ is the
ordinal number of the element of the linked list. Assume the linked list has a
loop of length $$T$$, then $$f(i+T)=f(i)$$ for some large enough $$i\ge m$$.
Therefore, with two pointers, we are checking if $$f(i)=f(2i)$$ for
$$i=1,2,\cdots$$. Since $$f(i+T)=f(i)$$, we also have $$f(i+kT)=f(i)$$ for $$i\ge m$$.
Hence, for $$k'T\ge m$$, we have $$f(k'T+k'T)=f(k'T)$$ or $$f(2k'T)=f(k'T)$$. Thus
whenever there is a loop, such algorithm can find it. Moreover, once
$$f(i)=f(2i)$$ is found, $$T$$ is a factor of $$i$$ and $$m\le i$$.

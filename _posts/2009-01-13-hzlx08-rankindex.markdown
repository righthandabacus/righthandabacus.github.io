---
layout: post
title: "Hua et al (2008) Rank-indexed Hashing: A Compact Construction of Bloom Filters and Variants (ICNP)"
date: 2009-01-13 16:28:48 -0500
category: paper
bibtex:
  type: inproceedings
  title: "Rank-indexed Hashing: A Compact Construction of Bloom Filters and Variants"
  author: "Nan Hua and Haiquan (Chuck) Zhao and Bill Lin and Jun (Jim) Xu"
  booktitle: "Proc ICNP"
  year: 2008
---

[PDF](https://drive.google.com/open?id=0B6DoI_vm0OLfU19EWHlDc0t2RlE)

This paper is a good survey of Bloom filter variants proposed in the history.

In Sect 2A, it introduces Bloom filters. Denotes number of bits as $m$, number
of element as $n$ and number of hash function as $k$, when using
$m=\frac{1}{\ln 2}kn \approx 1.44kn$, the false positive probability is
$\epsilon = (1-(1-\frac{1}{m})^{kn})^k \approx (1-e^{-kn/m})^k$. However, the
paper by Mitzenmacher in 2002 that proposed compressed Bloom filter pointed out
that the standard size of $1.44kn$ bits actually maximises the information
entropy of the Bloom filter. If we use more bits in the Bloom filter and apply
compression, the resultant size can be smaller. In the extreme case of using
infinite number of bits, the compressed size can reach the theoretical limit
$kn$. This property is favorible to some applications such as P2P that the
transferring cost is higher then the memory cost.

Section 2B mentioned the fingerprint hash table which proposed in the seminal
paper 'Networking Application of Bloom Filters: A Survey' (Broder and
Mitzenmacher, 2003). Its concept is that, if we have a perfect hash function
that hashes the $n$ element into $n$ bits without collision, and we have certain
fairly good hash function that return $(\log 1/\epsilon)$ random bits
(fingerprint) from an element, we can implement a membership query structure
similar to Bloom filter. The way to do is to put the $(\log 1/\epsilon)$ bits
into the bin pointed by the perfect hash function. For an element being queried,
it is a member if the bin's content is exactly the same $(\log 1/\epsilon)$
bits. In this case, the total memory used is optimal. Although perfect hashing
is hardly achievable.

In order to tolerate collision due to imperfect hash function, a linked list at
the bins is proposed. However, storing pointers in a linked list is a huge
overhead, compare to the $(\log 1/\epsilon)$ bits required for an element hash.
Another approach is called the $d$-left hashing, which use multiple subtables.
This is also using a lot of space.

This paper proposed rank-indexed hashing. Which is using linked list to store
fingerprints but can avoid the storage of lengthy pointers. The idea assumes we
have a `popcount` function, which returns the number of 1-bits in a bit vector.
This `popcount` function is available as machine instruction in x86-64
architecture. Given we have this function, the linked list can be implemented as
follows:

Assume we have $L$ buckets, each bucket may have a linked list or empty. Each
element in a linked list can have a successor or not. So looking at all linked
list as a whole, we can find some lists with 1st element, some lists with 2nd
element, etc. We put all 1st elements in an array and all 2nd elements in
another array, etc. We concatenate all these arrays into one fingerprint vector.
Each $n$-th element array has a corresponding bit vector, so does the $L$
buckets.

To traverse a linked list, we do the following: The linked list at bucket $j<L$
starts at location $k=\texttt{popcount}(I,j)$ of the fingerprint vector, where
$I$ is the bit vector of the $L$ buckets. The second element of such linked list
is at $\texttt{popcount}(I,L)+\texttt{popcount}(A,k)$ where $A$ is the bit
vector of 1st elements. So and so forth. A linked list is terminated if the
$k$-th bit in $A$ is zero.

---
layout: post
title: "Kraska et al (2017) The Case for Learned Index Structure (arXiv)"
date: Wed, 03 Jan 2018 23:54:41 -0500
category: paper
bibtex:
  type: article
  title: "The Case for Learned Index Structure"
  author: Tim Kraska and Alex Beutel and Ed H. Chi and Jeffrey Dean and Neoklis Polyzotis
  journal: arXiv
  number: arXiv:1712.01208v1
  month: "December"
  year: 2017
  url: https://arxiv.org/abs/1712.01208
---

This paper ([PDF](https://arxiv.org/pdf/1712.01208)) investigates the use of
neural network in deriving index structures (`learned indexes`), in the
direction to replace B-tree, hash map, or Bloom filters. The paper claims that
the index derived from neural network can outperform B-tree index in both speed
and space.

## The Case of B-tree

B-tree is a model that takes a key as an input and predicts he position of data
record. The paper assumes the records are stored in an array and sorted by the
records' keys. The B-tree is a look up function such that for an input key, the
output is the position in the array such that the record has a key equal to or
greater than the look up key.

B-tree is therefore a *regression tree* in machine learning. It maps a key to a
position with min- and max-error (min-error = 0, max-error = page size).

The paper's proposal is to use neural network to derive function for a *static*
set of $N$ keys (i.e., retraining may be required when the records are updated).
If we found this function, we are turning the $O(\log n)$ B-tree look up into a
$O(1)$ function computation.

Because we assumed the $N$ records are stored in an array in sorted order, the
function to find is a CDF (cumulative distribution function) scaled to $[0,N)$.
If the CDF for key $k$ is $F(k)=\Pr[X\le k]$, the function we are looking for is

$$ p = F(k) \times N $$

which $p\in [0,N)$.

The paper tried with a two-layer fully-connected neural network with 32 neurons
per layer and ReLU activation functions (i.e., $f(x) = \max(0,x)$) to find the
CDF. The resultant CDF is time and space efficient to *narrow down* the position
of a record to a region of thousands, but to further reduce the error will
require much more space and CPU time (the "last mile" index).

To overcome such challenge, the paper mentioned learning index framework (LIF)
and recursive model index (RMI). The LIF generates different index
configurations, optimizes them and test them. RMI structure the index into
steps in order to refine the error margin. It is a recursive regression model
$f(k) \in [0,N)$ such that at each stage $i$, the model $f_i^{(\ell)}(x) \approx y$ takes
the key and picks another model based on the stage output until the final stage,
then predicts the position. Indeed each stage has certain models
$f_i^{(1)}, \cdots, f_i^{(L)}$ and each model $f_i^{(\ell)}(x)$ is a valid
approximation for CDF with error minimized for a subrange of output $[0,N)$. By
staging models, we reduce the error in output (i.e., the last mile accuracy) and
complexity in each function.

If the model is really hard to learn, we can replace the last-stage function
with a small B-tree, forming a *hybrid index model*.

For insert and update, we consider that the model is approximating the CDF. If
the CDF does not change for the update, it is not necessary to retrain the
model. One simple approach to handle inserts is to build a delta-index which
all inserts are kept in buffer and merge back from time to time with potential
retraining of the model.

## The case of hash map

Hash map is a point index. The challenge to a hash map is the hash collision.
The case in Google's dense-hashmap shows a typical 78% overhead, i.e., for a
data that takes up 100% in space we need a hash data structure of size 178%.
Having a good hash function reduces the number of collisions as well as the
space overhead in storage.

The CDF of the key distribution, as investigated in earlier part of the paper,
is a hash function. If the model perfectly learned the CDF, there will be no
collision.

## The Case of Bloom filter

Bloom filter is a binary classifier, which based on a key to predict if it
exists in a set. Bloom filter is space-efficient but still significant: a 100M
record with false positive rate of 0.1% requires 1.76 Gbyte (14 times more bits
the number of records).

Bloom filter is a binary classification. We can use a neural network with
sigmoid activiation function and trained to minize the log loss:

$$
L = \sum_i y_i \log f(x_i) + (1-y_i) \log (1-f(x_i))
$$

where $x_i$ are records which also serve as input to the binary model function
$f(x)$ and $y_i = {0,1}$ are correct classification for $x_i$.

## Sidetrack

- Latency for transferring input and retrieving result from a GPU is roughly $2\mu s$
  or thousands of clock cycles
- Learning with CPU: we can rarely afford neural nets with more than 1 or 2
  fully-connected hidden layers and 8 to 128 neurons per layer
- For keys in string form, we consider an $n$-length string a vector of
  $\mathbb{R}^n$ with each element the ASCII decimal value, and we usually cap
  the input length to $N$ for the efficiency of the learning model
- RNN (recurrent neural networks) or CNN (convolutional neural networks) have
  shown to be very successful in modeling text

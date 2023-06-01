---
layout: post
title: "Explaining Attention Mechanism"
date: 2023-05-21
tags: math
highlightjs:
  - python
---

Attention mechanism was first mentioned in Bahdanau et al (2015) paper titled
"Neural Machine Translation by Jointly Learning to Align and Translate", and
Luong et al (2015) improved it with the paper "Effective Approaches to
Attention-based Neural Machine Translation". The key is to find the *attention
score* $a_{ij}$ between two state vectors, $h_i$ and $s_j$. Should there be
many $h_i$ and $s_j$, the attention score can tell which pair is most relevant.

The steps in producing the attention score are as follows:

1. Take $h_i$ (e.g., from encoder output) and $s_j$ (e.g., from decoder output)
2. With a function $a(\cdot,\cdot)$, compute $e_{ij} = a(h_i, s_j)$; this function can be implemented as a neural network, e.g., $a(h_i,s_j) = v^\top \tanh(W[h_i;s_j])$
3. Compute $\alpha_{ij} = \dfrac{\exp(e_{ij})}{\sum_k\exp(e_{ik})}$

If we are to compute the context vector of a sequence of vectors $h_i$, it can be the weighted sum

$$
c_j = \sum_{i=1}^T \alpha_{ij}h_i
$$

But computing $\alpha_{ij}$ from function $a(\cdot,\cdot)$ means the function
is to be called for $M\times N$ times for $M,N$ the cardinality of $h_i$ and
$s_j$ respectively. We can reduce this complexity to $M+N$ by first mapping
$h_i$ and $s_j$ to a common vector space:

$$
\begin{aligned}
q_i &= f(h_i) \\
k_j &= g(s_j) \\
e_{ij} &= q_i k_j^\top
\end{aligned}
$$

The nonlinearity is moved to $f(\cdot)$ and $g(\cdot)$ and $e_{ij}$ can be
computed all at once using matrix multiplication.

In the Vaswani et al (2017) paper, "Attention is All You Need", single-head
attention mechanism is an extension to the above to introduce the term, query,
key, and value. At a high level, query sequence $Q$ and key sequence $K$ are
used to compute attention score matrix $\alpha$, which is then
matrix-multiplied with value sequence $V$ to produce the output $\alpha V$.

Precisely, $Q$ and $K$ are sequences of vectors stacked into matrix form. Then
within the attention module, learnable matrices are multiplied with them, and
compute the attention output $O$ from the attention score and value $V$:

$$
\begin{aligned}
q &= QW^Q \\
k &= KW^K \\
v &= VW^V \\
\alpha &= \text{softmax}\Big(\frac{q k^\top}{\sqrt{d_k}}\Big) \\
O &= \alpha v = \text{attn}(Q, K, V)
\end{aligned}
$$

where softmax function is to compute
$\sigma(z_i) = \exp(z_i) / \sum_j \exp(z_j)$, each row of keys share the same
softmax and each query is independent. Note that usually $Q,K,V$ are in the same
dimension size in each sequence step. Transformation matrices $W^Q,W^K$ should
be of same shape to make $qk^\top$ possible. But $W^V$ can be of different
dimension (the output dimension size).

Multihead attention is a stack of $h$ single-head attention, each with
independent weights $W^Q, W^K, W^V$. The $h$ outputs are concatenated, then
transformed with a matrix:

$$ \text{MultiHead}(Q,K,V) = [O_1;\dots;O_h] W^O $$

where the shape of transformation matrices are:

$$
\begin{aligned}
W_i^Q &\in \mathbb{R}^{d_{\text{model}}\times d_k} \\
W_i^K &\in \mathbb{R}^{d_{\text{model}}\times d_k} \\
W_i^V &\in \mathbb{R}^{d_{\text{model}}\times d_v} \\
W_i^O &\in \mathbb{R}^{hd_v\times d_{\text{model}}}
\end{aligned}
$$

In many cases, $K,V$ are the same. For example, in translation, $Q$ is the
target sequence and $K,V$ are both the source sequence. In a recommendation
system, $Q$ is the target items and $K,V$ is the user profile. In language
models, self-attention is used and all $Q,K,V$ are the same. We often see $K=V$
when we need to relate different positions of the same sequence to one another
(e.g., what "it" refers to in the sentence).

Usually, feed-forward network is after the attention mechanism to introduce
nonlinearity. This feed-forward network is to replace the *hidden state* of an
RNN to extract hierarchical features.

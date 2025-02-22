---
layout: post
title: "Kwon et al (2023) PagedAttention"
date: 2025-02-05
category: paper
bibtex:
  type: inproceedings
  title: "Efficient Memory Management for Large Language Model Serving with Paged Attention"
  author: "Woosuk Kwon and Zhuohan Li and Siyuan Zhuang and Ying Sheng and Lianmin Zheng and Cody Hao Yu and Joseph E. Gonzalez and Hao Zhang and Ion Stoica"
  booktitle: "In Proceedings of the Symposium on Operating System Principles (SOSP)"
  month: Oct
  year: 2023
  address: "Koblenz, Germany"
  arXiv: 2309.06180
---

This is the paper that proposed PagedAttention and crafted the design of vLLM.
The authors pointed out that LLMs are autoregressive. One token is generated
from the prompt concatenated with the previously generated sequence. The KV
cache (described as "incremental multi-head attention" in the GQA paper) is to
share the memory between subsequent iteration of the autoregressive generation.
The paper is not about a particular transformer architecture, but focus on the
system to serve LLM. Usually those systems will store KV cache in a contiguous
memory space. The paper propose to borrow the idea of paged memory management
from OS to allow more efficient use of GPU memory, and hence supporting a
system with higher throughput.

The problem of KV cache is that the contiguous chunk of memory is pre-allocated
at the maximum length and reserved during the entire lifetime of the generation.
PagedAttention is building a virtual memory system with paging, such that the
KV cache is divided into blocks. Each block is in a fixed size for the fixed
number of attention keys and values.

## The problem

Language model is to predict a sequence of tokens $x=(x_1,\dots,x_n)$ as a
product of conditional probabilities (a.k.a. autoregressive decomposition):

$$
P(x) = P(x_1)P(x_2\mid x_1)\dots P(x_n\mid x_1,\dots, x_{n-1})
$$

The probability of each token $P(x_i\mid x_1,\dots,x_{i-1})$ is computed as the
dot product of attention score and the value vector:

$$
\begin{aligned}
o_i &= \sum_{j=1}^i a_{ij}v_j \\
a_{ij} &= \frac{\exp(q_i^\top k_j / \sqrt{d})}{\sum_{t=1}^i \exp(q_i^\top k_t / \sqrt{d})}
\end{aligned}
$$

in which $q_i,k_i,v_i$ are (column) vectors, projected from the input vector
$x_i$. The output vector $o_i$ is a weighted sum of $v_1,\dots,v_i$ with
attention scores as weights. As the generation progress from $o_i$ to
$o_{i+1}$, all vectors of $q_1,\dots,q_i$ etc. are cached and reused. The size
in bytes of the KV cache needed is calculated as follows: 2 (key and value) ×
hidden size × number of layers in model × sequence length × sizeof(float).
This can easily explode.

For a system serving LLMs, requests come in different times and for different
lengths should be served in batch. This further create inefficiency in memory.

## The solution

PageAttention is an algorithm in vLLM engine. There is a centralized scheduler
to coordinate the execution of GPU workers. There is also a KV cache manager to
maintain the physical memory.

The memory unit is a KV block. Each block holds a fixed number $B$ of tokens
for key and value. The attention computation is then in blocks:

$$
\begin{aligned}
K_j &= (k_{(j-1)B+1},\dots,k_{jB}) \\
V_j &= (v_{(j-1)B+1},\dots,v_{jB}) \\
o_i &= \sum_{j=1}^{\lceil i/B\rceil} V_j A_{ij}^\top \\
A_{ij} &= \frac{\exp(q_i^\top K_j / \sqrt{d})}{\sum_{t=1}^{\lceil i/B\rceil} \exp(q_i^\top K_t / \sqrt{d})}
\end{aligned}
$$
The query vectors $q_i$ is still processed one at a time, but keys are
processed in blocks $K_j$. The exponential function is applied elementwise and
$A_{ij}$ is a row vector of length $B$. The output vector $o_i$ is the weighted
sum of $v_{(j-1)B+1},\dots,v_{jB}$. In this way, the full output $o_i$ can be
computed one block at a time and then sum up the resulting vectors from all the
blocks. The blocks can be non-contiguous since one block is used at a time.

A block in the KV cache is like a page in virtual memory system. The KV block
manager maintains a block table to translate logical blocks to physical blocks.
The blocks' usage grows only when needed. Newly generated tokens are added to
the same block if space is available. Moreover, copy-on-write and garbage
collection on blocks is possible by implementing reference counting in the
manager's table.

Beam search is common in serving LLMs. The model generate multiple sequences
for the same input prompt by creating $k$ (the beam width) top tokens in each
step, recursively, and keep only the top $k$ sequences generated. The trivial
LLM serving system will need to copy the sequence in each step. In
PagedAttention, since blocks can be shared between sequences when CoW is
implemented, less copying results and more efficient memory usage.

Sharing blocks between different sequence is also useful for storing the system
prompt, which is unchanged for multiple inputs.

vLLM is a system with the memory manager and PagedAttention. vLLM implemented
GPT, OPT, and LLaMA. It can serve throughput at 2× to 4×. The memory manager
also implemented heuristic-based cache eviction policies, in which when GPU
memory runs low, blocks that will be used furthest in future are moved to CPU
memory. Since the manager layer is created, vLLM supports Megatron-LM style
parallelism across distributed GPUs.

The implementation of vLLM as reported by the paper was using 8.5K lines of
Python and 2K lines of C++.

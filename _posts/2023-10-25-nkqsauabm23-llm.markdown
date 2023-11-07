---
layout: post
title: "Naveed et al (2023) A Comprehensive Overview of LLMs"
date: 2023-06-10
category: paper
bibtex:
  type: unpublished
  title: "A Comprehensive Overview of Large Language Models"
  author: "Humza Naveed and Asad Ullah Khan and Shi Qiu and Muhammad Saqib and Saeed Anwar and Muhammad Usman and Naveed Akhta and Nick Barnes and Ajmal Mian"
  year: 2023
  arXiv: 2307.06435
---

This is a survey paper of the current status of large language models. It is a
nice read because it is short (37 pages only) and covered some key components
needed to understand what these models are about.

My notes below:

A chart of signature models presented in Fig.2 of the paper:

![](/img/LLM-timeline.png)

By consensus, a language model is considered large if there are 10B parameters or more.

Pretrained models can be fine-tuned for downstream tasks
- example tasks: code generation, text generation, tool manipulation, reasoning
- GPT-3 demonstrated impressive performance without fine-tuning

Instruction fine-tuning: incorporating additional prompts or instructions during fine-tuning to guide the output, such that users can get more fine-grained control over the output.

Parameter-efficient fine-tuning (PEFT): updating only a small number of parameters

## Components

### Tokenization

Tokenization is a preprocessing step to convert strings into characters, subwords, or symbols. Some common segmentation techniques are as follows.

- WordPiece selects tokens that increase the likelihood of n-grams
- Byte Pair Encoding (BPE) is from a compression algorithm, which iteratively generates tokens where pairs of adjacent symbols are replaced by a new symbol
- UnigramLM is based on a unigram language model trained with subword units

### Attention Layer

Self-attention aka intra-attention. This layer connects all the sequence positions to learn about the long-range dependencies
$$A=\sigma(\dfrac{QK^\top}{\sqrt{n}})$$
Cross-attention is seen in encoder-decoder architectures. The output of the encoder blocks acts as queries, and input from the decoder are the keys and values.

The native way of computing attention by the full sequence is called *full attention*. The complexity is $O(n^2)$. Alternatively, we can approximate it with *sparse attention*. To speed up the access to GPU memory, there is a *Flash Attention* technique to tackle the bottleneck of memory in computing attention by tiling memory blocks. Flash attention allows longer context-length in LLMs.

### Position Encoding

Transformer introduced positional encoding because attention is agnostic to input sequence order. There are absolute (assigning unique identifier to each position) and relative encodings (to signal relative dependencies). Encodings are added to the input sequence. Relative positional encoding can be learned.

Two famous relative encodings are Alibi and RoPE:
- Alibi: Scalar bias to subtract from attention score, which is calculated such that the bias increases with the distance between positions of the tokens. This favors neighboring tokens.
- RoPE (rotary position encoding): Rotation of query and key representations at an angle proportional to their absolute position difference of two token, hence the encoding decays with the distance

### Activation functions

The common variations of activation functions used in LLMs are as follows:

- ReLU, $ReLU(x) = \max(0, x)$
- GeLU: Gaussian error linear unit, as a combination of ReLU, dropout, and zoneout. $GeLU(x) = x\Phi(x)$ for $\Phi(x)$ the standard Gaussian CDF.
- GLU: Gated linear unit, which is a neural network layer with elementwise product. $GLU(x) = (xW+b)\otimes \sigma(xV+c)$ with $\sigma(\cdot)$ the sigmoidal function, $W,V$ the weight matrices, and $b,c$ the bias vector of an FC layer
	- there are variations of GLU, such as using ReLU, GeLU, or Swish function instead of $\sigma(\cdot)$ above
	- Swish function: $Swish_{\beta}(x)=x\sigma(\beta x)$

### Layer normalization

Goal of LN is for faster convergence. There are two ways of doing normalization:

1. LayerNorm: Compute statistics over hidden units, $\bar{a} = \frac{1}{n}\sum_i a_i$ for usually $a_i$ are across the elements of a vector of one sequence position. And $\sigma_a^2 = \frac{1}{n} \sum_i (a_i - \bar{a})^2$. The output of the LayerNorm is to re-center the distribution of each instance, with the gain parameter $g_i$, it updates to	$$\hat{a}_i = \frac{g_i}{\sigma_a}(a_i - \bar{a})$$
2. RMSNorm: Computationally efficient normalization that trades off re-centering invariance with speed. Computes
$$\hat{a}_i = \frac{g_i}{\sqrt{\frac1n\sum_j a_j^2}}a_i$$  
Normalization in LLM can put after the residual connection (post-LN, order of multihead attention-residual connection-LN) or before attention (pre-LN, order of LN-multihead attention-residual connection). Pre-LN is known to be more stable but will cause earlier layers to have larger gradients. To mitigate this, DeepNorm (arXiv:2203.00555) proposed the approach
$$ \hat{x} = LN(\alpha x + G(x,\theta))$$
where $G(x,\theta)$ is the attention+FF layer with parameter $\theta$, $\alpha$ is a constant.

### Data Preprocessing

The training data for LLM are filtered by (1) a classifier such that only high-quality text is selected (2) heuristics-based rules based on language, metrics, statistics, and keywords.

Training data are also deduplicated at multiple levels, from sentences to documents, and removing personal information using heuristics.

### Architecture

Transformers are designed as sequence transduction models for machine translation. The original architecture is an encoder-decoder.
- encoder output = context vector
- context vector passed to the decoder to predict tokens

Objective of an LLM is to predict the next token based on the input sequence. The *causal decoder* architecture restricts the flow of information backward, i.e., predicting token $\hat{t}_k$ using only the tokens $t_{1:k-1}$. This is the most widely used variant of LLMs.

Another decoder-only architecture is *prefix decoder*, which the sequence is always fully visible on a portion of the input (usually the first few tokens of a sequence). The prefix decoder is non-causal.

### Pretraining Objectives

- Autoregressive LM objective: Predict the future tokens given the previous tokens. Loss is computed throughout the sequence.
- Prefix language modeling: A prefix is chosen randomly and only remaining target tokens are used to calculate the loss
- Masked language modeling: Tokens or spans of consecutive tokens are masked randomly and the model is asked to predict the masked given the past and future context
- Unified language modeling: A combination of above

### Model Adaption

Alignment tuning: To align the model to human preferences

Pretraining: The first stage in building LLM. The model is trained in a self-supervised manner on a large corpus to predict the next tokens given the input.

Fine-tuning: There are multiple adaptations:
- Transfer learning approach to fit pretrained models to task-specific data, such as adding a FC layer on top of the pretrained model output
- Instruction-tuning to fine-tune on instruction formatted data (instruction, then input-output pairs). The instructions generally comprise multitask data in natural language. The model is expected to respond according to the prompt and input
- Alignment tuning is to update LLM's parameter to make it generate helpful, honest, and harmless output (HHH). This is usually performed with reinforcement learning with human feedback (RLHF)

Parameter-efficient tuning: Train LLMs with fewer resources. Multiple approaches are used
- Prompt tuning: Adds trainable prompt token embeddings as prefix to input, and train these embedding parameters for downstream tasks
- Prefix tuning: Adds task-specific trainable prefix vectors to the transformer layers, where only these prefix are fine-tuned. The input sequence tokens is then attended to the prefixes as virtual tokens.
- Adapter tuning: Add an encoder-decoder architecture sequential or parallel to the attention and FF layers in the transformer block.

Prompt engineering: In-context learning is also known as few-shot learning.

Reasoning: If LLM has been trained on reasoning datasets, it can generate reasons using a prompting style. Techniques of reasoning include chain-of-thought (CoT), which the model generates outcomes with step-by-step reasoning; tree-of-thought (ToT), which the model explores multiple reasoning paths with look ahead and backtrack, and self-consistency (arXiv:2203.11171) which generates multiple responses and select the most frequent answer.

## Large Language Models

### Pre-trained LLMs

T5: an encoder-decoder model for text-to-text NLP problems. Train with masked language modeling objective but the masked span is replaced with a single mask token. The model can be fine-tuned using adapter layers for downstream tasks.

GPT-3: same architecture as GPT-2 but with dense and sparse attention. It can train on larger batch size with a lower learning rate. GPT-3 has a model parameters of 175B.

mT5: A multilingual T5 model trained on mC4 dataset with 101 languages. The vocab size is 250K. It is suggested to use a small amount of pretraining datasets to include all languages together with English data for fine-tuning.

Gopher: A family of models from 44M to 280B parameters to study the effect of scale on LLM performance.

GPT-NeoX-20B: A model follows GPT-3 that is trained on Pile dataset without deduplication. it has parallel attention and FC layers in a transformer block, uses rotary positional encoding (RoPE), and only dense attention.

OPT: as a clone of GPT-3, which training employs dynamic loss scaling and restarts from earlier checkpoint with lower learning rate whenever loss divergence is observed.

BLOOM: Causal decoder model trained on the ROOTS corpus. It uses ALiBi positional encoding and adds a normalization layer after the embedding layer. These changes are found to stabilize training.

Chinchilla: Causal decoder trained on the same dataset as Gopher. Using AdamW optimizer. Chinchilla found the relationship that model size should double for every doubling of training tokens.

PaLM: Causal decoder with parallel attention and FC layers. It uses SwiGLU activation, and RoPE. If loss is spiked during training, it restart from 100 steps earlier and skips 200-500 batches of training data. PaLM-2 is a smaller multi-lingual variant of it, but trained on a better quality dataset for more iterations.

LLaMA: Decoder-only language models, famous for parameter-efficient and instruction tuning. LLaMA-1 implements casual attention by not storing and computing masked attention weights and key/query scores. LLaMA-2 has a chat model for dialog generation

CodeGen: Trained on both natural language and programming language data, in the sequence of Pile, BigQuery, and BigPython datasets. It also proposed a multi-step approach to synthesizing code. There is a multi-turn programming benchmark (MTPB) to evaluate multi-step program synthesis.

CodeT5+: Modified CodeT5 with shallow encoder and deep decoder to train on multiple stages, first on code data, then on text-code pairs.

StarCoder: Decoder-only model with Flash Attention to scale up the context length to 8K. It outperforms PaLM, LLaMA on HumanEval and MBPP benchmarks.

LaMDA: Decoder-only model pretrained on public dialog data. It produces responses with high quality, safety, and groundedness.

### Fine-tuned LLMs

T0, mT0: Use templates to convert existing dataset into prompt datasets then trained the model. 

Tk-Instruct: Fine-turned T5 with in-context instructions

OPT-IML, Flan: trained with task datasets

Flan-T5: Trained on 1.88M CoT samples

Techniques in fine-tuning:
- concatenating fine-tuning data with a few random pretraining samples in every iteration avoids network forgetting
- LLMs have limited context window due to expensive attention and memory consumption. Using ALiBi positional encoding can perform zero-shot length extrapolation
- Using efficient attention variants (local, sparse, dilated attentions) can reduce computation cost significantly


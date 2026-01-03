---
layout: post
title: "Zhang et al (2024) TinyLlama"
date: 2026-01-03
category: paper
bibtex:
  type: unpublished
  title: "TinyLlama: An Open-Source Small Language Model"
  author: "Peiyuan Zhang and Guangtao Zeng and Tianduo Wang and Wei Lu"
  month: June
  year: 2024
  arXiv: 2401.02385
  url: https://github.com/jzhang38/TinyLlama
---

This paper is a report on training the TinyLlama model, following the Llama 2 architecture but with 1.1B parameters.
The objective is to experiment with the idea that a smaller model but trained with more data can match or overperform a larger model.

The trained model are available on Hugging Face as:

- <https://huggingface.co/TinyLlama>
- <https://huggingface.co/TinyLlama/TinyLlama_v1.1>
- <https://huggingface.co/TinyLlama/TinyLlama-1.1B-Chat-v1.0>

And the source code is available on GitHub as: <https://github.com/jzhang38/TinyLlama>.

Hoffmann et al proposed the scaling law for language models and many other models are trained using this *compute-optimal* strategy. Touvron et al expects a model over-trained can achieve better performance. This paper experimentally found that this is true, by comparing the TinyLlama model with OPT-1.3B and Pythia-1.4B model in the downstream tasks (InstructEval benchmark).

## Architecture and Pre-training

The architecture of TinyLlama follows the Llama 2 architecture. Precisely, the [configuration](https://github.com/jzhang38/TinyLlama/blob/main/lit_gpt/config.py#L304) is as follows:

- Hidden size: 2048
- Intermediate size: 5632 (2.75x)
- Context length: 2048, training is always on maximum context length
- Attention heads: 32 (GQA with 4 groups, KV heads = 8)
- Layers: 22
- Vocab size: 32000, Llama tokenizer is used

To make the model training faster, lit-gpt from Lightning-AI is used to enable FSDP. Also FlashAttention is used for optimized attention, fused layernorm, fused cross entropy loss, and fused RoPE. Additionally, xFormer is used for fused SwiGLU.

The dataset for pretraining is predomintantly from [SlimPajama](https://www.cerebras.ai/blog/slimpajama-a-627b-token-cleaned-and-deduplicated-version-of-redpajama). It is derived from [RedPajama](https://github.com/togethercomputer/RedPajama-Data) (1.2T tokens) with additional cleaning, removal of low-quality content, and deduplication. Resulting in 50% of it, 627B tokens. It is available on Hugging Face as [cerebras/SlimPajama-627B](https://huggingface.co/datasets/cerebras/SlimPajama-627B).

Additional datasets are used to train variants of the model, to make up the total of 1T tokens from all datasets for each trained model:

- StarCoder: Code data in 86 programming languages, GitHub issues and text-code pairs that involve natural languages. Since SlimPajama also contains GitHub data, when StarCoder is used, SlimPajama will remove the GitHub subset.
- Proofpile: Math and science dataset. Not sure where it is sourced. Maybe [hoskinson-center/proof-pile](https://huggingface.co/datasets/hoskinson-center/proof-pile) or [EleutherAI/proof-pile-2](https://huggingface.co/datasets/EleutherAI/proof-pile-2). This codebase maybe related: <https://github.com/zhangir-azerbayev/proof-pile>
- Skypile: Chinese dataset

It is trained on 16 nodes of 4x A100-40G GPUs, using FSDP. To reduce overhead, FSDP only shards the model within a node. The training is run on batch size of 1.8M tokens for 3 epochs (3T tokens) in TinyLlama v1.0.

The training of TinyLlama v1.1 is revised and has a three-stage process, inspired by Wei et al (2023):

**Stage 1: Basic Pretraining**

It is using lit-gpt autoregressive objective in pretraining. AdamW with $\beta_1 = 0.9$, $\beta_2 = 0.95$, and weight decay of 0.1 is used. Cosine learning rate schedule with max learning rate $4\times 10^{-4}$ and min learning rate $4\times 10^{-5}$, with 2000 warmup steps. Gradient clipping is set to max norm of 1.0. Batch size is 1.8M tokens.

This stage trains for 1.5T tokens from SlimPajama. The objective is to develop common sense reasoning and set the foundation for more specialized training in the next stage.

**Stage 2: Continual Pre-training**

There are three variants of the model for three different domains:

- TinyLlama
- TinyLlama Math&Code
- TinyLlama Chinese

The plain model continue to use SlimPajama in this stage. The Math&Code model will introduce StarCoder (Python and Jupyter splits only), and Proofpile. The Chinese model will introduce Skypile. The additional data is to provide diversity with distinct corpora.

For the Math&Code and Chinese models, in order to make gradual adjustment of data and smoother, more stable training, the data are mixed with SlimPajama with linearly increasing ratios for 6B tokens until the target mix ratio is reached. The mix ratio in Math&Code at stability is 75% SlimPajama, 15% Starcoder, 10% Proofpile. The mix ratio in Chinese at stability is 50% SlimPajama, 50% Skypile.

Additional 350B tokens are trained in this stage.

**Stage 3: Cool Down Phase**

In this stage, the same training as in stage 2 is continued, but with the batch size increased to 4x of the original (1.8M → 7.2M tokens).

The cool down phase is inspired by MiniCPM (Hu et al, 2024) and DeepSeek LLM (Bi et al, 2024), in which the learning rate is decreased. But since cosine scheduler already set a low learning rate, TinyLlama set a larger batch size instead to create the same effect.

In this stage, additional 150B tokens are trained. Make up the total of 2T training tokens.

Additional parameters from the [codebase](https://github.com/jzhang38/TinyLlama) are as follows:

- global batch size 512, micro batch size 8
- gradient accumulation steps = global batch size / num devices / micro batch size
- max step to train 715256*2 (this will be 3T tokens at 1431K steps)
- log step interval 10
- save step interval 5000
- eval step interval 1000, eval iters 100
- always train on max seq length

The paper reported that the training achieved 24K tokens per second per A100-40G GPU. This speed translates to 3456 GPU hours to train 300B tokens. Compare to Pythia-1.0B uses 4830 GPU hours, and MPT-1.3B uses 7920 GPU hours for 300B tokens. According to <https://huggingface.co/TinyLlama/TinyLlama-1.1B-Chat-v1.0>, the training is completed in 90 days.

## Fine-tuning

Fine-tuning details are not reported in the paper. But the [model card on Hugging Face](https://huggingface.co/TinyLlama/TinyLlama-1.1B-Chat-v1.0) gives some information.

The fine-tuning follows [Zephyr-7B-α](https://huggingface.co/HuggingFaceH4/zephyr-7b-alpha) recipe:

- First on [UltraChat](https://huggingface.co/datasets/stingning/ultrachat) dataset, which is a diverse synthetic dialog generated by ChatGPT
- Then [aligned](https://github.com/huggingface/alignment-handbook) with [UltraFeedback](https://huggingface.co/datasets/openbmb/UltraFeedback) dataset, with 64K samples

This is to create a chat model. It is trained with effective batch size of 32. Adam optimizer is used with learning rate $5\times 10^{-7}$, $\beta_1=0.9$, $\beta_2=0.999$ and $\epsilon=10^{-8}$. Linear LR scheduler with warm up ratio 0.1. It is trained with only 1 epoch.


## Evaluation

The paper reported two evaluation results:

- Reasoning: HellaSwag, OpenBookQA, WinoGrande, ARC-Easy, ARC-Challenge, BoolQ, PIQA
  - Using Language Model Evaluation Harness framework for evaluation, <https://github.com/EleutherAI/lm-evaluation-harness>
- Problem-solving tasks: MMLU, BIG-Bench Hard, Discrete Reasoning Over Paragraphs (DROP), HumanEval
  - Using InstructEval benchmark, <https://github.com/declare-lab/instruct-eval>

It is found that TinyLlama v1.1 (multiple variants) outperforms OPT-1.3B, Pythia-1.0B, and Pythia-1.4B on most of the tasks. Exception is on BoolQ, where Pythia-1.4B is the best and TinyLlama is generally worse.


## References

Hoffmann, J., Borgeaud, S., Mensch, A., Buchatskaya, E., Cai, T., Rutherford, E., de las Casas, D.,
Hendricks, L. A., Welbl, J., Clark, A., Hennigan, T., Noland, E., Millican, K., van den Driessche,
G., Damoc, B., Guy, A., Osindero, S., Simonyan, K., Elsen, E., Vinyals, O., Rae, J. W., and Sifre,
L. (2022). Training compute-optimal large language models. In Proceedings of NeurIPS.

Touvron, H., Lavril, T., Izacard, G., Martinet, X., Lachaux, M.-A., Lacroix, T., Rozière, B., Goyal,
N., Hambro, E., Azhar, F., et al. (2023a). Llama: Open and efficient foundation language models.
arXiv preprint arXiv:2302.13971.

Wei, T., Zhao, L., Zhang, L., Zhu, B., Wang, L., Yang, H., Li, B., Cheng, C., Lü, W., Hu, R., et al.
(2023). Skywork: A more open bilingual foundation model. arXiv preprint arXiv:2310.19341.

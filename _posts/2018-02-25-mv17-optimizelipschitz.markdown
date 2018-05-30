---
layout: post
title: Malherbe & Vayatis (2017) Global optimization of Lipschitz functions
date: 2018-02-25
category: paper
bibtex:
  type: inproceedings
  title: Global Optimization of Lipschitz Functions
  author: Cedric Malherbe and Nicolas Vayatis
  year: 2017
  booktitle: Proceedings of the 34th International Conference on Machine Learning
  address: Sydney, Australia
---

A paper on optimization of a Lipschitz continuous function. Algorithms
provided. Useful if the evaluation of function is expensive.

Definition of Lipschitz continuity: Function $$f:X \mapsto Y$$ is Lipschitz
continuous if

$$ d_Y(f(x_1),f(x_2)) \le k d_X(x_1,x_2) \quad \forall x_1,x_2\in X,\ \exists k\ge 0 $$

where $$d_X$$ and $$d_Y$$ are metric on $$X$$ and $$Y$$ respectively. For example,
$$f:\mathbb{R}^2\mapsto\mathbb{R}$$ is Lipschitz continuous if

$$ ||f(x_1)-f(x_2)||_2 \le k ||x_1-x_2||_2 $$

## Set up

- $$f: \mathcal{X}\mapsto\mathbb{R}$$ defined on a compact convex set
  $$\mathcal{X}\subset\mathbb{R}^d$$
- Global optimization $$X^{\ast} \in \arg\max_{X\in\mathcal{X}} f(X)$$
- Search algorithm: produces sequence $$(X_1,f(X_1)), \ldots, (X_t,f(X_t))$$ and
  selection of $$X_{t+1}$$ depends only on $$(X_i,f(X_i))\ \forall i\le t$$
- Performance metric of algorithm: speed of convergence
  $$\max_{X\in\mathcal{X}} f(X) - \max_{i=1,\ldots,t} f(X_i)$$
- Function has finite Lipschitz constant:

$$ \exists k\ge 0\quad\textrm{s.t.}\quad |f(x)-f(x')| \le k ||x-x'||_2\quad\forall x,x' \in \mathcal{X}$$

## Notations

- $$X = (x_1, x_2, \ldots, x_d) \in \mathbb{R}^d$$, i.e. dimension $$d$$
- Standard $$L_2$$-norm is used: \\
  $$||X||_2^2 = \sum_{i=1}^d x_i^2$$
- Ball centered at $$X$$ with radius $$r$$: \\
  $$B(X,r) = \{X'\in\mathbb{R}^d: ||X-X'||_2 \le r\}$$
- Inner radius of $$\mathcal{X}\subset \mathbb{R}^d$$: \\
  $$\mathrm{rad}(\mathcal{X}) = \max\{r>0:B(x,r)\subseteq\mathcal{X}\quad\exists x\in\mathcal{X}\}$$
- Diameter of $$\mathcal{X}\subset\mathbb{R}^d$$: \\
  $$\mathrm{diam}(\mathcal{X}) = \max_{x,x'\in\mathcal{X}} ||x-x'||_2$$
- Volume of $$\mathcal{X}$$ defined by by Lebesgue measure 
- Set of $$k$$-Lipschitz functions defined on $$\mathcal{X}$$: \\
  $$\mathrm{Lip}(k) = \{f: \mathcal{X}\mapsto\mathbb{R}\quad\textrm{s.t.}\quad |f(x)-f(x')|\le k||x-x'||_2\quad\forall x,x'\in\mathcal{X}\}$$
- Set of all Lipschitz continusous functions: $$\bigcup_{k\ge 0}\textrm{Lip}(k)$$
- Uniform distribution: $$\mathcal{U}(\mathcal{X})$$

## Definitions and propositions

Optimization consistency of algorithm $$A$$, over set of functions $$\mathcal{F}$$,
which generates $$X_1, X_2, \ldots, X_n$$:

$$\forall f\in\mathcal{F}\quad\max_{i=1,\ldots,n} f(X_i) \to \max_{X\in\mathcal{X}} f(X) $$

Necessary and sufficient condition for consistency over the set of Lipschitz
functions (also guarantee for global maximizer):

$$\forall f\in\bigcup_{k\ge 0}\textrm{Lip}(k)\quad \sup_{X\in\mathcal{X}}\min_{i=1,\ldots,n} ||X_i-X||_2 \to 0$$

Convergence of pure random search: With probability no less than $$1-\delta$$,

$$\begin{align}
\sup_{X\in\mathcal{X}} \min_{i=1,\ldots,n} ||X_i-X||_2
  &\le \textrm{diam}(\mathcal{X})\cdot\left(\frac{\log(n/\delta) + d\log d}{n}\right)^{1/d} \\
  &= \Omega(n^{-1/d})
\end{align}$$

Minimax rate of algorithm $$A$$ over $$k$$-Lipschitz function $$f$$ according to Bull
(2011): $$\forall n\in\mathbb{N}$$,

$$c_1kn^{-1/d}
\le \inf_{A}\sup_{f\in\textrm{Lip}(k)} \mathbb{E}[\max_{X\in\mathcal{X}} f(X) - \max_{i=1,\ldots,n}f(X_i)]
\le c_2kn^{-1/d}$$

where

$$\begin{align}
c_1 &= \frac{1}{8\sqrt{d}}\mathrm{rad}(\mathcal{X}) \\
c_2 &= \mathrm{diam}(\mathcal{X})\cdot d!
\end{align}$$

and $$\mathbb{E}[\ ]$$ is expectation over the $$n$$ evaluation points $$X_1,\ldots,X_n$$. This suggests the convergence rate at $$\Theta(n^{-1/d})$$.

## Algorithms

LIPO algorithm: for function with Lipschitz constant $$k$$ is known

* Input: $$n\in\mathbb{N}, k\ge 0, \mathcal{X}\subset\mathbb{R}^d, f\in\mathrm{Lip}(k)$$
* Procedure
  1. Let $$X_1 \sim \mathcal{U}(\mathcal{X})$$
  2. Set $$t:=1$$, evaluate $$f(X_1)$$
  3. While $$t < n$$:
     1. Let $$X_{t+1} \sim \mathcal{U}(\mathcal{X})$$
     2. If
        $$\min_{i=1,\ldots,t}\left(f(X_i)+k||X_{t+1}-X_i||_2\right) \ge \max_{i=1,\ldots,t} f(X_i)$$
        1. Evaluate $$f(X_{t+1})$$
        2. Update $$t := t+1$$
* Output: $$X_i,\ i\in\arg\max_{i=1,\ldots,n} f(X_i)$$

The if-statement inside while loop is the decision rule to accept only those
$$X_{t+1}$$ that has the potential to maximize $$f$$. The RHS is the global maximum
encountered so far and LHS searches for the Lipschitz bound. It is provided as
a lemma in the paper that $$X_{t+1}$$ is a maximizer iff the inequality satisfies.

Adaptive LIPO for unknow Lipschitz constant:

* Input: $$n\in\mathbb{N}, k_{i\in\mathbb{Z}}, \mathcal{X}\subset\mathbb{R}^d, f\in\bigcup_{k\ge 0}\mathrm{Lip}(k)$$
* Procedure
  1. Let $$X_1 \sim \mathcal{U}(\mathcal{X})$$
  2. Set $$k_1:=0$$ and $$t:=1$$, evaluate $$f(X_1)$$
  3. While $$t < n$$:
     1. Draw a Bernoulli random variable $$B_{t+1}\in\{0,1\}$$ with probability $$p$$
     2. If $$B_{t+1} = 1$$, it is the exploration, $$X_{t+1}\sim\mathcal{U}(\mathcal{X})$$
     3. If $$B_{t+1} = 0$$, it is the exploitation, $$X_{t+1}\sim\mathcal{U}(\mathcal{X}_{k_t,t})$$
     4. Evaluate $$f(X_{t+1})$$
     6. Update $$t := t+1$$
     5. Update
        $$k_t = \inf\{k_{i\in\mathbb{Z}}:\max_{i\neq j} \frac{|f(X_i)-f(X_j)|}{||X_i-X_j||_2} \le k_i\}$$
* Output: $$X_i,\ i\in\arg\max_{i=1,\ldots,n} f(X_i)$$

The Lipschitz constant estimate at each step is $$k_t$$. The set
$$\mathcal{X}_{k,t}$$ is the set of potential maximizer, i.e., those
$$x\in\mathcal{X}$$ that statisfied the inequality above, i.e.,

$$\min_{i=1,\ldots,t}\left(f(X_i)+k||X_{t+1}-X_i||_2\right) \ge \max_{i=1,\ldots,t} f(X_i)$$

## Note

A naive implementation would not be any faster than Monte Carlo search if the
inequality has to be evaluated every time. A more efficient approach would be to
modify the random function to draw exactly those $$X\in\mathcal{X}_{k,t}$$. This
would be depend on shape of the function domain.

([PDF](https://drive.google.com/open?id=1jgQAD7OLggRPBJM_euXTdXd6V65USfof))

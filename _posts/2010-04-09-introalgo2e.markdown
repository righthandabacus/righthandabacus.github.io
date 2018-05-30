---
layout: post
title: "Introduction to Algorithms 2/e"
subtitle: "Cormen et al / 2002"
date: 2010-04-09 17:51:34 -0400
category: book
bibtex:
  type: book
  title: Introduction to Algorithms
  edition: 2nd
  author: Thomas H. Cormen and Charles E. Leiserson and Ronald L. Rivest and Clifford Stein
  publisher: MIT Press
  year: 2002
---

Summary of selected chapters:

# Chapter 3
  - $$f(n)=\Theta(g(n))$$ is a sandwich bound ( = )
  - $$f(n)=O(g(n))$$ is a tight upperbound ( ≤ )
  - $$f(n)=\Omega(g(n))$$ is a tight lowerbound ( ≥ )
  - $$f(n)=o(g(n))$$ is a loose upperbound ( $$<$$ )
  - $$f(n)=\omega(g(n))$$ is a loose lowerbound ( $$>$$ )

e.g. $$2n^2 = O(n^2)$$ but $$2n^2 \neq o(n^2)$$, $$n=o(n^2)$$ and $$n=O(n^2)$$, $$f(n)=\omega(g(n))\;\implies\;\Omega(g(n))$$

# Chapter 4
How to prove an recurrence formula?

  - Substitution method: Guess the anser and proof it by mathematical induction
  - Recursion tree: Build a tree with the nodes denoting the fixed cost and children denotes
    recurred functions. Then sum up all nodes after the tree is complete
  - Using master theorem: For solving $$T(n)=aT(n/b)+f(n)$$

Master theorem: Let $$T(n)=aT(n/b)+f(n)$$ where $$a\ge 1$$ and $$b>1$$. The term $$n/b$$ in the formula can also be $$\textrm{ceil}(n/b)$$ and $$\textrm{floor}(n/b)$$. $$T(n)$$ is defined on non-negative integers. Then,

  - If $$f(n)=O(n^{\log_b a-\epsilon})\ \exists\epsilon>0$$ then
     $$T(n)=\Theta(n^{\log_b a})$$
  - If $$f(n)=\Theta(n^{\log_b a})$$ then
     $$T(n)=O(n^{\log_b a}\log_2 n)$$
  - If $$f(n)=\Omega(n^{\log_b a+\epsilon})\ \exists\epsilon>0$$ and
     $$af(n/b)\le cf(n)\ \exists c<1\ \forall n>n_0$$ then
     $$T(n)=\Theta(f(n))$$

Interpretation of the master theorem: The recursion $$T(n)=aT(n/b)$$ gives $$\Theta(n^{\log_b a})$$. Thus if the recursion part dominates, the solution is this. But if the $$f(n)$$ part dominates, the result is as $$f(n)$$. If both parts weigh equally, both appears in the result. The proof is in section 4.4.

# Chapter 5
## Calculating probability using indicator variable

Let $$I_A$$ be an indicator (0 or 1) of event $$A$$ happened. Then the expected value of $$I_A$$ equals to the probability of $$A$$.

Example: To interview n random persion in sequential order, how many times we see the best-candidate-so-far? If we are interviewing the $$k$$-th persion, it is the best so far in probability of $$1/k$$. Defining $$X_k$$ to be the indicator of best-so-far, the count $$N$$ satisfies $$E(N)=\sum_k E(X_k)=\sum_k 1/k = O(\log n)$$. Indeed, $$E(N)=\ln n + O(1)$$.

## Birthday paradox
In $$n$$ bins and $$m$$ balls, each ball falls in a bin randomly. Find the probability that no bin contains ≥2 balls.

For $$m=2$$, the probability is $$1/n$$ and the probability of not is $$1-1/n$$. For $$m=3$$, the probability of not is $$1\times(1-\frac{1}{n})\times(1-\frac{2}{n})$$ (first ball $$\times$$ second ball $$\times$$ third ball)

Thus for general $$m$$ ($$m\le n$$), it is $$\prod_{k=1}^{m-1} (1-\frac{k}{n})$$. In case $$m<<n$$, we have $$e^x = 1+ x + x^2/2! + \cdots \approx 1+x$$ thus $$\prod_{k=1}^{m-1}(1-\frac{k}{n})=\prod_{k=1}^{m-1}e^{-k/n}=\exp(-\frac{m(m-1)}{2n})$$

For probability of $$<0.5$$, what is the marginal $$m$$?  
Solving $$\exp(-\frac{m(m-1)}{2n}) \le \frac{1}{2}$$ gives $$(m-1)m\ge 2n ln2$$, then 

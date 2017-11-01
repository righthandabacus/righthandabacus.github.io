---
layout: post
title: "Stolz-Cesàro theorem"
date: 2012-03-05 13:06:23 -0500
tags: math
---
L'Hôpital's rule: \\
for functions $f$ and $g$ which are differentiable on $I \backslash\{c\}$ ,
where $I$ is an open interval containing $c$, if

$$
\begin{aligned}
& \lim_{x\to c}f(x)=\lim_{x\to c}g(x)=0 \textrm{ or } \pm\infty,   \\
& \lim_{x\to c}\frac{f'(x)}{g'(x)} \textrm{ exists},  \\
& g'(x)\neq 0 \quad\forall x \in I, x\neq c,
\end{aligned}
$$

then

$$
\begin{aligned}
\lim_{x\to c}\frac{f(x)}{g(x)} = \lim_{x\to c}\frac{f'(x)}{g'(x)}. 
\end{aligned}
$$

Stolz-Cesàro theorem: \\
for real-number sequences $$(a_n) _{n \ge 1}$$ and $$(b_n)_{n \ge 1}$$,
assume $ b_n $ is stictly increasing and unbounded, and

$$
\begin{aligned}
\lim_{n\to\infty} \frac{a_{n+1}-a_n}{b_{n+1}-b_n}=\ell,
\end{aligned}
$$

then

$$
\begin{aligned}
\lim_{n \to \infty} \frac{a_n}{b_n} = \ell
\end{aligned}
$$

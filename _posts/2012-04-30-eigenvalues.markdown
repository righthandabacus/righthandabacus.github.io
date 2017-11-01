---
layout: post
title: "Property of Matrix and Its Eigenvalues"
date: 2012-04-30 18:29:36 -0400
tags: math
---

For a matrix $A$, and its eigenvalue $\lambda_i$

  * $A$ and $A^T$ always share the same eigenvalues
  * $A$ and $M^{-1}AM$ always share the same eigenvalues
  * eigenvalues of $A^{-1}$ are $1/\lambda_i$
  * eigenvalues of $A+cI$ are $\lambda_i + c$
  * eigenvectors of $AA^T$ are basis of columns of $A$
  * eigenvectors of $A^TA$ are basis of rows of $A$
  * if $A$ is singular, then some $\lambda_i = 0$
  * if $A$ is symmetric,
      * $\lambda_i$ are all real
      * eigenvectors can be chosen orthonormal
      * number of $\lambda_i > 0$ = number of positive pivots
  * if $A$ is skew-symmetric, then $\lambda_i$ are all imaginary
  * if $A$ is symmetric positive definite, then all $\lambda_i > 0$
  * if $A$ is full rank, then eigenvectors form a basis for $\mathbb{R}^n$
  * if $A$ is real, then eigenvalues and eigenvectors come in conjugate pairs
  * if $A$ is diagonal, then eigenvalues are the diagonal elements
  * if $A$ is orthogonal, then $$|\lambda_i|=1$$ for all $i$
  * if $A$ is a projector matrix (i.e. $A^2=A$), then $\lambda_i$ is either 0 or 1
  * if $A$ is a Markov/stochastic matrix, $\max_i \lambda_i = 1$ (which the corresponding eigenvector is the vector of stationary probabilities)

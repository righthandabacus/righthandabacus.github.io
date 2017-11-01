---
layout: post
title: "The Elements of Programming Style"
subtitle: "Brian W. Kernighan and P. J. Plauger / 1974"
date: 2010-09-11 21:45:34 -0400
category: book
bibtex:
  type: book
  title: The Elements of Programming Style
  author: Brian W. Kernighan and P. J. Plauger
  publish: McGraw-Hill
  year: 1974
  classification: QA 76.5 K47
  note: Dibner
---
As said in the preface, this book is mimicking Stunk and White. And also align with Stunk and White, what this book advocates is to make your program as short as possible, so that it can be clear.

However, writing an essay and writing a program is different. Writing an essay is meant to be read. But writing a program is meant to be executed. Reading a program is a by-product (although sometimes important). Therefore, there is an intrinsic dilemma in optimizing the clarity (for human) and code execution (for machine).

Besides this book is based on Fortran (which makes some of the suggestions in the book obsolete now), there are some useful suggestions, listed below:

  - Write clearly --- don't be too clever
  - Say what you mean, simply and directly
  - Use library functions (to shorten your code for clarity)
  - Don't sacrifice clarity for efficiency
  - Let the machine (compiler) to do the dirty work (and optimizations)
  - Avoid unnecessary branches
  - Don't use conditional branches as substitute for logical expression
  - Don't patch bad code --- rewrite it
  - Test input for plausibility and validity
  - Watch out for off-by-one errors
  - Don't compare floating point numbers solely for equality
  - Make it right, fail-safe, and clear before you make it faster
  - Don't strain to re-use code; reorganize instead
  - Keep it simple to make it faster

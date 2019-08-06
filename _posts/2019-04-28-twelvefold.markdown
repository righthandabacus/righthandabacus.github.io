---
layout: post
title: Twelvefold of combinatorics
date: Sun, 28 Apr 2019 23:31:58 -0400
tags: math
---

Recently I come across the term "[Twelvefold
way](https://en.wikipedia.org/wiki/Twelvefold_way)" of combinatorics. I wish I
knew the term earlier as I studied quite many of those in the graduate schools
and collected "recipes" for them. I always thought of them as some cases of
[urn models](https://en.wikipedia.org/wiki/P%C3%B3lya_urn_model) of George
Pólya but they indeed have a neat classification. I try to reiterate the twelve
and derive the solution below.

## Terms

Generically, the twelvefold is modeling the count of possible mappings between
two finite sets, $$N$$ and $$X$$, which each has cardinality of $$n$$ and $$x$$
respectively.  The mapping is $$f: N\mapsto X$$ and the property of the mapping
identifies the twelve cases. First the nature of mapping:

1. No condition, simply $$\forall a\in N,\ \exists b\in X$$
2. Injective $$f$$, so $$f(a)$$ is unique: $$\forall a\in N,\ \exists !b\in X$$
3. Surjective $$f$$: $$\forall b\in X,\ \exists a\in N, f(a)=b$$

Then the equivalence of mappings:

1. $$f$$, simple
2. $$f\circ S_n$$, equality up to a permutation of $$N$$
3. $$S_x \circ f$$, equality up to a permutation of $$X$$
4. $$S_x \circ f \circ S_n$$, equality up to a permutation of $$N$$ and $$X$$

If we visualize the above using a ball and urn model, we have a finite set of
balls $$N$$ and a finite set of urns $$X$$, and we are counting how many ways
are can put the balls into the urns under different definition. The three
natures of mapping means:

1. No condition as long as we put all balls into urns
2. Injective: All urns contain no more than one ball
3. Surjective: All urns contain at least one ball, i.e., no urn can leave empty

and the four equivalence of mapping means:

1. $$f$$: All balls and urns are labelled, so everything is distinguishable from each other ($$f: \textrm{labelled }N\mapsto\textrm{labelled }X$$)
2. $$f\circ S_n$$: All urns are labelled, but the balls are identical ($$f: \textrm{unlabelled }N\mapsto\textrm{labelled }X$$)
3. $$S_x \circ f$$: All balls are labelled, but urns are indistinguishable from each other ($$f: \textrm{labelled }N\mapsto\textrm{unlabelled }X$$)
4. $$S_x \circ f \circ S_n$$: Neither balls nor urns are labelled ($$f: \textrm{unlabelled }N\mapsto\textrm{unlabelled }X$$)

Twelvefold way gets its name from the $$3\times 4=12$$ different possible
models. We can try to make up problems, as follows.

## (1) Unrestricted f maps labelled N to labelled X

Problem: How many different ways to put $$n$$ distinct balls into $$x$$
distinct urns?

Solution: Each of the $$n$$ balls can put into one of the $$x$$ urns, so there
are $$x^n$$ ways

## (2) Unrestricted f maps unlabelled N to labelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$ urns?

Solution: We can rephrase the problem as: "If we line up $$n$$ balls, how many
ways can we put $$x-1$$ separators between the balls". Then, from left to
right, the separators partitioned the sequence of (identical) balls into $$x$$
groups. Because we do allow urns to be empty, more than one separators can be
put between two balls. It is like in a sequence of $$n+x-1$$ spaces, we put
$$n$$ balls and $$x-1$$ separators. For example, the following is one result of $$n=6,
x=4$$:

    O O | | O | O O O

So the number of combinations is

$$ \binom{n+x-1}{x-1} = \binom{n+x-1}{n} $$

## (3) Unrestricted f maps labelled N to labelled X

Problem: How many ways to put $$n$$ distinct balls into $$x$$ identical urns?

Solution: Problem (11) is similar to this problem except with an extra
condition that no urn can leave empty. The solution of problem (11) is
$$\genfrac{\{}{\}}{0pt}{}{n}{x}$$. So for this problem, if we count only the
non-empty urns, there can be 1, 2, up to $$x$$ urns (unless $$n=0$$, in which
case all urns will be empty). So the total count of possible ways is

$$\sum_{k=0}^x \genfrac{\{}{\}}{0pt}{}{n}{k}$$

Note that the above summation should have only up to $$\min(n,x)$$ terms as we
can have at most $$n$$ urns non-empty if $$x>n$$. But in those cases, we always
have $$\genfrac{\{}{\}}{0pt}{}{n}{x} = 0$$.

## (4) Unrestricted f maps unlabelled N to unlabelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$ indistinguishable urns?

Solution: If we further requires the urns to be non-empty, this becomes problem
(12) and it is called the partitioning problem, which means how many ways we
can break down integer $$n$$ into sum of smaller positive integers. Let us
denote the count of partition problem to be $$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x}$$.

Using similar reasoning as in problem (3), if we relaxed the condition to allow
urns to be empty, unless $$n=0$$, we can have 1, 2, up to $$x$$ non-empty urns.
So the answer is

$$\sum_{k=0}^x \genfrac{\lvert}{\rvert}{0pt}{}{n}{k}$$

which similarly, the above summation should have only up to $$\min(n,x)$$ terms
because if $$x>n$$, all $$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x} = 0$$. But
indeed, using the recurrence relation explained in problem (12),

$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x} = \genfrac{\lvert}{\rvert}{0pt}{}{n-1}{x-1} + \genfrac{\lvert}{\rvert}{0pt}{}{n-x}{x}$$

we can derive that:

$$\begin{align}
\genfrac{\lvert}{\rvert}{0pt}{}{n+x}{x}
&= \genfrac{\lvert}{\rvert}{0pt}{}{n+x-1}{x-1} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x} \\
&= \genfrac{\lvert}{\rvert}{0pt}{}{n+x-2}{x-2} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x-1} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x} \\
&= \genfrac{\lvert}{\rvert}{0pt}{}{n+x-3}{x-3} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x-2} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x-1} + \genfrac{\lvert}{\rvert}{0pt}{}{n}{x} \\
&= \cdots \\
&= \genfrac{\lvert}{\rvert}{0pt}{}{n+x-x}{x-x} + \sum_{k=1}^x \genfrac{\lvert}{\rvert}{0pt}{}{n}{k} \\
&= \sum_{k=0}^x \genfrac{\lvert}{\rvert}{0pt}{}{n}{k}
\end{align}$$

So we can avoid the summation and find the answer to this problem to be

$$\sum_{k=0}^x \genfrac{\lvert}{\rvert}{0pt}{}{n}{k} = \genfrac{\lvert}{\rvert}{0pt}{}{n+x}{x}$$

## (5) Injective f maps labelled N to labelled X

Problem: How many ways to put $$n$$ balls into $$x$$ urns such that no urn can
hold more than one ball?

Solution: Because of the restriction that no urn can have more than one ball,
it makes sense only for $$x\ge n$$. When we place the first ball, we pick one
of the $$x$$ urns. Then the second ball, we pick one of the $$x-1$$ remaining
urns, and so on. So the number of ways is:

$$\begin{align}
& x(x-1)(x-2)\cdots(x-n+1) \\
=& x^{\underline{n}} \\
=& \frac{x!}{(x-n)!}
\end{align}$$

## (6) Injective f maps unlabelled N to labelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$ urns such that
no urn can hold more than one ball?

Solution: Again, this makes sense only for $$x\ge n$$. As the result is only
$$n$$ out of $$x$$ urns can have a ball, so it is a combination problem with
solution

$$\binom{x}{n}$$

## (7) Injective f maps labelled N to unlabelled X

Problem: How many ways to put $$n$$ distinct balls into $$x$$ indistinguishable
urns such that no urn can hold more than one ball?

Solution: Assume $$x\ge n$$. As the urns are indistinguishable, we will find
$$n$$ urns with balls labelled as 1, 2, up to $$n$$ and the remaining urns (if
any) are empty. So there will only be one way. The general answer is either 1
or 0, depends on whether $$x\ge n$$. Using the Iverson bracket notation:

$$[x\ge n] := \begin{cases}
1 & \textrm{if }x\ge n \\
0 & \textrm{otherwise}
\end{cases}$$

## (8) Injective f maps unlabelled N to unlabelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$
indistinguishable urns such that no urn can hold more than one ball?

Solution: Indeed this is identical to problem (7) as the balls are labelled or
not does not matter in the reasoning of the solution in that problem. So the
count of ways is $$[x\ge n]$$.

## (9) Surjective f maps labelled N to labelled X

Problem: How many ways to put $$n$$ balls into $$x$$ urns such that no urn is
left empty?

Solution: If the urns are indistinguishable, it becomes problem (11), which has
solution $$\genfrac{\{}{\}}{0pt}{}{n}{x}$$. But if urns are labeled, each
solution in problem (11) correspond to a permutation of urns in this problem,
which $$x$$ urns has $$x!$$ such rearrangement possible. So the solution is

$$x! \genfrac{\{}{\}}{0pt}{}{n}{x}$$

## (10) Surjective f maps unlabelled N to labelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$ urns such that
no urn is left empty?

Solution: If we line up $$n$$ balls, there are $$n-1$$ spaces between two
balls. We pick $$x-1$$ of them to place a separator, then we resulted in a
partition of the $$n$$ balls into $$x$$ groups, each is non-empty. So the
number of ways is

$$\binom{n-1}{x-1}$$

## (11) Surjective f maps labelled N to unlabelled X

Problem: How many ways to put $$n$$ balls into $$x$$ indistinguishable urns
such that no urn is empty?

Solution: This is the simplified problem for problems (3) and (9). We will
denote the answer as

$$\genfrac{\{}{\}}{0pt}{}{n}{x}$$

Which it has the following properties. For the trivial case of $$n=x$$ or
$$x=1$$, there can only be one way so we denote

$$\genfrac{\{}{\}}{0pt}{}{n}{n} = \genfrac{\{}{\}}{0pt}{}{n}{1} = 1$$

also, if $$x > n$$, we can have no solution, thus

$$\genfrac{\{}{\}}{0pt}{}{n}{x} = 0 \quad \textrm{if }x>n>0$$

and for the degenerated cases:

$$\begin{align}
\genfrac{\{}{\}}{0pt}{}{0}{0} = 1 \\
\genfrac{\{}{\}}{0pt}{}{n}{0} = 0 \quad \textrm{if }n\ge 1
\end{align}$$

Then we consider for other cases of $$\genfrac{\{}{\}}{0pt}{}{n}{x}$$: Assume
we already have an answer for the case of $$n-1$$ balls and we are adding the
$$n$$-th ball into the problem. We can have two cases:

1. The $$n$$-th ball is in its own urn and the other $$n-1$$ balls are in
   $$x-1$$ urns
2. The $$n-1$$ balls already scattered across $$x$$ urns and the $$n$$-th ball
   is put into one of the $$x$$ urns

For the latter case, there is $$x$$ possibilities of the $$n$$-th ball. So
these two cases forms the recurrence relation, when $$n>x>1$$:

$$\genfrac{\{}{\}}{0pt}{}{n}{x} = \genfrac{\{}{\}}{0pt}{}{n-1}{x-1} + x \genfrac{\{}{\}}{0pt}{}{n-1}{x}$$

Indeed, this is called the Stirling number of the second kind, which is the
number of ways to partition $$n$$ elements into $$x$$ subsets:

$$\genfrac{\{}{\}}{0pt}{}{n}{x} = \frac{1}{x!}\sum_{k=0}^x (-1)^k\binom{x}{k}(x-k)^n$$

## (12) Surjective f maps unlabelled N to unlabelled X

Problem: How many ways to put $$n$$ identical balls into $$x$$
indistinguishable urns such that no urn is left empty?

Solution: This is the simplified problem of (4) and the solution is the
partition number, which is the number of ways to write integer $$n$$ as sum of
positive integers. We denote such number as

$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x}$$

and it has the following properties. The trivial case that $$x=n$$ and $$x=1$$
has only one way, so

$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{n} = \genfrac{\lvert}{\rvert}{0pt}{}{n}{1} = 1$$

For $$x>n$$, we have no solution, so

$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x} = 0 \quad \textrm{if }x>n>0$$

and for the degenerated cases:

$$\begin{align}
\genfrac{\lvert}{\rvert}{0pt}{}{0}{0} = 1 \\
\genfrac{\lvert}{\rvert}{0pt}{}{n}{0} = 0 \quad \textrm{if }n\ge 1
\end{align}$$

Then we consider for other cases of $$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x}$$:

1. If one urn has one ball, take away that urn is a solution for the case of
   $$n-1$$ balls and $$x-1$$ urns
2. If all urns has at least two balls, take away one ball form each urn is a
   solution for $$n-x$$ balls and $$x$$ urns

So combining the two cases above we have the following recurrence relation,
when $$n>x>1$$:

$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x} = \genfrac{\lvert}{\rvert}{0pt}{}{n-1}{x-1} + x \genfrac{\lvert}{\rvert}{0pt}{}{n-x}{x}$$

## Summary

||f|injective f|surjective f|
|---|---|---|---|
|$$f$$ (LN → LX)|sequence<br/>$$x^n$$|permutation<br/>$$x^{\underline{n}}$$|set partition<br/>$$x!\genfrac{\{}{\}}{0pt}{}{n}{x}$$|
|$$f\circ S_n$$ (UN → LX)|multicombination<br/>$$\binom{n+x-1}{n}$$|combination<br/>$$\binom{x}{n}$$|composition<br/>$$\binom{n-1}{n-x}$$|
|$$S_x \circ f$$ (LN → UX)|set partition<br/>$$\sum_{k=0}^x\genfrac{\{}{\}}{0pt}{}{n}{k}$$|pigeon hole<br/>$$[x\ge n]$$|set partition<br/>$$\genfrac{\{}{\}}{0pt}{}{n}{x}$$|
|$$S_x \circ f\circ S_n$$ (UN → UX)|partitioning<br/>$$\genfrac{\lvert}{\rvert}{0pt}{}{n+x}{x}$$|pigeon hole<br/>$$[x\ge n]$$|partitioning<br/>$$\genfrac{\lvert}{\rvert}{0pt}{}{n}{x}$$|

## Extension

The twelvefold way is attributed to Gian-Carlo Rota (according to Wikipedia)
and the Wikipedia page also mentioned an extension to twentyfold way, which
include the cases that all urn must have exactly one ball (i.e., the mapping
$$f$$ is bijective), or the urn is presented as a list instead of a set, so
there is ordering of the balls in the urn (in which case, [Lah
number](https://en.wikipedia.org/wiki/Lah_number) is involved).

A paper [Let's Expand Rota's Twelvefold Way For Counting
Partitions!](https://arxiv.org/pdf/math/0606404.pdf) also extends the
twelvefold way into thirtyfold.

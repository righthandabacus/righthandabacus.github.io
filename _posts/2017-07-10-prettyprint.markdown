---
layout: post
title: Pretty printing something
subtitle: it is not trivial
date: Mon, 10 Jul 2017 21:21:26 -0400
tags: code
category: blog
---

Pretty-printing is [not trivial][1].

Pretty-printing a piece of code probably can be defined as follows:

1. Line width defined as finite $N$
2. Unless some item is mandatory to begin at new line, a segment of code should
   be in one line if it will fit
3. If a segment of code cannot fit in one line, it should be broken down into
   smaller fragments and aligned at left side appropriately

To realise (3), it is important to identify fragments of equal level, or
equivalently, [group logically contiguous tokens into same fragment][2].

[Oppen (1979)][2] devised the following algorithm:

1. scan function to produce (1) token and its length, or (2) the beginning of a group of
   tokens and the total length, including the spaces added between tokens
   within, or (3) a space between token the size of the space plus length of
   next token. Scan function need to lookahead
2. a buffer stream (FIFO) to store tokens seen. when scan function computed the
   length $\ell$ for token $x$ at the left end of the buffer, it prints and
   removes $x$
3. print function decides how to print a token using the length information:
   (1) string is printed immediately, or (2) beginning of a group prints nothing
   but pushes the current indentation on a stack, or (3) end of a group pops the
   indentation from a stack, or (4) for a blank, check to see if the next block
   can fit in the current line, print a blank if so or skip to next line with
   indentation specified by the top-of-stack plus arbitrary offset.

In developing pretty-printers, it is preferred to avoid back-tracking for
[performance reasons][3].

[Jackson et al (2008)][4] outlines a structure for pretty printing: The abstract
syntax tree (AST) produced by parser should be converted into a layout options tree
(LOT) by layout options generator (LOG).

## Reference:
1. [Bob Boyer (1973) Pretty-print, Memo No 64][1]
2. [Derek C. Oppen (1979) Pretty printing. Stanford University, Stanford Verification Group Report No
   13 / Computer Science Department Report No STAN-CS-79-770][2]
3. [Olaf Chitil (2012) Efficient simple pretty printing combinators][3]
4. [Jackson et al (2008) Stable, flexible, peephole pretty-printing. Elsevier Science][4]
5. [Swierstra and Chitil (2009) Linear, Bounded, Functional Pretty-Printing. J Functional Programming, 19(01):182-196, Jan][5]

[1]: https://www.cs.utexas.edu/~boyer/pretty-print.pdf
[2]: http://i.stanford.edu/pub/cstr/reports/cs/tr/79/770/CS-TR-79-770.pdf
[3]: https://www.cs.kent.ac.uk/people/staff/oc/pretty.html
[4]: http://macbeth.cs.ucdavis.edu/ph-final.pdf
[5]: https://www.cs.kent.ac.uk/pubs/2009/2847/index.html

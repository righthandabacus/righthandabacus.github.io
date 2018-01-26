---
layout: post
title: "LaTeX Package: Fill in Blanks"
date: 2007-03-21 21:34:58 +0800
lastedit: Fri, 26 Jan 2018 20:40:21 -0500
tags: code
---

LaTeX package file [`fillinblank.sty`](/img/fillinblank.sty).

This is created for making notes for the ERG2013 remedial classes. My idea is to
create a set of "fill-in-blank" notes for the students so that they are forced
to attend the course and pay attention to my teaching. How about the solution?
Actually I don't want to prepare two separate set of notes. Hence this is the
tool.

To use it, that's very easy. See the following example:

```tex
\documentclass{article}
\usepackage{amsmath,amssymb}
\usepackage[answers]{fillinblank}

\begin{document}
\title{Example}
\maketitle

This is a \fillinblank{blank} in a sentence.

\begin{equation*}
\int_0^1 \fillinblank{x^2} dx= \frac{1}{3}
\end{equation*}

Try to find the solution of $x^2+2x-3=0$ by completing square.
\answerspace{
\begin{align*}
x^2 + 2x - 3 & = 0   
(x^2 + 2x + 1) - 4 & = 0   
(x+1)^2 & = 4   
x+1 & = \pm 2   
x & = -1 \pm 2   
\therefore\ x & = -1 + 2 = 1   
\textrm{or } x & = -1 - 2 = -3.
\end{align*}
}
\end{document}
```

The package receives an option: `answers` or `noanswers`, which denotes whether
you want the output to be *filled* or *not filled*. The default is `answers` if
not given.

The package provides the following commands:

  * `\showanswers`: From the point onwards, all the blanks are filled, regardless of the option given when loading the package
  * `\noshowanswers`: From the point onwards, all the blanks are not filled, regardless of the option given when loading the package
  * `\fillinspace{}`: Provide a space and then put the items in the braces into the space. It is required that the space is an inline element.
  * `\fillinunderline{}`: Provide an underline area and then put the items in the braces onto the underline area. It is required that the space is an inline element.
  * `\fillinblank{}`: If currently we are in math mode, do `\fillinspace`, otherwise, do `\fillinunderline`
  * `\fillinblank*{}`: If currently we are in math mode, do `\fillinunderline`, otherwise, do `\fillinspace`
  * `\answerspace{}`: The item in the brace is a paragraph element, which means it is a block or a box. Provide a space for this block and fill the item in.

Edit (2018/1/26): There is an `exam` document class that serve a similar
purpose, see <http://www-math.mit.edu/~psh/exam/examdoc.pdf>

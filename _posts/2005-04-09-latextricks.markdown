---
layout: post
title: "LaTeX tricks"
lastedit: 2021-01-27
date: 2005-04-09 02:59:20 +0800
tags: reference
highlightjs:
  - latex
---


# Text, style, and grammar

### Grammar check

Try [Queequeg](http://queequeg.sourceforge.net/index-e.html)

### Prevent hyphenation at once

    \hyphenpenalty=5000
    \tolerance=1000

### Using limit-mode in summation
Force limits to place at above and below (useful for inline equation):

    \sum\limits_{x=1}^{n}\frac{1}{n}


### Equal by Definition

    i^2 \stackrel{\mathrm{def}}{=} -1

### Self-defined function in equation mode
To make foo appears like `\sin` or `\exp`, use

    \operatorname{foo}(x)

or to save space, define the following in preamble:

    \newcommand{\foo}{\operatorname{foo}}

### Modifying counters

Counters available: part, chapter, section, subsection, subsubsection,
paragraph, subparagraph, figure, table, equation, enumi, enumii, enumiii,
enumiv, footnote, mpfootnote

Setting counter value:

    \setcounter{page}{1}

Setting style:

    \part_style{arab}

### Font size

To adjust font size by "zooming", use this:

```latex
\usepackage{scalefnt}

Normal size - \scalefont{2}Linear double - \scalefont{0.5} Normal -
\scalefont{1.414}Double size (area) - \scalefont{0.707} Normal

{\scalefont{2}Double size} - Normal again
```

If the scaling is in terms of "levels", use this:

```latex
\usepackage{relsize}

Normal size - \relsize{1}Linear double - \relsize{-1} Normal -
\relsize{2}Double size (area) - \relsize{-2} Normal

{\relsize{1}Double size} - Normal again
```

### Complicated Matrix

For creating matrix (in math mode) but with part of the stuff outside the
bracket, there are several ways to do. First one is to use Knuth's
`\bordermatrix` macro, like this one (copy from [Cambridge's
web](http://www.eng.cam.ac.uk/help/tpl/textprocessing/latex_maths+pix/node17.html))

```latex
\begin{math}
\bordermatrix{&a_1&a_2&...&a_n\cr
            b_1 & 1.2  & 3.3  & 5.1  & 2.8  \cr
            c_1 & 4.7  & 7.8  & 2.4  & 1.9  \cr
            ... & ...  & ...  & ...  & ...  \cr
            z_1 & 8.0  & 9.9  & 0.9  & 9.99  \cr}
\end{math}
```

The above will have the matrix with only the number and those letters on the top
and left outside the matrix. If you want not the topmost row and leftmost column
outside the bracket, you can use `\bordermatrix*`,

```latex
\begin{math}
\bordermatrix*{
            1.2  & 3.3  & 5.1  & 2.8  & b_1 \cr
            4.7  & 7.8  & 2.4  & 1.9  & c_1 \cr
            ...  & ...  & ...  & ...  & ... \cr
            8.0  & 9.9  & 0.9  & 9.99 & z_1 \cr
            a_1  & a_2  & ...  & a_n  &     \cr}
\end{math}
```
which, in turn, show the rightmost column and the bottom row outside the bracket.

However, if you want a LaTeX version instead of plain TeX, one may use K.
Border's [kbordermatrix package](http://www.hss.caltech.edu/~kcb/LaTeX.shtml).
The documentation is
[here](http://www.hss.caltech.edu/~kcb/TeX/kbordermatrix.pdf).

Furthermore, if you want to show the matrix/determinant operations (i.e. the
arrows showing which row is multiplied by what and add to which row, etc.), you
may found the
[gauss.sty](http://www.tug.org/tex-archive/macros/latex/contrib/gauss/) package
useful.

### Collection of math mode tricks

See the documentation of
[mathmode](http://www.dante.de/CTAN/info/math/voss/mathmode/), written by
Herbert Voss with 130+ pages. Very detail and contains almost everything you
need for typesetting equations.


# Fonts

### Beautiful CM Fonts

To use a much better CM font for LaTeX, get cm-super package (optionally for
X11: cm-super-x11) in Debian. The add the lines in your LaTeX document preamble:

    \usepackage{type1ec}
    \usepackage[T1]{fontenc}

Then your output would include no bitmap CM fonts.

### Times font for everything

The times package can only make you have times font for main text but not for
equations. To make everything including equation to use times font, call this:

    \usepackage{mathptmx}
    \DeclareSymbolFont{largesymbols}{OMX}{cmex}{m}{n}

The second line make the big symbols like summation to use computer modern font
instead of times which is bigger and look nicer.

### Package bm for Bold Greek

For getting a bold Greek letter, `\mathbf{\alpha}` doesn't work. We have to put

    \usepackage{bm}

in the preamble and use `$\bm{\alpha}$` in math mode.

### mathcal style

Sometimes, we may use `\mathcal` or `\cal` in LaTeX for a calligraphic font.
However, what is happening may not be what you expected.

In normal case, the calligraphic font would be the one in `cmsy.pfb`

If you are using `mathptmx` package, the font will be a script font.

If you loaded with `eucal` package, the font will be the "Euler Script" font, which looks like an upright version of `cmsy.pfb`.

So if you are using `mathptmx` package, but want to get back the old script font (which is bold and easier to read), then issue the following command in the peamble:

    \DeclareMathAlphabet{\mathcal}{OMS}{cmsy}{m}{n}

### FdSymbols

A lot of symbols are defined in FdSymbols but if we use
`\usepackage{fdsymbol}`, some pre-existing symbols will be overwritten
(integral sign and summation sign, for example). To introduce just one symbol
at a time from FdSymbol, we use the following

```latex
\DeclareFontFamily{U}{FdSymbolE}{}
\DeclareFontShape{U}{FdSymbolE}{m}{n}{<-> s * FdSymbolE-Book}{}
\DeclareSymbolFont{fdbigsign}{U}{FdSymbolE}{m}{n}
\DeclareMathSymbol{\divslash}{\mathrel}{fdbigsign}{"87}
```

There are font families `FdSymbolA` to `FdSymbolE` and the code number (87 in
above) is the hex code for the symbol on that family. To find which is which,
we can print out all symbols using the following document:

```latex
\documentclass{article}
\usepackage{fdsymbol}
\usepackage{fonttable}
\begin{document}
\xfonttable{U}{FdSymbolA}{m}{n}
\xfonttable{U}{FdSymbolB}{m}{n}
\xfonttable{U}{FdSymbolC}{m}{n}
\xfonttable{U}{FdSymbolD}{m}{n}
\xfonttable{U}{FdSymbolE}{m}{n}
\end{document}
```

# Figures

### Side-by-side figures

Make two figures side-by-side, use this:

```latex
\begin{figure}[hbtp]
{\scriptsize
  \hbox{
      \input{plot-e-alp09}
      \input{plot-i-alp09}
  }
  \hbox{\hspace{38mm}\hbox{(a)\hspace{83mm}(b)}}
}
\caption{(a) elastic and (b) inelastic utility vs $\alpha$ with $\rho$=0.95}
\end{figure}
```

Alternative way: using minipage

```latex
\begin{figure}[hbtp]
    \hfill
    \begin{minipage}[t]{.45\textwidth}
        \epsfig{file=figure1.eps, scale=0.5}
        \caption{figure 1}
    \end{minipage}
    \hfill
    \begin{minipage}[t]{.45\textwidth}
        \epsfig{file=figure2.eps, scale=0.5}
        \caption{figure 2}
    \end{minipage}
    \hfill
\end{figure}
```

Yet another way: using subfigure package

```latex
\usepackage{subfigure}
\begin{figure}[htbp]
     \mbox{
        \subfigure[Honda]{\scalebox{0.3}{\input{prelude.pstex_t}}} \quad
        \subfigure[Toyota]{\scalebox{0.3}{\input{celica.pstex_t}}} \quad
        \subfigure[Nissan]{\scalebox{0.3}{\input{Fairlady.pstex_t}}} 
     }
     \caption{I like these!}
\end{figure}
```

### Four figures in a square

```latex
\begin{figure}[htbp]
    \mbox{
      \subfigure[Honda]{\scalebox{0.3}{\input{prelude.pstex_t}}} \quad
      \subfigure[Toyota]{\scalebox{0.3}{\input{celica.pstex_t}}}
    }
    \mbox{
      \subfigure[Nissan]{\scalebox{0.3}{\input{Fairlady.pstex_t}}} \quad
      \subfigure[Subaru]{\scalebox{0.3}{\input{Outback.pstex_t}}} 
    }
    \caption{I like these!}
\end{figure}
```


# Spacing

### Setting paper margin

    \usepackage{geometry}
    \geometry{verbose,a4paper,tmargin=1.75cm,bmargin=2cm,lmargin=2cm,rmargin=2cm,footskip=1cm}

Alternative method (specifying paper size and print region only)

    \usepackage[vcentering,dvips]{geometry}
    \geometry{papersize={170mm,240mm},total={124mm,185mm}}

### Line spacing

    \renewcommand{\baselinestretch}{1.4}

### Reduce space around captions

Remove the extra space between figure and captions, as well as the space between
two adjacent figure blocks:

    \setlength{\abovecaptionskip}{0pt}
    \setlength{\floatsep}{0pt}

### Removing large margins at print by pstops
If you got a doc with A5 content centered at an A4 paper, use this for two-pages-on-one-sheet:

    pstops -pa4 '2:0L@1(25.35cm,-3.075cm)+1L@1(25.35cm,11.775cm)' onepage.ps twopages.ps

If it is in Springer LNCS format, use this:

    pstops -pa4 '2:0L@1(26.6cm,-3.075cm)+1L@1(26.6cm,11.775cm)' onepage.ps twopages.ps

But sometimes, it is a bit larger than A5, the following is what I will use (I got this by trial and error):

    pstops -pa4 '2:0L@.87(24cm,-1.5cm)+1L@.87(24cm,13.35cm)' onepage.ps twopages.ps

### Spacing around display equations

Two set of skip length for normal and short lines.

	\setlength{\abovedisplayskip}{11pt}
	\setlength{\abovedisplayshortskip}{0pt}
	\setlength{\belowdisplayskip}{11pt}
	\setlength{\belowdisplayshortskip}{11pt}

To find the default of these values, we can use this LaTeX document:

	\documentclass{article}
	\begin{document}
	\the\abovedisplayskip{}\\
	\the\belowdisplayskip
	\end{document}

# Presentations

### BibTeX

Using BibTeX. Put these at the end of the document:

    \bibliographystyle{ieeetr}
    \bibliography{fair}

then run LaTeX by:

    $ latex document        # To generate *.aux
    $ bibtex document       # Base on *.aux to generate *.bbl
    $ latex document        # Learn about the existence of *.bbl
    $ latex document        # Regenerate the document

then you will have to dvi file

### Presentation using seminar class

Template for "seminar" slides:


    \documentclass[A4,16pt]{seminar}
    \begin{document}
    \begin{slide}
    \newslide
        \section{Hello}
        world?
        \begin{itemize}
            \item Here
           \item There
        \end{itemize}
    \end{slide}
    \end{document}


# Other

### Fancy headers

Look for the fancy header package

    \usepackage{fancyhdr}

### Single column ACM SIG Proceedings

My way to make a double column style template use for single column mode is use
the following prologue:

    \documentclass[a4paper]{sig-alternate}
    \makeatletter
    %Remove ACM copyright notice at the lower left corner
    \renewcommand{\@copyrightspace}{}
    %Make the ACM template into single column
    \renewcommand{\twocolumn}[1][1]{\onecolumn #1}
    \makeatother

### Modifying section headers (as well as others)

For example, section numbers should be in romans instead of arab:

    \renewcommand\thesection{\roman{section}}

also like this:

    \renewcommand{\thefigure}{\thechapter.\arabic{figure}}

see <http://www.iam.ubc.ca/~newbury/tex/numbering.html> for more

### Acknowledgment as footnotes

You may want footnotes without numbers or symbols. Here is the way:

    \def\blfootnote{\xdef\@thefnmark{}\@footnotetext}

Remember to enclose the definition block with `\makeatletter` and `\makeatother`. 
Source: <http://help-csli.stanford.edu/tex/latex-footnotes.shtml>

### Tabular with different justifications

Do in this way:

    \begin{tabular}{p{1cm}p{3cm}}
    ROW 1 & left justified   
    ROW 2 & \makebox[3cm]{centered}   
    ROW 3 & \makebox[3cm][r]{right justified}   
    \end{tabular}

more tablar tricks from
<http://amath.colorado.edu/documentation/LaTeX/reference/tables/ex3.html> and
<http://www.msu.edu/user/harris41/latex_tablespacing.html> and
<http://www.comp.leeds.ac.uk/andyr/misc/latex/latextutorial4.html>

### Parboxes

Make a box of text in paragraph mode as a "character" in a line:

    \parbox[b]{3cm}{blah blah blah}

where `b` is for bottom-aligned (choice: `c`, `t`) and `3cm` is the width.
Similar function can be achieved by minipage:

    \begin{minipage}{3cm}
    blah blah blah
    \end{minipage}

If you want framed version, enclose them with `\fbox{...}`

### Beautify tables

Use the `booktab` package by Simon Fear. The way to make tables beautify (and
look professional) is to use as little decoration as possible, e.g. don't use
vertical lines. The table elements shall have their own common region and
aligned to make it sound like a table. This is what we called the Gestalt
Principle: *things that are seen as forming a known shape are seen as being
together*.

### Spliting PostScript
Enlarging an A4 document into A3 size, with two sheets of A4 output make up one page of A3:

    # psresize -pa3 -Pa4 a4document.ps a3.ps
    # pstops -pa4 '1:0@1L(42cm,0)' a3.ps a4-upper.ps
    # pstops -pa4 '1:0@1L(21cm,0)' a3.ps a4-lower.ps

In the above, (42cm,0) means to shift the sheet left 42cm and up 0cm. It is
required because you rotated the sheet left a right angle (origin is on the
lower left corner). Hence I move the pictures to fit it into an A4 sheet.

If your printer cannot create zero-margin, you may need to change the shift
amount to cover those lost margin.


# Packages to note

 * Fancy chapter titles: [fncychap](http://www.ctan.org/tex-archive/help/Catalogue/entries/fncychap.html)
 * Nice figure captions: [caption](http://www.ctan.org/tex-archive/help/Catalogue/entries/caption.html)
 * Print crop marks: [crop](http://www.ctan.org/tex-archive/help/Catalogue/entries/crop.html)
 * Beautify tables: [booktab](http://www.ctan.org/tex-archive/macros/latex/contrib/booktabs/booktabs.pdf)

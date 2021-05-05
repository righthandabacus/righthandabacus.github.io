---
layout: post
title: "LaTeX package: qbordermatrix"
date: 2010-03-29 23:58:53 -0400
tags: code
---

In plain TeX, there is a macro `\bordermatrix` but I am not very satisfied with
it. The problem of that is, the "border" is always at the leftmost and topmost,
but not any other combinations. For some unknown reason, I am surprised that no
one has any problem with it and no one has ever developed a new package to
remediate this problem. Thus I do my own.

## Dissection of the legacy bordermatrix macro

The code for legacy `\bordermatrix`, as from the TeXbook, is below

```latex
\newdimen\p@renwd \setbox0=\hbox{\tenex B} \p@renwd=\wd0
\def\bordermatrix#1{\begingroup \m@th
  \setbox0=\vbox{\def\cr{\crcr\noalign{\kern2pt\global\let\cr=\endline}}
      \ialign{$##$\hfil\kern2pt\kern\p@renwd&\thinspace\hfil$##$\hfil
        &&\quad\hfil$##$\hfil\crcr
        \omit\strut\hfil\crcr\noalign{\kern-\baselineskip}
        #1\crcr\omit\strut\cr}}
  \setbox2=\vbox{\unvcopy0 \global\setbox1=\lastbox}
  \setbox2=\hbox{\unhbox1 \unskip \global\setbox1=\lastbox}
  \setbox2=\hbox{$\kern\wd1\kern-\p@renwd \left( \kern-\wd1
    \global\setbox1=\vbox{\box1\kern2pt}
    \vcenter{\kern-\ht1 \unvbox0 \kern-\baselineskip} \,\right)$}
\null\;\vbox{\kern\ht1\box2}\endgroup}
```

The code is explained below:

```latex
% Define the width of a bracket as \p@renwd
\newdimen\p@renwd \setbox0=\hbox{\tenex B} \p@renwd=\wd0

% Define the macro \bordermatrix
\def\bordermatrix#1{
\begingroup
  \m@th % Set the mathsurround stuff
  %
  % Put the matrix into box 0, without brackets
  \setbox0=\vbox{
     % Redefine \cr to provide inter-row gap
     \def\cr{\crcr\noalign{\kern2pt\global\let\cr=\endline}}
     % Matrix as an \ialign
     \ialign{
        $##$\hfil\kern2pt\kern\p@renwd& % First column: Big space at right
          \thinspace\hfil$##$\hfil&& % Second column: Small space at left
          \quad\hfil$##$\hfil % All other columns: space at left as inter-column gap
          \crcr % End of \ialign template
        \omit\strut\hfil\crcr % Small space to separate matrix from the line above
        \noalign{\kern-\baselineskip}
        #1\crcr % The matrix as provided by the user
        \omit\strut\cr % One empty cell as the last row
     }
  }
  %
  % Determine the first column's width:
  % Firstly, extract box0 for its last box inside, i.e. the last row
  % Then, unbox the last row to find the last (i.e. leftmost) column, save as box 1
  % Afterwards, the width of box 1 is taken as the width of first column
  \setbox2=\vbox{\unvcopy0 \global\setbox1=\lastbox}
  \setbox2=\hbox{\unhbox1 \unskip \global\setbox1=\lastbox}
  %
  % Put the bracket into the matrix, and save the result as an \hbox
  \setbox2=\hbox{$ % Start math mode
     $\kern\wd1\kern-\p@renwd % Shift right the first column's width minus bracket's width
     \left( % Print the left bracket
     \kern-\wd1 % Shift back the first column's width
     \global\setbox1=\vbox{\box1\kern2pt} % Save box 1's height + 2pt
     \vcenter{ % Make a vbox to print the matrix in correct position
        \kern-\ht1 % Shift upward a box 1's height as the topmost row should not inside bracket
        \unvbox0 % Print the matrix
        \kern-\baselineskip % Subtract a row's height from the calculation of the matrix's height
      } % Now this vbox's height is one row shorter then box 0
      \,\right) % Leave a tiny space and print the right bracket
  $}
  \null\; % Print some small space
  \vbox{\kern\ht1\box2} % Then the bracketed matrix shifted down one row to avoid
                        % the topmost border not overprinting existing text
\endgroup
}
```

## Related work

Herbert Voss wrote a manual "Math mode" that describes an enhanced `\bordermatrix`
so that we can provide user-specified bracket type and also it provided
`\bordermatrix*` as an derived version that puts the border at rightmost column
and bottommost row.

K. Border wrote a `\kbordermatrix` that allows the border to be in different font style.

## qbordermatrix

My work is a more generic version. It allows any combination of the four sides to be the border.

{% raw %}
```latex
% qbordermatrix.sty - A more flexible \bordermatrix for LaTeX
% Copyright 2010 (c) Adrian S. Tam <adrian.sw.tam@gmail.com>
% 
% This LaTeX package provides three commands, namely,
%   \bordermatrix - similar to the Knuth's version
%   \bordermatrix* - adapted from the manual "Math mode" by Herbert Voss
%                    that provides the border at bottom and right instead
%   \qbordermatrix - Quadri-border matrix, you can select any combination
%                    of the four borders to put outside of the matrix
%
% The \qbordermatrix macro takes the syntax of the following
%    \qbordermatrix[#1]{#2}{
%       a11 & a12 & a13 & a14   
%       a21 & a22 & a23 & a24   
%       a31 & a32 & a33 & a34
%    }
% The optional parameter #1 provides two characters as the matrix's left and
% right delimiters. The parameter #2 provides a subset of "n", "s", "e", "w"
% to tell which border should be outside of the matrix. Then the matrix is
% provided in the last curly bracket.
%
% In this sense, \bordermatrix is same as \qbordermatrix{nw} and
% \bordermatrix* is same as \qbordermatrix{se}. An example is given at
% the bottom of this file.
% 
% Compared to the Knuth's version, the matrices constructed by this package
% has the following properties
%  1. The row height is determined dynamically. Thus if the border row is
%     tall, e.g. a display fraction, the bracket is placed in the correct
%     size.
%  2. The matrix is aligned at the middle of the bracket, regardless the
%     heights of the matrix or the border rows
%  3. Different matrix delimiters are allowed, as long as they can fit into
%     the \left and \right pairs in equations
%  4. The output is in a vbox and the "canva" covers exactly the matrix
% Also the spacing around the matrix is a bit tight.
%
% Please send your comments and enhancement ideas to adrian.sw.tam@gmail.com
% 
\NeedsTeXFormat{LaTeX2e}
\ProvidesPackage{qbordermatrix}[2010/3/24 qbordermatrix v1.0]
\makeatletter
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Support of \bordermatrix and \bordermatrix* as described in "Math mode - v.2.43" by
% Herbert Voss
%
\newdimen\matrixrowsep
\matrixrowsep=2pt
% Determine if an asterisk is following \bordermatrix, and assert \@borderstar
\def\bordermatrix{\@ifnextchar*{\@bordermatrix@se}{\@bordermatrix@nw}}
% If no bracket is provided, use parenthesis
\def\@bordermatrix@se*{\@ifnextchar[{\@bordermatrix@se@i}{\@bordermatrix@se@i[()]}}
\def\@bordermatrix@nw{\@ifnextchar[{\@bordermatrix@nw@i}{\@bordermatrix@nw@i[()]}}
% Construction of the bordermatrix
\def\@bordermatrix@se@i[#1]#2{\@qbordermatrix@i[#1]{se}{#2}}
\def\@bordermatrix@nw@i[#1]#2{\@qbordermatrix@i[#1]{nw}{#2}}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% More flexible \qbordermatrix macro
%
\newif\if@north
\newif\if@south
\newif\if@east
\newif\if@west
\def\@swallow#1{}
% If no bracket is provided, use parenthesis
\def\qbordermatrix{\@ifnextchar[{\@qbordermatrix@i}{\@qbordermatrix@i[()]}}
% Construction of the bordermatrix
\def\@qbordermatrix@i[#1]#2#3{%
\begingroup
  % Parse parameters
  \@northfalse\@southfalse\@eastfalse\@westfalse
  \in@{n}{#2}\ifin@\@northtrue\fi
  \in@{s}{#2}\ifin@\@southtrue\fi
  \in@{e}{#2}\ifin@\@easttrue\fi
  \in@{w}{#2}\ifin@\@westtrue\fi
  \m@th % mathsurround stuff
  % compute bracket width
  \setbox\z@\hbox{$\left\@firstoftwo#1\right.$}
  \newdimen\@leftbwd
  \@leftbwd\wd\z@
  \setbox\z@\hbox{$\left.\right\@secondoftwo#1$}
  \newdimen\@rightbwd
  \@rightbwd\wd\z@
  % Define carriage-return for array rows
  \def\cr{\crcr\noalign{\kern\matrixrowsep\global\let\cr\endline}} % for a safer \cr
  \def\\{\cr}
  % Set box0 to contain the matrix without bracket
  \setbox\z@\vbox{%
    \ialign{ % the matrix
      \hfil$##$\hfil\kern 2\p@\kern\@leftbwd & % first column: big space at right for open bracket
      \thinspace\hfil $##$\hfil && % second column: small space at left
      \quad\hfil $##$\hfil % all other columns: big space at left for column separation
      \crcr % end of preamble
      #3\crcr % the matrix itself
    }
  }%
  % Determine the last row height
  % Extract last row from box 0 and measure its height
  \setbox\thr@@\vbox{\unvcopy\z@\global\setbox\tw@\lastbox}
  \setbox\tw@\vbox{\hrule\box\tw@\hrule}
  \newdimen\@lastrowht
  \@lastrowht\ht\tw@
  % Determine the first row height
  % by removing one row at a time from the content of box 0 (copied to box 3).
  % If, after removing a row, the height of the box is less than 4pt, we rollback
  % one step and that is the first row's height
  \loop
    \ifdim\ht\thr@@>0pt
      \setbox\tw@\copy\thr@@
      \setbox\thr@@\vbox{%
        \unvbox\thr@@
        \loop% Remove kern, skip, and penalty until we found a box to remove
          \unkern\unskip\unpenalty
          \global\setbox\@ne\lastbox
          \ifvoid\@ne
        \repeat
      }
  \repeat
  \setbox\tw@\vbox{\hrule\copy\@ne\hrule} % use \copy\tw@ for taller height
  \newdimen\@firstrowht
  \@firstrowht\ht\tw@
  % Determine the last column width
  % After above, the first row is stored in box 1 and we extract the last
  % column and meausre its width
  \setbox\tw@\hbox{%
    \unhcopy\@ne
    \loop
      \unkern\unskip\unpenalty
      \global\setbox\thr@@\lastbox
      \ifvoid\thr@@
    \repeat
  }
  \newdimen\@lastcolwd
  \@lastcolwd\wd\thr@@
  % Determine the first column width
  % The entire first row is stored in box 1 and we do the similar iterations
  % to remove one box at a time from box 1 to find the first column width
  \loop
    \setbox\tw@\copy\@ne
    \setbox\@ne\hbox{%
      \unhbox\@ne
      \loop
        \unkern\unskip\unpenalty
        \setbox\thr@@\lastbox
        \ifvoid\thr@@
      \repeat
    }
    \ifdim\wd\@ne>0pt
  \repeat
  \newdimen\@firstcolwd
  \@firstcolwd\wd\tw@
  % Put the bracket into the matrix:
  % First right shift for first column's width, and put the opening bracket, then
  % left shift to original position to put the matrix, and then shift left to close
  % the bracket and shift back to right.
  % The matrix, however, is also shifted to reflect the desired height of bracket,
  \setbox\tw@\hbox{$
    \if@west\kern\@firstcolwd\kern-\@leftbwd\fi
    \left\@firstoftwo#1
    \if@west\kern-\@firstcolwd\fi
    \vcenter{%
      \if@north\kern-\@firstrowht\fi
      \unvcopy\z@
      \if@south\kern-\@lastrowht\fi
    }
    \if@east\kern-\@lastcolwd\fi
    \right\@secondoftwo#1
    \if@east\kern-\@rightbwd\kern\@lastcolwd\fi
  $}
  % Print the result
  \if@north\setbox\tw@\vbox{\kern\@firstrowht\box\tw@}\fi
  \if@south\setbox\tw@\vtop{\box\tw@\kern\@lastrowht}\fi
  \box\tw@
\endgroup
}
\makeatother
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Example:
%
% \documentclass{article}
% \usepackage{qbordermatrix}
% \begin{document}
% \hrule y\vrule$\bordermatrix[\{\}]{%
% 0 &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% a & x_1 & y_2 & z_1 & t_1 \cr
% b & x_3 & y_4 & z_2 & t_2 \cr
% c & x_5 & y_6 & z_3 & t_3 \cr
% d &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% }$\vrule z\hrule
% 
% \hrule y\vrule$\bordermatrix*[\{\}]{%
% 0 &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% a & x_1 & y_2 & z_1 & t_1 \cr
% b & x_3 & y_4 & z_2 & t_2 \cr
% c & x_5 & y_6 & z_3 & t_3 \cr
% d &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% }$\vrule z \hrule
% 
% \hrule y\vrule$\qbordermatrix[\{\}]{nesw}{%
% 0 &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% a & x_1 & y_2 & z_1 & t_1 \cr
% b & x_3 & y_4 & z_2 & t_2 \cr
% c & x_5 & y_6 & z_3 & t_3 \cr
% d &   1 &2^{2^2}& 3 & \displaystyle\frac{4}{2} \cr
% }$\vrule z
% \hrule
% \end{document}
%
```
{% endraw %}

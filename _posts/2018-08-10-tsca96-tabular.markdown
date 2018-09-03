---
layout: post
title: "Tupaj et al (1996) Extracting Tabular Information From Text Files"
date: 2018-08-10 12:00:00 -0400
category: paper
bibtex:
  type: misc
  title: "Extracting Tabular Information From Text Files"
  author: "Scott Tupaj and Zhongwen Shi and Hwa Chang and Hassan Alam"
  year: 1996
---

A paper on how to identify and extract tables from scanned text documents. The
author describe the whole process as four phrases: (1) isolate potential table
areas, (2) OCR, (3) isolate the beginning and end of the tables, (4) isolate
table components, i.e., header, cells, footer.

The paper concerns two types of tables: Technical tables in scientific journal
that contains numeric data with extensive use of measuring units; and financial
tables with data of dates, currency, percentage and text and often include
calculations done such as sum and ratios.

# Isolating tables from image

The paper proposes to use a combination of white space analysis and keyword
analysis to mark rectangular zones on the page that *may* contain tabular
information.

First horizontal pass: To find significant vertical white space gaps. On each
horizontal scan line of image (assume black and white bitmap), we take into
account of three values:

1. pixel count $$n$$: the number of black pixels along the scan line
2. cross counts $$c$$: the number of times the pixel value changes in the scan
   line
3. extent $$e$$: the number of pixels between the first and the last black pixel
   on the scan line

For example, the following bitmap as one scan line:

    00110111101001111100010000

the scan line has 26 pixels, with $$n=13, c=9, e=19$$. And then we can define
the following functions to classify a line:

- bar of noise of horizontal line: high $$n/e$$ ratio and low $$c/n$$ ratio
- speckled noise, ascenders or descenders: low $$n/e$$ ratio and high $$c/n$$
  ratio
- text: high $$n/e$$ ratio and high $$c/n$$ ratio

Using such property, we can define a function $$f(n,c,e)$$ as indicator for
whether a scan line belongs to text (indicator should be interpreted as output
of sigmoidal function, such as logistic function). We then score each scan line
with $$f(n,c,e)$$ and it will be easy to identify white spaces scan lines (see
Figure 6 of the paper). If we have white space gaps $$>k$$ std dev from the
mean, it identifies a significant document separation points (i.e., not an
ordinary space between text lines but likely a paragraph break or space between
text and figures, etc.) The paper suggests $$k=1.5$$.

Then within the table, we can apply the similar scan line scoring technique to
identify horizontal table separators (e.g., lines between rows in the table).
Such as the lines that have equally spaced segments (for dashed lines) or large
consecutive segments (for solid lines). At this stage, we broke down a table
into horizontal strips.

# Vertical scan lines

The next step is to figure out if the horizontal strips contain multiple
columnes of flow text. The technique is similar, apply the scoring function to
each *vertical* scan line to see if we can have vertical cuts. The paper assumed
no document will have more then three columns. Therefore if we find cuts that
results in $$>3$$ columns, it is a table.

The following are to identify multiple column document:

- Let each interval of zero values have width $$Z_i$$, and each interval of
  non-zero values have width $$N_i$$
- $$\{ Z_1 N_1 Z_2 N_2 Z_3 \}$$ exists with (1) $$Z_2<30$$ and (2) $$\dfrac{\min(N_1,N_2)}{\max(N_1,N_2)} <0.9$$
  - $$\{ Z_1 N_1 Z_2 N_2 Z_3 \}$$ = two text column separated by a white space
    and margins at both end

The paper assumes we recognize text by OCR in table once it is identified. The
output of OCR converts the data from pixel coordinates to text coordinates, and
we can then analyze words for content and semantic meaning.

# Text analysis

To undestand components inside a table, the paper proposes to label each text
lines with the following types:

- B: blank line
- S: line containing a single text entity
- C: line containing multiple text entities (usually tabbed text)

An entity is defined as a set of characters bounded on the left and right by
four or more spaces (to tell apart spaces between words and spaces between table
columns), or by the table boundary.

For example, this is a table and the type tags of each line:

```
Currency data is 5 minutes delayed (times in ET) and based     S
on the Bloomberg Generic Composite rate (BGN). See full        S
details and disclaimer.                                        S
                                                               B
CURRENCY    VALUE       CHANGE  NET CHANGE  TIME (EDT)         C
                                                               B
EUR-USD       1.1390    -0.0023     -0.20%  12:57 PM           C
USD-JPY     110.7000    -0.1300     -0.12%  12:57 PM           C
GBP-USD       1.2745    -0.0013     -0.10%  12:57 PM           C
AUD-USD       0.7264    -0.0038     -0.52%  12:57 PM           C
USD-CAD       1.3152     0.0012     +0.09%  12:57 PM           C
USD-CHF       0.9940    -0.0012     -0.12%  12:58 PM           C
EUR-JPY     126.0900    -0.4200     -0.33%  12:58 PM           C
EUR-GBP       0.8938     0.0002     +0.02%  12:57 PM           C
USD-HKD       7.8499     0.0000     0.00%   12:57 PM           C
EUR-CHF       1.1323    -0.0036     -0.32%  12:57 PM           C
USD-KRW   1,134.0100     5.0000     +0.44%   2:29 AM           C
```

Afterwards, we find keywords that may appear in table titles as the starting
point and scan downwards. (Here assumed captions are at top of the table) The
paper suggests to flag the lines as table body if the sequence fits any of the
following:

- `{C}`
- `{S}`
- `{B S (C | S)}`
- `{B S B S}`
- `{B B C}`
- `{B B (S | B) C}`

and the pattern `{B B (S | B) C}` futher marks the end of a table.

# Isolating table components

Based on the notion of intersecting consecutive vectors of space intervals. Each
line $$i$$ of text in the table is represented as a set:

$$ R_i = (a_1,b_1) \cup (a_2,b_2) \cup \cdots \cup (a_n,b_n) $$

where $$a_j,b_j$$ are the leftmost and rightmost position of the $$j$$-th white
space interval of the line. Since we always have left and right margin, there is
at least two white space interval in each non-blank line. Then for the whole
table, we create

$$ R = R_1 \cap R_2 \cap \cdots \cap R_m $$

for all body line $$R_i$$. If a table line contains only two space intervals, it
is either a table header/footer or a single entry in that row.  The midpoint of
each space interval in the resultant $$R$$ is a potential vertical plumb line in
the table.  We have to be careful not to include the header/footer or otherwise
a lot of plumb lines are lost in $$R$$.

After this stage, the paper said there will possibly be two scnarios that needs
further semantc analysis:

1. A "ghost" column is form due to extra spacing
2. Column hierarchy is not detected in case of spanning superheaders

# Reference

- recursive X-Y cut method: G. Nagy and S. Seth, "Hierarchical representation of
  optically scanned documents", Proc of 7th Int Conf on Pattern Recognition,
  pp.347-349, 1984.
- isolating table components (convert table into a Cartesian grid): A.
  Laurentini and P. Viada, "Identifying and understanding tabular material in
  compound documents", Proc. Int. Conf. no Pattern Recognition, pp.405-409,
  1992.

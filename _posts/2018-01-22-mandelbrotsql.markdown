---
layout: post
title: Mandelbrot set SQL
subtitle: using common table expressions in SQLite
date: Mon, 22 Jan 2018 19:34:34 -0500
tags: code math
---
There is a recent change in [SQLite
documentation](http://www.sqlite.org/lang_with.html), that depicts the use of
common table expressions (CTEs) in SQLite to draw a Mandelbrot set using ASCII
art:

```sql
WITH RECURSIVE
  xaxis(x) AS (VALUES(-2.0) UNION ALL SELECT x+0.05 FROM xaxis WHERE x<1.2),
  yaxis(y) AS (VALUES(-1.0) UNION ALL SELECT y+0.1 FROM yaxis WHERE y<1.0),
  m(iter, cx, cy, x, y) AS (
    SELECT 0, x, y, 0.0, 0.0 FROM xaxis, yaxis
    UNION ALL
    SELECT iter+1, cx, cy, x*x-y*y + cx, 2.0*x*y + cy FROM m 
     WHERE (x*x + y*y) < 4.0 AND iter<28
  ),
  m2(iter, cx, cy) AS (
    SELECT max(iter), cx, cy FROM m GROUP BY cx, cy
  ),
  a(t) AS (
    SELECT group_concat( substr(' .+*#', 1+min(iter/7,4), 1), '') 
    FROM m2 GROUP BY cy
  )
SELECT group_concat(rtrim(t),x'0a') FROM a;
```

and the output is

```
                                    ....#
                                   ..#*..
                                 ..+####+.
                            .......+####....   +
                           ..##+*##########+.++++
                          .+.##################+.
              .............+###################+.+
              ..++..#.....*#####################+.
             ...+#######++#######################.
          ....+*################################.
 #############################################...
          ....+*################################.
             ...+#######++#######################.
              ..++..#.....*#####################+.
              .............+###################+.+
                          .+.##################+.
                           ..##+*##########+.++++
                            .......+####....   +
                                 ..+####+.
                                   ..#*..
                                    ....#
                                    +.
```

How is it done?

A Mandelbrot set is defined on the complex plane with the function
$$f_c(z)=z^2+c$$ such that it is the set of all points
$$ \{c\in\mathbb{C}: |f_c^{(n)}(0)| < \infty,\; \forall n\in\mathbb{N}\} $$
with the notation $$f_c^{(n)}(z)$$ defined as compound function,
$$f_c^{(1)}(z) = f_c(z)$$, $$f_c^{(n+1)}(z) = f_c(f_c^{(n)}(z))$$.
Finding the infinite sequence of compound function is impractical, which we
usually approximate the Mandelbrot set for:

$$ \{c\in\mathbb{C}: |f_c^{(n)}(0)| < R, \; n=1,...,N\} $$

with some predefined large $$N$$ and $$R$$.

This is how we can understand the above SQL: From top to bottom, the table
`xaxis` and `yaxis` are values of axis. Because of the text mode screen, $$y$$
(i.e. imaginary axis) is coarser than $$x$$ (the real axis). This is a recursive
CTE that starts with values --1.0 and --2.0 respectively and increments the
value recursively until the upperbound is met.

Next is table `m`, which starts with a Cartesian join with $$x$$- and $$y$$-axes
and assign it as iteration 0. This is a lot of data points. Then we do for
iteration 1 to 27, and in each iteration, we compute:

$$\begin{aligned}
z &= x + y\mathrm{i} \\
c &= c_x + c_y \mathrm{i} \\
f_c(z) &= z^2 + c \\
       &= (x^2 - y^2 + c_x) + (2xy + c_y) \mathrm{i}
\end{aligned}$$

and we remember the result of $$|f_c(z)| < 4$$ only in each iteration. This is
assuming if $$f_c(z)$$ moved to outside the circle of $$R=4$$, it is diverging and
if it does not so for the first 27 iterations, it never.

The tables `xaxis`, `yaxis`, and `m` are built recursively using `UNION ALL`.
While it may contain redundant data it is an optimization to defer the
repetition removal until later, in table `m2`.

Table `m2` records the maximum iteration ever found in table `m` for each $$c$$.
We imposed the bound $$R=4$$, hence it is the iteration that $$c$$ is determined
diverging. Also note that `m2` is built with `GROUP BY` on `cx, cy`, therefore
it is implicitly sorted by such as well.

Then table `a` is drawing the Mandelbrot set by scanning for each constant
value of imaginary part of $$c$$. With `m2` implicitly sorted, the `group_concat()`
function is collating values of real part of $$c$$ in ascending order. We use
characters space, `.`, `+`, `*`, and `#` for iteration count 0, 1, 2, 3, and 4
and above respectively. The selection is achieved by `substr()` call, a neater
alternative to `CASE` statement.

Finally, `group_concat()` in the `SELECT` statement is to join lines of text
from table `a` with newline character (ASCII `0x0A`). The part `rtrim(t)` is
remove right hand size whitespaces from each row (in the form of the only
column `t`) of table `a`.

Using similar logic, this is the way to generate Julia set (after appropriately
defining `xaxis`, `yaxis` and substitute `cx` and `cy` with desired values):

```sql
WITH RECURSIVE
  xaxis(x) AS (...),
  yaxis(y) AS (...),
  m(iter, zx, zy, x, y) AS (
    SELECT 0, x, y, 0, 0 FROM xaxis, yaxis
    UNION ALL
    SELECT iter+1, zx, zy, x*x-y*y + cx, 2.0*x*y + cy FROM m 
     WHERE (x*x + y*y) < 4.0 AND iter<28
  ),
  m2(iter, zx, zy) AS (
    SELECT max(iter), zx, zy FROM m GROUP BY zx, zy
  ),
  a(t) AS (
    SELECT group_concat( substr(' .+*#', 1+min(iter/7,4), 1), '') 
    FROM m2 GROUP BY zy
  )
SELECT group_concat(rtrim(t),x'0a') FROM a;
```

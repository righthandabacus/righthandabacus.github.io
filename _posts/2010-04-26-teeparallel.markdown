---
layout: post
title: "Parallelism by tee in bash"
date: 2010-04-26 21:21:51 -0400
tags: code
---

This is what I did in my simulation processing code.

Assume you are generating a very big file to stdout, and you want to compress it
(gzip), extract the first 1000 lines (head), extract the last 1000 lines (tail),
count the total number of lines (wc), and some other filtering. It is
inefficient to do all these individually because the file is too big to write to
hard disk by itself and compressing/decompressing takes a long time. It would be
the best if we can do all these at the same time.

Here is how I did it:

```sh
GenerateBigFile |
  tee >(filter.pl > output.filter) |
  tee >(tail -1000 > output.tail) |
  tee >(head -1000 > output.head) |
  gzip > output.gz
```

The principle is, use bash's process substitution to create a FIFO to write to.
Then the `tee` command writes a copy to the process and pipe another copy to
another tee. In this way, we are doing something MISD (multiple instruction,
single data).

---
layout: post
title: "Reservoir Sampling with Uniform Probability"
date: 2010-04-26 12:51:32
tags: code
---

Assume you have a million lines of data in ordered pairs and you want to plot it
in GNU Plot. That's likely to kill your computer. Indeed, plotting a million
dots on a graph does not give more information than that plotting a thousand
dots. Therefore, how we can sample a thousand dots from a million dots is an
issue.

GNU Plot allows you to pipe data through a command, for example, instead of

    plot "million.dots" using 1:2 with points

we can have

    plot "< cat million.dots | sample 1000" using 1:2 with points

so that the data are sampled by the script `sample`.

## Reservoir sampling

Sampling is trivial, but an online sampling may be not. Reservoir sampling is to
do online sampling with a pool of fixed size. The requirement here is that,
every input is sampled in equal probability. Check out my contribution on
[Wikipedia](http://zh.wikipedia.org/wiki/%E6%B0%B4%E5%A1%98%E6%8A%BD%E6%A8%A3)
for mathematical proof, below is my code: [`reservoir.pl`](/img/reservoir.pl)

```perl
#!/usr/bin/perl
#
# Script to do reservoir sampling. The script takes the first parameter as the
# reservoir sizea k and then sample the inputted file into k lines. The output
# is sorted in the original order.
#
# Synopsis:
#    cat BigFile | reservoir.pl 4
#  or
#    reservoir.pl 4 BigFile
#
use strict;
use warnings;
use 5.010;

my $k = shift // 1;
my @lines;
my @linum;
my $n = 0;
srand;
while(<>) {
	$n++;
	if ($n <= $k) {
		push @lines, [$n,$_];
	} elsif (rand() < ($k/$n)) {
		$lines[ int(rand($k)) ] = [$n,$_];
	};
}
foreach (sort {$$a[0] <=> $$b[0]} @lines) { print $$_[1]; };
```

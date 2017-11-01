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

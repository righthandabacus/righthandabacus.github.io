#!/usr/bin/perl
# Random function caller
# Put function pointers into an array and define a transition probability
# matrix, an initial probability array, such that we jump from one state
# to another state and in each state, we call the corresponding function
# once.
#
# Adrian Sai-wah Tam
# Wed Apr  4 21:26:21 HKT 2007

use strict;
use warnings;
use vars qw(@transprob @funcvec @initvec $termcheck);

# the n-by-n transition probability matrix, assumed irreducible
@transprob = (	[0.2, 0.2, 0.1, 0.5],
		[0.3, 0.2, 0.4, 0.1],
		[0.1, 0.3, 0.3, 0.3],
		[0.2, 0.3, 0.3, 0.2] );
# the initial probability n-vector
@initvec = (1.0, 0, 0, 0);

# the function pointers stored in n-vector
@funcvec = (\&func1, \&func2, \&func3, \&func4);

# termination condition checker
$termcheck = \&termCheckFunc;

##################################
# The main code: Random transition
# (nothing shall be changed below)

# Sanity check: Make sure the probability matrix is a probability matrix
# and the probability vector is a probability vector
for my $i (0..$#transprob) {
	die "Transition probability matrix is not square (on row $i)\n"
		if ($#{$transprob[$i]} != $#transprob);
	my $sum=0.0;
	my $j;
	for $j (0..$#{$transprob[$i]}) {
		$sum += $transprob[$i][$j];
	};
	if ($sum != 1.0) {
		warn "Sum of row $i of transition probability matrix isn't 1. Fixed.\n";
		for $j (0..$#{$transprob[$i]}) {
			$transprob[$i][$j] /= $sum;
		};
	};
};
my $sum=0.0;
for (0..$#initvec) { $sum += $initvec[$_]; };
if ($sum != 1.0) {
	warn "Initial probability vector does not sum to 1. Fixed\n";
	for (0..$#initvec) { $initvec[$_] /= $sum; };
};
die "Dimension mismatch between initial prob vector & transition prob matrix\n"
	if ($#initvec != $#transprob);
die "Dimension mismatch between function vector & transition prob matrix\n"
	if ($#funcvec != $#transprob);
for (0..$#funcvec) {
	die "Element $_ of function vector is not a function\n"
		if (ref($funcvec[$_]) ne 'CODE');
};

# Select initial state
my $state;
my $random = rand(1);
$sum=0.0;
for ($state=0; $state<=$#initvec; $state++) {
	$sum += $initvec[$state];
	last if ($sum >= $random);
};

# until terminating condition is met, traverse the Markov chain
until (&$termcheck()) {
	&{$funcvec[$state]}();		# Call the corresponding function
	$random = rand(1);		# Toss a coin
	$sum=0.0;			# Lookup the transition matrix
	my $oldstate=$state;		#   and jump accordingly
	for ($state=0; $state<=$#transprob; $state++) {
		$sum += $transprob[$oldstate][$state];
		last if ($sum >= $random);
	};
};

# code ends: Random transition
# (nothing shall be changed above)
##################################

########################################
# Things should be changed for real use:
#
sub func1 {
	print "1";
};

sub func2 {
	print "2";
};

sub func3 {
	print "3";
};

sub func4 {
	print "4";
};

BEGIN {
	my $counter=0;
	my $endcount=100;
	sub termCheckFunc {
		return (++$counter == $endcount);
	};
}

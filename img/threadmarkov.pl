#!/usr/bin/perl
# Multithreaded random function caller
# Start a number of threads, each of them running a loop such that it
# traverses a Markov Chain and calls a function on every step.
#
# Adrian Sai-wah Tam
# Fri Apr  6 02:07:58 HKT 2007

use strict;
use warnings;
use threads;
use threads::shared;
use Time::HiRes qw( sleep time );	# For allowing floating point seconds
use vars qw(@transprob @funcvec @initvec $termcheck);

###### Markov chain parameters:
#
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

########################
# Main body

# Thread control
select((select(STDOUT), $|=1)[0]);	# Make STDOUT `hot', i.e. no buffering
my $run : shared = 1;			# Flag to control the thread to continue
$SIG{TERM} = sub { $run=0; };		# Setting signal handler as anonymous function
my $numthread = 2;			# How many threads we want to run?
my $mean = 0.1;				# Mean sleep time
my @threads;				# Thread pool
sanitycheck();				# sanity check the prob matrix & vectors
for (0..$numthread-1) {
	$threads[$_] = threads->new(\&markovtrail);
};

for (0..$numthread-1) {			# If ^C received, join the threads
	$threads[$_]->join();
};
print "\n";
exit;

# Main body ends
########################

# Maximum of two
sub max { return $_[0]>$_[1]?$_[0]:$_[1]; };

# Sanity check for the probability matrix and vectors
sub sanitycheck {
	# Sanity check: Make sure the probability matrix is a probability matrix
	# and the probability vector is a probability vector
	for my $i (0..$#transprob) {
		die "Transition probability matrix is not square (on row $i)\n"
			if ($#{$transprob[$i]} != $#transprob);
		my $j;
		my $sum=0.0;
		for $j (0..$#{$transprob[$i]}) { $sum += $transprob[$i][$j]; };
		if ($sum != 1.0) {
			warn "Sum of row $i of transition probability matrix isn't 1. Fixed.\n";
			for $j (0..$#{$transprob[$i]}) { $transprob[$i][$j] /= $sum; };
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
};

# Markov Chain function
sub markovtrail {
	# Select initial state
	my $state=-1;
	my $random = rand(1);
	my $sum=0.0;
	for (; $sum<$random; $sum+=$initvec[++$state]) {};
	
	# until terminating condition is met, traverse the Markov chain
	my $before; my $after;			# Initialize timers
	until (&$termcheck()) {
		$before = time;
		&{$funcvec[$state]}();		# Call the corresponding function
		$random = rand(1);		# Toss a coin
		$sum=0.0;			# Lookup the transition matrix
		my $oldstate=$state;		#   and jump accordingly
		for ($state=-1; $sum<$random; $sum += $transprob[$oldstate][++$state]) {};
		$after = time;
		sleep max(0,-log(1-rand(1))*$mean-$after+$before);
	};
};

########################################
# Things should be changed for real use:
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
	my $endcount=50;
	sub termCheckFunc {
		return 0 if ($run==0);
		return (++$counter == $endcount);
	};
}


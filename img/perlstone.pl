#!/usr/bin/perl
# Perlstone
# Stress testing a web site by running multiple Markov process
#
# Adrian Sai-wah Tam
# Fri Apr  6 03:00:22 HKT 2007

use strict;
use warnings;
use threads;
use threads::shared;
use LWP::UserAgent;
use Time::HiRes qw( sleep time );	# For allowing floating point seconds

########################
# Markov chain parameters:

# the n-by-n transition probability matrix, assumed irreducible
my @transprob = (	[0.2, 0.2, 0.1, 0.5],
			[0.3, 0.2, 0.4, 0.1],
			[0.1, 0.3, 0.3, 0.3],
			[0.2, 0.3, 0.3, 0.2] );
# the initial probability n-vector
my @initvec = (1.0, 0, 0, 0);

# the function pointers stored in n-vector
my @funcvec = (\&func1, \&func2, \&func3, \&func4);

# termination condition checker
my $termcheck = \&termCheckFunc;

# Markov chain parameters end
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
# Markov chain functions

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

# Markov chain functions end
########################
# Browser emulation

# Global variable for browser emulation
my $browser = new LWP::UserAgent;
$browser->cookie_jar({});
push @{$browser->requests_redirectable}, 'POST';

# Global variable for browser behaviour functions
my $server_addr = "http://192.168.123.456/";
my $user_id = "JohnDoe";
my $user_pw = "foobar";

# Browser emulation end
########################
# Crawling functions

my @problems;

sub crawling1 {
        my $response=$browser->get($server_addr."dgp");
        if ($response->is_success) {
                my $content=$response->content;
                return 0 unless (index($content,"</html>")>=0);
                return 1 if ($content =~ m/this\.location\.href.*mainBody/s);
                return 2 if ($content =~ m/parent\.location\.href.*index/s);
        };
        return 0;
};
 
sub crawling2 {
        my $response=$browser->post($server_addr."login.php",
                [ uid => $user_id, pwd => $user_pw, submit => 'Login' ] );
        if ($response->is_success) {
                my $content=$response->content;
                return 0 unless (index($content,"</html>")>=0);
                return 0 unless ($content =~ m/<div id='main'\s[^>]*>/s);
                my @links = ($content =~ m#/problemset\.php\?frame=main[^']*#sg);
                if ($#links==29) {
                        @problems = @links;
                        return 1;
                };
                return 2;
        };
        return 0;
};

# Crawling functions end
########################
# Dumb stuff
sub func1 {
	die "Server error" unless crawling1();
};

sub func2 {
	die "Server error" unless crawling1();
};

sub func3 {
	die "Server error" unless crawling2();
};

sub func4 {
	die "Server error" unless crawling2();
};

BEGIN {
	my $counter=0;
	my $endcount=50;
	sub termCheckFunc {
		return 0 if ($run==0);
		return (++$counter == $endcount);
	};
}

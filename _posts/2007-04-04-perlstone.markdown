---
layout: post
title: "Perlstone: Multithreaded web system stress test engine in Perl"
date: 2007-04-04 13:23:38 +0800
tags: code
---

In the good old days, all the web pages are static and the web server is
fragile. Hence we use some tools like *webstone* to stress test our web server.
This is what I did ten years ago.

Nowadays, webstone cannot fit our use for two reasons: (1) our web server is
more robust that you can hardly exhaust it in a simple way and (2) we made the
web a *system* and it is dynamic in nature, with whole bunch of stuff like
Flash, cookies, web sessions, AJAX, Javascript, etc. Hence we need something
more advanced.

To stress test a web, I suggest to write a script to do so rather than use
`wget` or `curl` or something alike because of the same reason. It is
troublesome to write a C++ or Java program and it is kind of overkill to do so.
The best tool for a quick and dirty stress test is Perl actually. Hence I made
this tool.

## The idea of traversing a web: Markov Chain

To stress test a web *page* is easy: Just craw this page for an infinite time.
To stress test a web *site* is not so trivial, because there are many pages
and we don't want to craw the same page in the same order again and again ---
this is not what people will do when they are browsing.

My idea of web stress test is to use a Markov Chain to simulate human behavior:
We start on a page and then go to another page and then yet another page, with
the possibility of going forward and backward, or jump to something else. This
is actually a trail following certain Markov Chain.

To model this, I created the following code:
([`markovcaller.pl`](/img/markovcaller.pl) - Markov Chain trailwalker for
calling functions)

```perl
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
@transprob = (  [0.2, 0.2, 0.1, 0.5],
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
    &{$funcvec[$state]}();          # Call the corresponding function
    $random = rand(1);              # Toss a coin
    $sum=0.0;                       # Lookup the transition matrix
    my $oldstate=$state;            #   and jump accordingly
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
```

The above code does the following: It defines a Markov Chain transition
probability matrix, and an initial probability vector. When the program starts,
it tosses a coin to select the initial state according to the initial
probability vector. Subsequently, it changes its state by generating a random
number and consulting the transition probability matrix. Every state in the
program is associated with a function so that the corresponding function is
called for once whenever we are transiting to certain state.

When you run the above program, you will see a line of a hundred random digits:

    14112342143141322334414334....

## Multithreaded Perl

To create a mutithreaded Perl script for crawling web, you may want to choose from the following:

* when we downloaded one page, shall we proceed immediately, or wait for a while first?
* if we wait, how long should we do?
* we should measure the waiting time (i.e. the time we slept) or we should
  measure the action interval (i.e. the time between the beginning of two
  consecutive actions)?

The simplest way for doing multithread on Perl is the following piece of code:

```perl
use threads;
use threads::shared;
use Time::HiRes qw( sleep );        # For allowing floating point seconds

select((select(STDOUT), $|=1)[0]);  # Make STDOUT `hot', i.e. no buffering
my $run:shared=1;                   # Flag to control the thread to continue
$SIG{TERM}=sub { $run=0; };         # Setting signal handler as anonymous function
my $thread1=threads->new(\&func,1); # Fork a thread
my $thread2=threads->new(\&func,2)  # Fork another thread

func(0);                            # Myself is a thread as well

$thread1->join();                   # If ^C received, join the threads
$thread2->join();

### Thread function:
sub func {
    while($run) { print $_[0]; sleep 0.1; };
};
```

For random *sleep* in exponential distribution, you can simply replace the `sleep 0.1` with

```perl
sleep -log(1-rand(1))*$mean
```

where `$mean` is the mean sleep time in seconds.

Actually, *sleep* for an exponential time does not mean you can simulate Poisson
behavior. For more accurately simulate Poisson, you should make sure your sleep
time has your "wake time" subtracted:

```perl
use Time::HiRes qw( sleep time );     # For allowing floating point timer
sub func {
    sub max { return $_[0]>$_[1]?$_[0]:$_[1]; }
    while($run) {
        my $before = time         # Current time in floating point
        print $_[0];              # Suppose this will take a significant time
        my $after = time;
        sleep max(0,-log(1-rand(1))*$mean-$after+$before);
    };
};
```

Of course, you may also use semaphore and shared variables to allow threads communicate.


## Example: Multithreaded Markov Chain Trailwalker

Combining the code in the previous two sections, we have the following:
([`threadmarkov.pl`](/img/threadmarkov.pl) - Multithreaded Markov Chain
trailwalker)

```perl
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
use Time::HiRes qw( sleep time );       # For allowing floating point seconds
use vars qw(@transprob @funcvec @initvec $termcheck);

###### Markov chain parameters:
#
# the n-by-n transition probability matrix, assumed irreducible
@transprob = (  [0.2, 0.2, 0.1, 0.5],
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
select((select(STDOUT), $|=1)[0]);      # Make STDOUT `hot', i.e. no buffering
my $run : shared = 1;                   # Flag to control the thread to continue
$SIG{TERM} = sub { $run=0; };           # Setting signal handler as anonymous function
my $numthread = 2;                      # How many threads we want to run?
my $mean = 0.1;                         # Mean sleep time
my @threads;                            # Thread pool
sanitycheck();                          # sanity check the prob matrix & vectors
for (0..$numthread-1) {
    $threads[$_] = threads->new(\&markovtrail);
};

for (0..$numthread-1) {                 # If ^C received, join the threads
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
    my $before; my $after;                  # Initialize timers
    until (&$termcheck()) {
        $before = time;
        &{$funcvec[$state]}();          # Call the corresponding function
        $random = rand(1);              # Toss a coin
        $sum=0.0;                       # Lookup the transition matrix
        my $oldstate=$state;            #   and jump accordingly
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
```

## Emulating a browser by LWP

LWP stands for library for WWW in Perl. The simplest way of using it is to
download a piece of data through HTTP protocol:

```perl
use LWP::Simple;

# These two are equivalent...
my $data = get('http://example.net/index.php?var=data');
print $data if defined $data;

getprint('http://example.net/index.php?var=data');
```

This kind of usage provide convenience and code readability, but not
functionality. It cannot support the use of cookies and authorization. To do
these, you need to emulate a browser:

```perl
use LWP;
my $browser = LWP::UserAgent->new;                # Emulate a browser
$browser->agent('Mozilla/4.76 [en] (Win98; U)');  # claim as NS 4.76 on Win98

# instead of just declaring 'agent', we can declare a full header fields for later use
my @ns_headers = {
    'User-Agent' => 'Mozilla/4.76 [en] (Win98; U)',
    'Accept' => 'image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*',
    'Accept-Charset' => 'iso-8859-1,*,utf-8',
    'Accept-Language' => 'en-US' };

# Two ways to GET something and save in a HTTP::Response object
my $response = $browser->get('http://example.net/index.php?var=data');
my $response = $browser->get('http://example.net/index.php?var=data', @ns_headers);

# Check if response is 200, or print stuff like "404 File not found." thou' status_line
die "Download failed: ".$resonse->status_line
    unless $response->is_success;

# Check content type, print the content only if it is HTML
if ($response->content_type eq 'text/html') {
    print $response->content;
} else {
    # Print both full header and content otherwise
    print $response->as_string;
};
```

The above code shows how we can use GET method, to use POST method, things are similar:

```perl
# Two ways to POST something and save in a HTTP::Response object
my $response = $browser->post('http://example.net/index.php?var=data',
    [
        'var' => 'data',
        'foo' => 'bar',
    ]);
my $response = $browser->post('http://example.net/index.php?var=data',
    [
        'var' => 'data',
        'foo' => 'bar',
    ], @ns_headers);
```

For a complicated GET request, one may want to build the URI in a better way:

```perl
use URI;
my $url = URI->new('http://example.net/index.php');
$url->query_form(
    'var' => 'data',
    'foo' => 'bar' );
my $response = $browser->get($url);
```

In case cookies are required, a cookie jar should be assigned, with an optional
file for which to read/write cookies from/to.

```perl
use HTTP::Cookies;
#Different means of use:
$browser->cookie_jar(HTTP::Cookies->new(
    'file' => '/path/to/cookies.lwp',     # file to read/write, in proprietary format
    'autosave' => 1,                      # save file when done
));

$browser->cookie_jar(HTTP::Cookies::Netscape->new(
    'file' => '/path/to/cookies.txt',      # Netscape's format
));

$browser->cookie_jar({});        # Neither save to nor read from a file
```

Moreover, you can also add a bunch of HTTP authentication information to the browser like this:

```perl
$browser->credentials(
    'server.name.org:port',
    'realm-name',
    'username' => 'password'
);
```

After these, our code is much like a normal browser. But we still have the following things to tune up:

```perl
# Give up request if no response for $n seconds
$browser->timeout($n);

# Use the HTTP/1.1 "Keep-Alive" feature to reuse the same socket connection for speeding up requests
use LWP::ConnCache;
$browser->conn_cache(LWP::ConnCache->new());

# Make the browser honours http_proxy environment variable
$browser->env_proxy;
```

More detail about the other usage of LWP (and related HTTP stuff) can be found
over CPAN, following are some pointers:

  * HTTP::Message and HTTP::Headers provide the foundation for HTTP::Response
  * URI::Escape provide functions to escape and unescape URL strings (%20%21%22 stuff)
  * HTML::Entities provide functions to escape and unescape HTML strings (&amp;&gt;&lt; stuff)
  * HTML::TokeParser and HTML::TreeBuilder helps parsing HTML documents
  * HTML::LinkExtor helps finding links in a HTML document


## Real world example of using LWP
Now let's see how can we make use of LWP to crawl a web:

Assume we have a very simple web-based multiple choice system, where

  * Users have to load the index page first, where they can login
  * When a user login, it submit his info to another page for checking, which
    will forward the user to the main page when authenticated
  * The main page lists a set of problem for you to answer
  * You can either answer one, such will tell if you are correct immediately
  * or you can logout at the main page

To do this task, we can build a couple of code snippets, first of all, the function to crawl the index page and that to do login:

{% raw %}
```perl
# Create browser, supports cookie and follows redirect for POST requests
use LWP::UserAgent;
my $browser = new LWP::UserAgent;
$browser->cookie_jar({});
push @{$browser->requests_redirectable}, 'POST';

# Global variable for browser behaviour functions
my $server_addr = "http://192.168.123.456/";
my $user_id = "JohnDoe";
my $user_pw = "foobar";

# Global variable for storing multiple choice stuff
my @problems;

# Load the login page
sub getIndexPage {
    my $response=$browser->get($server_addr."dgp");
    if ($response->is_success) {
        my $content=$response->content;
        return 0 unless (index($content,"</html>")>=0);
        return 1 if ($content =~ m/this\.location\.href.*mainBody/s);
        return 2 if ($content =~ m/parent\.location\.href.*index/s);
    };
    return 0;
};

# Emulate login behavior
sub loginAndGetMainPage {
    my $response=$browser->post($server_addr."dgp/login.php",
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
```
{% endraw %}

The functions are returning zero if failure and other codes to mean success.
This is to help we check whether the web response correctly. In the function to
login and get the main page (which is the most complicated), we do the following
to check whether the web page returned by the server is complete and correct:

  - The web page should be a well-formed HTML, ending the the tag `</html>`
  - The web page should contain a `div` with the ID `main`, otherwise, the page is malformed (PHP error?)
  - There should be exactly 30 links to `problemset.php`, which are the links to
    problems to answer. The way to get the problem links is to use the regex
    matching with flag `/g` and collect the result by an array

This should show some idea on how you should program the function calls. Once
all sort of functions are created, we can build the transition probability
matrix, initial probability vector, and the function vectors. The control loop
in the Markov Chain trailwalker should also be modified to check the return code
of the functions called, and perform appropriate actions accordingly (e.g. flag
error if there is any).

## Knowing about web behaviour

One can see that, in the previous section, we build the web stress test program
by emulating a browser behaviour. But how can we know the browser behaviour?

This should be discusses in two aspects: From a microscopic points of view, we
should use `tcpdump` to see exactly what is provided by the browser and what is
obtained from the server. Items that we should pay attention to includes the
cookies, data passed via POST method, browser headers, to name a few. We can
also measure the response time, connection concurrency, and whether keep-alive
is in use by investigating `tcpdump` data.

From a macroscopic point of view, we can also make use of `tcpdump` to collect
user behaviour: we try to browse the web once with a real browser and record the
traversal pattern exactly. This includes obtaining the fully qualified URLs and
check whether some page is redirected via HTTP header, or the META tag on a HTML
page.

In other words, this part of work is very human and no way to automate (right?).
Given the Perl script, we should spend most of our time in knowing how to
program the crawling functions.


## Baseline code for Perlstone

[`perlstone.pl`](/img/perlstone.pl) - Multithreaded web stress test engine

```perl
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
````

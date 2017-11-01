#!/usr/bin/perl
# Copyright 2007 (c) Adrian Sai-wah Tam
# Wed Mar 28 10:26:05 HKT 2007
#
# Maximal matching for segmentation
# It tries with forward max matching first, then go with backward if patterns
# not found.
#
# Test case:
#   with dict.txt contains: ab abcd acde cdef def cdac dec defd efg
#   invoking with
#     echo abcdacdecdefdefgab | perl maxmatch3.pl
#   shall give: "ab/cdac/dec/defd/efg/ab";
#   invoking with
#     echo abcdacdec22defd33efgab | perl maxmatch3.pl
#   shall give: "ab/cdac/dec/22/defd/33/efg/ab" with new token "22/33";
#
# Version 3.0
# Added the functionality of new token detection

use strict;
use warnings;

my $TRUE=1;
my $FALSE=0;

my %dict;		# Dictionary of matching tokens (fmt: token=>1)
my $maxtoklen=0;	# Max token length
my $sep='/';		# Separator for printing the tokenized
my @newToks;		# Array to hold the newly found tokens

# Open and read-in the dictionary
open(DICT,"dict.txt") || die "Cannot open dictionary\n";
while(<DICT>) {
	chomp;
	$dict{$_} = 1;
	$maxtoklen = length($_) unless ($maxtoklen > length($_));
};
close(DICT);

# Read the string from console and tokenize it
while(<>){
	chomp;
	my @tokens = tokenize($_);
	print join($sep,@tokens) . "\n";
	print "New tokens found: " . join($sep,@newToks) . "\n" if (scalar(@newToks));
};

##########
# Function pool
# 

## Tokenize the supplied string
sub tokenize {
	my @tokens;	# Array of tokens
	my $rest=$_[0];	# The string not yet tokenized
	my $lastIsRollback = $FALSE;
	while (length($rest)) {
		my @result = matchOne($rest);	# Get the first token
		# Check if it is matched successfully
		if (length($result[0])) {
			push @tokens, $result[0];
			$rest = $result[1];
			$lastIsRollback = $FALSE;
			next;
		};
		# Is it a new token or mistake in maximal matching?
		if ($lastIsRollback) {
			getNewTok(\@tokens, \$rest);
			next;
		};
		# Now we see something wrong in maximal matching, rollback!
		my $brokenTok;
		$lastIsRollback = $TRUE;
		unless ( rollback(\@tokens, \$rest, \$brokenTok) ) {
			# Rollback failed, must be some new tokens there
			getNewTok(\@tokens, \$rest);
			next;
		};
		# Then, use reverse maximal matching to rectify the tokens
		unless ( revmatch(\@tokens, $brokenTok) ) {
			# if reverse match faild, $brokenTok is possibly a new token
			push @tokens, $brokenTok;	# Append to the tokens array
			push @newToks, $brokenTok;
			$dict{$brokenTok} = 1;		# and add to dictionary
			$maxtoklen = length($brokenTok) unless ($maxtoklen > length($brokenTok));
		};
		# Now we know that the next one must be a matching token, this is
		# to prevent flip-flop-like looped rectification
		last unless (length($rest));
		@result = matchOne($rest);
		$rest = $result[1];
		push @tokens, $result[0];
	};
	return @tokens;
};

## Find the leading token in the string by maximal matching
sub matchOne {
	my @result = ("", $_[0]);	# 1=token, 2=remaining

	# Remove leading spaces
	(my $sanitized = $result[1]) =~ s/^\s+//;
	# Extract substrings in various length and match with dictionary
	for(my $i=$maxtoklen; $i>=1; $i--) {
		my $substr = substr($sanitized,0,$i);
		# Update @result if something is found
		if ( defined($dict{$substr}) ) {
			$result[0] = $substr;
			$result[1] = substr($sanitized,length($substr));
			last;
		};
	};
	return @result;
};

## Rollback function: to search for the (possibily) correct boundary from
#                     the given array of tokens and the untokenized string
sub rollback {
	(my $refTok, my $refRest, my $refBrokenTok) = @_;
	my $rest = $$refRest;		# Copy of untokenized string
	my @tokens = @$refTok;		# Copy of tokens array
	my $brokenTok;			# Variable to hold the broken token
	my $lastTok;
	my @tok = ("","");		# Array to hold the search result
	# Loop to ensure we exhausted $maxtoklen possibilities
	for (my $lenTried=0; $lenTried<$maxtoklen;) {
		$tok[0]="";			#   clear the misleading indication
		last if (scalar(@tokens)==0);	# If no more things to rollback
		$lastTok = pop @tokens;
		$brokenTok = $lastTok;
		# Boundary-searching loop
		while (length($brokenTok)) {
			$rest = substr($brokenTok,-1) . $rest;
			$brokenTok = substr($brokenTok,0,-1);
			$lenTried++;
			@tok = matchOne($rest);		# Try if this is a boundary
			last if (length($tok[0]));	# Stop if found something
		};
		# Check if we need to continue the search
		last if ($lastTok ne $tok[0]);	# Check if we found something new
	};
	# Check the indication to see if we found anything, and report
	return $FALSE if ($lastTok eq $tok[0] || length($tok[0])==0);
	$$refRest = $tok[0].$tok[1];	# Update the info
	$$refBrokenTok = $brokenTok;
	@$refTok = @tokens;
	return $TRUE;
};

## Reverse maximal matching to rectify incorrect tokenization
sub revmatch {
	my $rest = $_[1]; 		# Copy of the broken token
	my @tokens = @{$_[0]};		# Copy of tokens array
	my @newToks;			# Array to store the new tokens
	my $brokenTok;			# Variable to hold the broken token in the process

	while (length($rest)) {
		# if $rest is already a token, we can stop here
		if ( defined($dict{$rest}) ) {
			unshift @newToks, $rest;
			last;
		};
		# Return if reverse match failed. (new token?)
		return $FALSE unless (rollback(\@tokens,\$rest,\$brokenTok));
		# Otherwise, try to get the new token
		my @result = matchOne($rest);
		# If the remaining if not zero, it is not reverse max matching
		return $FALSE if (length($result[1]));
		# Put the new token into array and continue matching
		unshift @newToks, $result[0];
		$rest = $brokenTok;
	};
	push @tokens, @newToks;
	@{$_[0]} = @tokens;
	return $TRUE;
};

## Get new token: Given the string is headed by a new token, extract it
#          and append to the token array
sub getNewTok {
	my $rest = ${$_[1]};	# Copy of the string
	my $token = "";		# Array to store the new token
	my @result = matchOne($rest);
	# The matching loop
	while (length($result[0])==0) {
		$token = substr($rest,0,1).$token;
		$rest = substr($rest,1);
		last unless (length($rest));
		@result = matchOne($rest);
	};
	# We are here either because we found the next dictionary word or
	# we exhaused the matching string, in either case, the new word
	# is in $token
	push @{$_[0]}, $token;	# Put the new word into array
	push @newToks, $token;	# and record it as a new word
	${$_[1]} = $rest;	# Update the string to be matched
	$dict{$token} = 1;	# and add to dictionary
	$maxtoklen = length($token) unless ($maxtoklen > length($token));
};

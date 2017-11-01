#!/usr/bin/perl
# Copyright 2007 (c) Adrian Sai-wah Tam
# Wed Mar 28 10:26:05 HKT 2007
#
# Maximal matching for segmentation
# It tries with forward max matching first, then go with backward if patterns
# not found.
#
# Version 2.0
# Updated the code to make it modularized and easier to read

use strict;
use warnings;

my $TRUE=1;
my $FALSE=0;

my %dict;		# Dictionary of matching tokens (fmt: token=>1)
my $maxtoklen=0;	# Max token length
my $sep='/';		# Separator for printing the tokenized

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
};

##########
# Function pool
# 

## Tokenize the supplied string
sub tokenize {
	my @tokens;	# Array of tokens
	my $rest=$_[0];	# The string not yet tokenized
	while (length($rest)) {
		my @result = matchOne($rest);	# Get the first token
		# Check if it is matched successfully
		if (length($result[0])) {
			push @tokens, $result[0];
			$rest = $result[1];
			next;
		};
		# Now we see something wrong in maximal matching, rollback!
		my $brokenTok;
		rollback(\@tokens, \$rest, \$brokenTok) || die "Rollback failed.\n";
		# Then, use reverse maximal matching to rectify the tokens
		revmatch(\@tokens, $brokenTok) || die "Reverse match failed: New tokens?\n";
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
	my @tok = ("","");		# Array to hold the search result
	# Loop to ensure we exhausted $maxtoklen possibilities
	for (my $lenTried=0; $lenTried<$maxtoklen;) {
		if (scalar(@tokens)==0) {	# If no more things to rollback
			$tok[0]="";		#   clear the misleading indication
			last;			#   and stop
		};
		$brokenTok = pop @tokens;
		my $lastTok = $brokenTok;
		# Boundary-searching loop
		while (length($brokenTok)) {
			$rest = substr($brokenTok,-1) . $rest;
			$brokenTok = substr($brokenTok,0,-1);
			$lenTried++;
			@tok = matchOne($rest);	# Try if this is a boundary
			last if (length($tok[0]));	# Stop if found something
		};
		# Check if we need to continue the search
		last if ($lastTok ne $tok[0]);	# Check if we found something new
	};
	# Check the indication to see if we found anything, and report
	if (length($tok[0])) {
		$$refRest = $tok[0].$tok[1];	# Update the info
		$$refBrokenTok = $brokenTok;
		@$refTok = @tokens;
		return $TRUE;
	} else {
		return $FALSE;
	};
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

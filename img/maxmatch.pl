#!/usr/bin/perl
# Copyright 2007 (c) Adrian Sai-wah Tam
# Tue Mar 27 13:20:03 HKT 2007
#
# Maximal matching for segmentation
# It tries with forward max matching first, then go with backward if patterns
# not found.
#
# Version 1.0

use strict;
use warnings;

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
	my $reverted;	# Reverted token
	my $rest=$_[0];	# The string not yet tokenized
	while (length($rest)) {
		my @result = tokenizeOne($rest);	# Get the first token
		# Check if it is matched successfully
		if (length($result[0])) {
			push @tokens, $result[0];
			$rest = $result[1];
			next;
		};
		# something went wrong in maximal matching, rollback!
		$reverted = pop @tokens;
		defined($reverted) || die "Token array is empty. Cannot revert.\n";
		my @rbResult = rollback($reverted,$rest);
			# @rbResult = (matched token, rest string, remaining token)
		# verify if we can find something by rolling back
		if ($reverted eq $rbResult[0]) {
			# Rollback matching failed: We returned to the same state
			die "Rollback matching failed.\n";
		};
		# scanning backward to rectify the wrong stuff
		my @tempTokens = ($rbResult[0]);	# temporary token array
		$rest = $rbResult[1];			# The string not yet tokenized
		my @revResult = revMatch($rbResult[2]);	# The stuff to match reversely
		my $notmatched = pop @revResult;
		unshift @tempTokens, @revResult;
		if (length($notmatched)==0) {
			# We are done! No more stuff to match
			push @tokens, @tempTokens;
			next;
		};
		# Continue to do reverse match until either the token array is empty
		# or we have no more stuff to match
		while (length($notmatched) && scalar(@tokens)) {
			@revResult = revMatch(pop(@tokens).$notmatched);
			$notmatched = pop @revResult;
			unshift @tempTokens,@revResult;
		};
		# If we emptied the @tokens, put what is not matched into it
		if (scalar(@tokens)) {
			push @tokens, $notmatched;
		};
		# then, append @tempTokens to @tokens
		push @tokens, @tempTokens;
	};
	return @tokens;
};

## Find the leading token in the string by maximal matching
sub tokenizeOne {
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

## Rollback one token and redo maximal matching 
sub rollback {
	(my $reverted, my $rest) = @_;
	my @result;
	do {
		$rest = substr($reverted,-1) . $rest;
		$reverted = substr($reverted,0,-1);
		@result = tokenizeOne($rest);
	} while(length($result[0])==0);
	$result[2] = $reverted;
	return @result;
};

## Reverse maximal matching for one token
sub reverseOne {
	my @result = ("",$_[0]);

	# Remove trailing spaces
	my $sanitized = $_[0];
	$sanitized =~ s/\s+$//;
	# Extract substrings in various length and match with dictionary
	for(my $i=$maxtoklen; $i>=1; $i--) {
		my $substr = substr($sanitized,-$i);
		# Update @result if something is found
		if ( defined($dict{$substr}) ) {
			$result[0] = $substr;
			$result[1] = substr($sanitized,0,-$i);
			last;
		};
	};
	return @result;
};

## Reverse maximal matching for as much tokens as possible
sub revMatch {
	my @tokens;
	my $string = $_[0];
	while(length($string)) {
		# match for one token
		my @result = reverseOne($string);
		# if found, put into array, else, break the loop
		if (length($result[0])) {
			unshift @tokens, $result[0];
			$string = $result[1];
		} else {
			last;
		};
	};
	# put the unmatched string (whatever) to the array, and return
	push @tokens, $string;
	return @tokens;
};

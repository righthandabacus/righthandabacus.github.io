---
layout: post
title: "Perl One-Liners and other tricks"
date: 2012-01-27 22:39:30 -0500
tags: code
---

## Set operations in Perl

Assume we use arrays as set:

```perl
my @odds = (1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25);
my @primes = (2, 2, 3, 5, 7, 11, 13, 17, 19, 23);
```

then we can do the following set operations as one-liners:

Check membership:
```perl
my $OddsHasTwo = scalar(grep{$_ == 2}@odds);```

Find unique members:
```perl
my @uniq = sort {$a<=>$b} keys %[map{$_=>1}@primes}];```

Union:
```perl
my @union = sort {$a<=>$b} keys %[map{$_=>1}(@odds,@primes)}];```

Intersection:
```perl
my @inter = grep {my $i=$_;scalar(grep{$_==$i}@primes)} @odds;```

Difference: (`@odds - @primes`)
```perl
my @diff = grep {my $i=$_;scalar(grep{$_==$i}@primes)==0} @odds;```

## Print a 2-D array using map command

```perl

push(@a,[1,2,3]);
push(@a,[2,4,6]);
push(@a,[3,6,9]);
print join("\n",map{ join("", map {sprintf("%3d",$_)} @$_) } @a)."\n";
```

output:
```
  1  2  3
  2  4  6
  3  6  9
```

## Summation
Any of the below prints "6":

```
echo -e '1\n2\n3' | perl -e 'map{$x+=$_}<>;print $x;'
echo -e '1\n2\n3' | perl -e 'while(<>){$x+=$_};print $x;'
echo -e '1\n2\n3' | perl -e 'print((map{$x+=$_}<>)[-1]);'
echo -e '1\n2\n3' | perl -ne '$x+=$_;END{print $x};'
```

## Indenting here documents

```perl
($blah = <<'    ENDHEREDOC') =~ s/^ {8}//gm;
        blah blah blah blah
        blah blah blah blah blah blah
        blah blah blah blah blah
    ENDHEREDOC
```

also, it is better to quote the here document tags, so that you can have a semicolon:

```perl
$blah = <<'ENDHEREDOC';
blah blah blah
ENDHEREDOC
```

# Get a piece of text from a matching
In other words, get something from a pattern without using substitution:

```perl
($piece = $whole) =~ (/regex(piece)/)[0];
```

## Removing consecutive, duplicated characters
That is, to make `heello` to become `helo`:

    s/(.)\1/\1/g;

or

    tr///cs;

The `s` line is trival. The `tr` line means to have everything (the complement `c` of nothing `<nowiki>//</nowiki>`) and its consecutives to replace by itself.

## Converting every word into first-letter uppercase
Such As This One

    s/(\w+)/\u\L$1/g;

The `\u` and `\L` are modifiers to make the next character(s) to be upper or lower cases.

## Capturing nested parenthesis
Such as capturing the while function call to `foo()` in `foo(a+(b+c)+(d+(e+f)))+g+(h+i)`. The following piece of code is from "Mastering Regular Expression" by Friedl.

```perl
my $string = "foo(a+(b+c)+(d+(e+f)))+g+(h+i)";
my $nest;
$nest = qr/\( ( [^()] | (??{$nest}) )* \)/x;
$string =~ m/(foo$nest)/;
print "Function call is $1\n";
```

The `(??{code})` construct is used to embed a piece of code into a regex, which a nesting is done.

## Separating CSV data
Where the CSV data may include commas inside a pair of quote and quotes may be escaped inside quotes. The following piece of code is from "Mastering Regular Expression" by Friedl.

```perl
@new = ();
push(@new, $+) while $text =~ m{
     "([^\"\\]*(?:\\.[^\"\\]*)*)",?  # groups the phrase inside the quotes
   | ([^,]+),?
   | ,
}gx;
push(@new, undef) if substr($text,-1,1) eq ',';
```

## Schwartzian Transform
A way to speed up the sorting process by Randal L Schwartz:

```perl
@sorted = map  { $_->[0] }
          sort { $a->[1] cmp $b->[1] }
          map  { [ $_, foo($_) ] } @data;
```

The above is to get a bunch of items from `@data`, and construct a temporary 2-column array for sorting (i.e. calling `foo()` for N times only). Then sort the 2-column array according to the string order of the 2nd column, and then extract the first column as the sorted result.

## Opening a really temporary file
Passing `undef` as filename:
```perl
    open my $tmp, '+>', undef or die $!;
```
or you can open a temporary file at RAM if you're using Perl 5.8+:
```perl
  open(my $wr, '>', \$string) or die "Couldn't write to string";
  open(my $rd, '<', \$string) or die "Couldn't read string";
```

---
layout: post
title: "Binary Tricks"
date: 2010-04-09 14:40:34 -0400
tags: code
---

These are some binary tricks that I collected on the web. Useful or not. Some of them are the reinventing-the-wheel product of mine. But it is nice to keep a record here.

## Return right-most bit

Return right-most bit set, either one:

```c
x & -x
x & (~x + 1)
```

Return right-most bit unset:

```c
~x & (x + 1)
```

By the similar logic, the increment and decrement can be done bitwise:

```c
x = -(~x);  // x++
x = ~(-x);  // x--
```

And also doing negation:

```c
x = (~x)+1;     // x = -x
x = (x ^ -1)+1; // x = -x
```

So we also have the following two ways to do absolute value (assume integer of 32-bit):

```c
(x ^ (x>>31)) - (x>>31) // (x>0)?x:-x
(x ^ (x>>31)) + (unsigned(x)>>31)
```

## Comparing integers of equal sign

```c
if ((a ^ b) >= 0)  // Same as a*b > 0
```

But note, when any of the two integer is zero, the construct is not exactly equal: In this case, zero is treated as positive.

## Even/Odd check

```c
if ((a & 1) == 0) // Same as a%2 == 0
```

## Reset the right-most bit

Right-most set-bit become unset

```c
x & (x - 1)
```

We can check if a positive number is a power of 2 by verifying `x & (x-1) == 0`. Alternative ways to check is `x & -x == x`.

## Get the next power of 2

Simply speaking, find the MSB set, and left shift by 1. The key idea below is to set all bits after the MSB set, then increment it.

```c
x |= x>>1;
x |= x>>2;
x |= x>>4;
x |= x>>8;
x |= x>>16;
x++;
```

But the above code will convert a power of 2 to the next power of 2 rather than
staying there. Thus it is not "rounding" to next power of 2 exactly. To do
rounding, i.e. not modifying the value if it is already a power of 2, do a `x--`
first.

## Tell the position of the right-most set bit

```c
popcount(x ^ (x-1))
```

## Reverse bits

```c
x = ((x>>1) & 0x55555555) | ((x<<1) & 0xAAAAAAAA);
x = ((x>>2) & 0x33333333) | ((x<<2) & 0xCCCCCCCC);
x = ((x>>4) & 0x0F0F0F0F) | ((x<<4) & 0xF0F0F0F0);
x = ((x>>8) & 0x00FF00FF) | ((x<<8) & 0xFF00FF00);
x = ((x>>16)& 0x0000FFFF) | ((x<<16)& 0xFFFF0000);
```

# Rounding

If we want to round a number `x` to the *lower* multiple of `y` (i.e. round down), we can do this:

```c
int x;
x = (int)(x/y)*y;
```

To round to the *upper* multiple of `y` (i.e. round up), it is:

```c
x = (int)((x+y-1)/y)*y;
```

To round off, i.e. to the nearest multiple of `y`, it is:

```c
x = (int)((x+y/2)/y)*y;
```

## Guarantee a positive/negative value

For an integer `x`, make it zero if it is positive/negative and leave it
untouched otherwise. To make sure it is non-positive, mask it with 0 if its MSB
is 0:

```c
x &= x >> 31;
```

To make sure it is non-negative, mask it with 0 if its MSB is 1:

```c
x &= (~x) >> 31;
```

## Identities

```c
x + y == (x & y) + (x | y)
x | y == (x ^ y) + (x & y)
```

----

More:
  - <http://graphics.stanford.edu/~seander/bithacks.html>
  - <http://bob.allegronetwork.com/prog/tricks.html>

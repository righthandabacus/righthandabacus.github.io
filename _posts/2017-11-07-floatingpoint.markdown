---
layout: post
title: Floating point endianness
subtitle: the case of IEEE 754 binary64
date: Tue, 07 Nov 2017 12:54:05 -0500
tags: code
---

Floating point numbers have a standard -- IEEE 754. But how exactly those
numbers stored? Below is a piece of code that shows it:

```c++
#include <iostream>
#include <iomanip>
#include <set>

using namespace std;

int main(int argc, char**argv)
{
    std::multiset<double>* mset = new std::multiset<double>();
    mset->insert(3.14);
    mset->insert(6.28);
    mset->insert(2.71);
    mset->insert(0.61);
    mset->insert(1.61);
    mset->insert(9.89);
    mset->insert(0.5);
    for(std::multiset<double>::const_iterator it = mset->cbegin(); it != mset->cend(); it++) {
        double val = *it;
        cout << val << " = ";
        unsigned char* blobchar = reinterpret_cast<unsigned char*>(&val);
        cout << hex << uppercase << internal << setfill('0');
        for(unsigned i=0; i<sizeof(double); ++i) {
            cout << setw(2) << (unsigned)blobchar[i] << " ";
        };
        cout << dec << endl;
    };
    delete mset;
    return 0;
}
```

In my x86 Linux box, this is the output:

```
0.5 = 00 00 00 00 00 00 E0 3F 
0.61 = 85 EB 51 B8 1E 85 E3 3F 
1.61 = C3 F5 28 5C 8F C2 F9 3F 
2.71 = AE 47 E1 7A 14 AE 05 40 
3.14 = 1F 85 EB 51 B8 1E 09 40 
6.28 = 1F 85 EB 51 B8 1E 19 40 
9.89 = 48 E1 7A 14 AE C7 23 40 
```

The first line, a round number in binary, is a clue that the `double` is indeed
saved as little endian because the Intel x86 architecture is little endian.
Take the second line as an example, we tie out the representation to validate
that it is actually the case:

IEEE 754 [binary64 format](https://en.wikipedia.org/wiki/Double-precision_floating-point_format):
- sign bit: 1 bit (bit 0)
- exponent: 11 bits (bit 1 to bit 11)
   - biased (0 to 2047 for representing $e+1023$) or unbiased ($-1024$ to $1023$ in two's complement)
   - exponent of $-1023$ and $+1024$ are reserved
- mantissa: 53 bits with 52 bits explicitly stored (bit 12 to bit 63)
   - with implicit integer bit of value 1 except for special values
   - guaranteed precision of 15 significant decimal digits ($53\log_{10}2 = 15.95$)

Reversing for big-endian order, 0.61 becomes
```
3F E3 85 1E B8 51 EB 85
```
- sign bit: `0`
- exponent: `0x3FE` (`=011 1111 1110`)
  - 1022 in decimal, biased representation of $1022-1023 = -1$
- mantissa: `0x3851EB851EB85` (`=0011 1000 0101 0001 1110 1011 1000 0101 0001 1110 1011 1000 0101`)

Converting to binary, it is:
$$
1.0011\ 1000\ 0101\ 0001\ 1110\ 1011\ 1000\ 0101\ 0001\ 1110\ 1011\ 1000\ 0101 \times 2^{-1}
$$
and it matches the mantissa of 0.61:
```
0 . 61
------
1 . 22
0 . 44
0 . 88
1 . 76
1 . 52
1 . 04
0 . 08
0 . 16
0 . 32
0 . 64
1 . 28
0 . 56
1 . 12
0 . 24
0 . 48
0 . 96
1 . 92
1 . 84
1 . 68
1 . 36
0 . 72
1 . 44
0 . 88
...
```

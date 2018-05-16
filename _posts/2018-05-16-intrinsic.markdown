---
layout: post
title: Using Intel Intrinsics
date: Wed, 16 May 2018 16:20:29 -0400
tags: code
---

Intel intrinsics is available in many compliers (VC++, gcc, clang, icc) to
allow SIMD programming using vector instruction sets from MMX to AVX2/3.

Below is an example, the key thing is the `x86intrin.h` header and the `__m128` data type:

```cpp
// adapted from https://gist.github.com/bellbind/38d7ae4c32d6babda916
// compile with: clang -std=c11 -msse3 example.c -o example
#include <x86intrin.h>
#include <stdio.h>

int main() {
    float mem[4];
    __m128 v = _mm_set_ps(2, 3, 5, 7);
    __m128 sq = _mm_mul_ps(v, v);
    _mm_store_ps(mem, sq);
    printf("%f, %f , %f, %f\n", mem[0], mem[1], mem[2], mem[3]);
    return 0;
}
```

The SIMD instructions are for *fine-grain parallelism* which in the ideal we
shouldn't care because the complier should be smart enough to use it as part of
code optimization. But if we do, we are dealing with a pack of $n$ primitive
data types (e.g.  float, ouble, int) into $k$ bits. For example, in SSE3 we
have registers of 128 bits (AVX has 256 bits) that holds the following:
```cpp
__m128  f; /* = 4 float, SSE */
__m128d d; /* = 2 double, SSE2 */
__m128i i; /* = integers, can be 8-128 bits */
```

Operation on vector types are, like the example above, basically construct
vector from scalars, populate array of scalars from vector, and manipulation on
vectors. The following

    __m128 v = _mm_set_ps(2, 3, 5, 7);

set vector `v` with four single precision floats, with the first one (2) at the
MSB side of the vector and last (7) at the LSB side. To dump the vector back to
array of scalar,

    _mm_store_ps(mem, sq);

so the first element of the array (`mem[0]`) is the one packed at the LSB side
of the vector. Finally the operation

    __m128 sq = _mm_mul_ps(v, v);

is to manipulate on vectors and return another vector. Basically all those SIMD
instructions are using a prefix of `_mm` and imported by the header `x86intrin.h`.
Intel has an [intrinsics guide](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)
for us to look up the usage of these hundreds of functions, and there is a
[cheatsheet](https://db.in.tum.de/~finis/x86-intrin-cheatsheet-v2.1.pdf)
that summarize them.

Below is another example, trying to plot Mandelbrot set on text screen:

```cpp
// adapted from https://gist.github.com/bellbind/38d7ae4c32d6babda916
// compile with: clang -std=c11 -msse3 mandel.c -o mandel
#include <stdio.h>
#include <complex.h>
#include <x86intrin.h>

/* complex norm using SIMD: |(z1j+z0)| = z0^2 + z1^2
 */
static inline double cnorm(const __m128d z) {
    const __m128d sq = _mm_mul_pd(z, z);      // _mm_mul_pd : (a1,a0)x(b1,b0) -> (a1*b1, a0*b0)
    const __m128d norm = _mm_hadd_pd(sq, sq); // _mm_hadd_pd : (a1,a0)x(b1,b0) -> (b1+b0, a1+a0)
    double mem[2];
    _mm_store_pd(mem, norm);
    return mem[0]; // mem[0] == mem[1] here
}
/* complex multiplication: (w1j+w0)*(z1j+z0) = (w1z0+w0z1)j + (w0z0-w1z1)
 */
static inline __m128d cmul(const __m128d w, const __m128d z) {
    const __m128d w0 = _mm_unpacklo_pd(w, w); // _mm_unpacklo_pd : (a1,a0)x(b1,b0) -> (a0,b0)
    const __m128d w1 = _mm_unpackhi_pd(w, w); // _mm_unpacklo_hi : (a1,a0)x(b1,b0) -> (a1,b1)
    const __m128d c = _mm_mul_pd(w0, z);      // c == (w0*z1,w0*z0)
    const __m128d z_ = _mm_shuffle_pd(z, z, 0b01);// _mm_shuffle_pd( (a1,a0), (b1,b0), 0b01) -> (b0,a1)
    const __m128d d = _mm_mul_pd(w1, z_);     // d == (w1*z0,w1*z1)
    return _mm_addsub_pd(c, d);               // _mm_addsub_pd : (a1,a0)x(b1,b0) -> (a1+b1,a0-b0)
}
/* mandelbrot: packed double as complex number c, z_0 = c, iterative i<50 times
 * on z := z*z+c and find the threshold of i for |z|>4
 */
static int mandel(const __m128d c) {
    __m128d z = c;
    for (int i = 0; i < 50; i++) {
        if (cnorm(z) > 4.0) return i;
        z = _mm_add_pd(cmul(z, z), c);
    }
    return -1;
}
static char plot(int n) {
    return (n == -1) ? '@' :
           (n > 10)  ? '*' :
           (n > 5)   ? '+' :
           (n > 3)   ? '-' :
           (n > 2)   ? '.' :
                       ' ';
}
int main() {
    // screen size 80x24 chars
    const int w = 150, h = 50;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // complex coordinate at (x,y), mapping the screen onto [-1,1]x[-1,1]
            const double re = 2.0 * (2.0 * x / w - 1.0), im = (2.0 * y / h - 1.0); 
            putchar(plot(mandel(_mm_set_pd(im, re))));
        }
        putchar('\n');
    }
    return 0;
}
```

Output:
```
                                                  ...........------+++*+++*@--........                                                                
                                                ............------++++***+++---........                                                               
                                              .............------+++++***+++-----........                                                             
                                            .............-------++*++*@@@*+++------.......                                                            
                                          ..............-------+*****@@@*@**++-------......                                                           
                                        ..............--------++++*@@@@@@@@*+++--------.....                                                          
                                       ............---------+++++**@@@@@@@@*++++---------....                                                         
                                     ............----+++++++++++++@@@@@@@@@*+++++----------...                                                        
                                   ...........-----+*+***+++**++****@@@@@@***+++*++++++++*--...                                                       
                                 .........--------+++*****++*@@*@@@@@@@@@@@@*@**@*++++++*++--..                                                       
                               .......------------+++*@@@@***@@@@@@@@@@@@@@@@@@@@**+*******+--..                                                      
                             .....---------------++++*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**@@@***---...                                                     
                           ....------------------++++****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+----..                                                     
                        ....-------------------++*****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+++----...                                                    
                    .....--+----------------+++++**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+++---...                                                    
                .......---++*++++++++++++++++++++***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+++*--....                                                   
           ..........------+*+++++++**++++++++++***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@***+--...                                                   
        ...........-------++*****+++***+*+++++++**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---....                                                  
      ...........--------+++++*******@*****++++*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**++---....                                                  
    ............--------+++++***@@@@@@@@@**@****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---....                                                  
   ...........---------++++*+***@@@@@@@@@@@@****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---.....                                                 
  ...........--------+++++++*@@@@@@@@@@@@@@@@***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+----.....                                                 
 ..........--------+++**+****@@@@@@@@@@@@@@@@@@*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*++----.....                                                 
 ..---...--------++++++******@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*++-----.....                                                 
 ------+++++++++++++++**@@@@*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+++-----.....                                                 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**++++-----.....                                                 
 ------+++++++++++++++**@@@@*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+++-----.....                                                 
 ..---...--------++++++******@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*++-----.....                                                 
 ..........--------+++**+****@@@@@@@@@@@@@@@@@@*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*++----.....                                                 
  ...........--------+++++++*@@@@@@@@@@@@@@@@***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+----.....                                                 
   ...........---------++++*+***@@@@@@@@@@@@****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---.....                                                 
    ............--------+++++***@@@@@@@@@**@****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---....                                                  
      ...........--------+++++*******@*****++++*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**++---....                                                  
        ...........-------++*****+++***+*+++++++**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+---....                                                  
           ..........------+*+++++++**++++++++++***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@***+--...                                                   
                .......---++*++++++++++++++++++++***@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+++*--....                                                   
                    .....--+----------------+++++**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+++---...                                                    
                        ....-------------------++*****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+++----...                                                    
                           ....------------------++++****@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**+----..                                                     
                             .....---------------++++*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@**@@@***---...                                                     
                               .......------------+++*@@@@***@@@@@@@@@@@@@@@@@@@@**+*******+--..                                                      
                                 .........--------+++*****++*@@*@@@@@@@@@@@@*@**@*++++++*++--..                                                       
                                   ...........-----+*+***+++**++****@@@@@@***+++*++++++++*--...                                                       
                                     ............----+++++++++++++@@@@@@@@@*+++++----------...                                                        
                                       ............---------+++++**@@@@@@@@*++++---------....                                                         
                                        ..............--------++++*@@@@@@@@*+++--------.....                                                          
                                          ..............-------+*****@@@*@**++-------......                                                           
                                            .............-------++*++*@@@*+++------.......                                                            
                                              .............------+++++***+++-----........                                                             
                                                ............------++++***+++---........                              
```

Brief timeline of the Intel's evolution of vector instruction set extensions:
 - 1996, Pentium, MMX
   - integers, 64-bit = 2x32 or 8x8
 - 1999, Pentium III, SSE
   - 4 single precision float
   - 8 128-bit registers
 - 2001, Penium 4, SSE2
   - 2-way float ops in double precision
 - 2004, Pentium 4E Prescott, SSE3
   - 2-way and 4-way vector instructions
 - 2006, Core Duo, SSSE3
 - 2007, Core2 Duo Penryn, SSE4
 - 2011, Sandy Bridge, AVX
 - 2013, Haswell, AVX2

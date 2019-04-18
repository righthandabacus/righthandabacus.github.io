---
layout: post
title: Karatsuba method of multiplication
date: Thu, 18 Apr 2019 02:56:00 -0400
tags: math
---

This is a [way of doing
multiplication](https://www.wired.com/story/mathematicians-discover-the-perfect-way-to-multiply/)
of large numbers that I learned today. It allows the multiplication below $$O(n^2)$$ complexity.

Consider the simple case of two multiple digit decimal numbers, written in the
form of $$a\times 10^k+b$$ and $$c\times 10^k + d$$ for some positive integer
$$k$$. Then their product is

$$ac\times 10^{2k} + (ad+bc)\times 10^k + bd$$

But we can find that $$(a+b)\times (c+d) = ac+ad+bc+bd$$ so we can simply
calculate the product by the algorithm below:

1. Multiplication: $$ac$$ and $$bd$$
2. Addition: $$a+b$$ and $$c+d$$
3. Multiplication: $$(a+b)\times(c+d)$$
4. Subtraction: $$ad+bc = (a+b)(c+d) - ac - bd$$
5. Addition, with digit shifting: $$ac\times 10^{2k} + (ad+bc)\times 10^k + bd$$

So if each of $$a,b,c,d$$ are of $$k$$ digits, the naive multiplication will do
$$(2k)^2$$ single-digit multiplications. The Karatsuba method will do:

1. $$2k^2$$ single-digit multiplication
2. $$2k$$ single-digit addition
3. $$k^2$$ single-digit multiplication
4. $$2k$$ single-digit subtraction, twice
5. $$2k$$ single-digit addition, twice

So a total of $$3k^2$$ multiplications and $$10k$$ addition/subtractions, which
is an obvious reduction from $$4k^2$$ multiplications. Recursive application
gives a complexity of $$\Theta(n^{\log_2 3})$$ for multiplying two $$n$$-digit
numbers.

Generalizing the Karatsuba method, we can assume the two numbers are of the
form $$a\times p^k+b$$ and $$c\times p^k+d$$ for a $$p$$-ary number. But we
need both $$b$$ and $$d$$ to be $$k$$ digits to apply this method.

If we do not only split a $$n$$-digit number into two, like above, but $$m\ge 2$$
numbers, then we have the [Toom-Cook multiplication](https://en.wikipedia.org/wiki/Toom%E2%80%93Cook_multiplication),
of complexity $$\Theta(c(m)n^\epsilon)$$ where $$c$$ the complexity of addition
of small constants, $$n^\epsilon$$ the time for sub-multiplication; $$\epsilon = \log(2m − 1) / \log(m)$$


## Reference

- <https://brilliant.org/wiki/karatsuba-algorithm/>
- The picture below from <https://www.wired.com/story/mathematicians-discover-the-perfect-way-to-multiply/> makes the method easy to understand

![](https://media.wired.com/photos/5cb10d6b439ed155d5df38cc/master/w_700,c_limit/KaratsubaMethod_560-1065x1720.jpg)


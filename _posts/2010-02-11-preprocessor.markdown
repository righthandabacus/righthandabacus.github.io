---
layout: post
title: "Using C Preprocessors"
date: 2010-02-11 19:19:10 -0500
tags: code
---

The well-known `#define` and `#if..#endif` is no big deal. But from time to
time, we might want to use some features that provided by the CPP.

## Concatenation of tokens

All input to CPP are text strings and therefore we can do concatenation of strings like this:

    #define NAMEVALUE(x,y,z) x##y(z)

Then by doing

    NAMEVALUE(pri,ntf,"hello world\n");

it resolves to

    printf("hello world\n");

## Quotes

To put stuff as string, we are not using `"x"` but `#x`, for example

    #define DEBUGPRINT(x) printf("%s = %d\n",#x,x)

Then doing

    DEBUGPRINT(x);

will output something like

    x = 3

## Predefined Macros

  * `__FILE__` resolves into the filename of the source code file.
  * `__LINE__` resolves into the source line number
  * `__DATE__` and `__TIME__` resolves into the compile date and time
  * `__func__` resolves into the function name, only in C99

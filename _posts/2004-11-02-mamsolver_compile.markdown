---
layout: post
title: "How to Compile MAMSolver"
date: 2004-11-02 22:20:19 +0800
tags: code
---

[MAMSolver](http://www.cs.wm.edu/MAMSolver/) is a piece of software by Alma Riska from The College of William and Mary. I got it through email as described in that site and I found it contains an ELF binary but not usable by me due to shared library problems. Hence I compile another version for my own use.

I am using Debian and hence I can nearly apt-get anything I wanted.

The MAMSolver package requires the numerical libraries `lapack` and `blas` to work. And it contains mixed codes of C and Fortran. Hence I did this:

    # apt-get install make gcc g77 blas-dev lapack-dev

and also these for the run-time libraries of lapack and blas:

    # apt-get install lapack blas

Afterwards, I just extract the tarball, and run `make`. That's it.

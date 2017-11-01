---
layout: post
title: "Unmaintainable code"
date: 2005-11-22 00:13:15 +0800
tags: code business
---

This is a very funny, nice piece of article for [writing unmaintainable code](http://thc.org/root/phun/unmaintain.html).
Should be read by every programmer.

  - the maintenance programmer has no time to read all the code to understand
    it. He can only see a tiny piece at a time. To make code unmaintenable,
    1. make people not getting the big picture from a tiny piece
    2. make people feel safe to ignore anything
  - naming skill:
    - single letter variable names
    - baby names
    - misspellings and misleading names
    - abstract names
    - acronyms
    - use alternative vocab
    - inconsistent capitalisation or underscore use or plural use
    - reuse of variables
    - Hungarian notation with too much detail
  - camouflage
    - comment out codes
    - in C but not C++, struct and typdef struct are in different namespaces
    - macro definitions
    - make search hard by breaking up codes with line continuation characters
    - make variables look similar
    - abuse of overloading
  - bad documentation
    - lie in comments or comment the obvious
    - document how but not why
    - missing out use of measure of variables
    - referencing design documents (e.g., feature_1 as function name)
    - too much redundant boilerplate comments at every file, make it easy going out of date
  - design
    - casting variables a lot
    - avoid assert() or validate or encapsulation
    - clone and modify code rather than making resuable modules
    - bad OO design:
      - friend classes
      - mix of accessor methods and public variables
      - wrap or expose and unnecessary
      - different classes in very minor detail changed
      - abuse of static methods or variables
    - globals, environment variables, etc.
 - long lines, high nesting depth
 - exceptions
 - confounding imports: `import something.*` for no one knows what's there
 - keep no test code, which reveals how code should be used

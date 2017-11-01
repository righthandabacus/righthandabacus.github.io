---
layout: post
title: "Python Strings"
date: 2012-11-06 12:17:24 -0500
tags: code
---
When you make a string in Python, but the string shall look like multiple lines in the source code, the following doesn't work:

``` python
id = "line1 "
     "and line2"
```

But we can do this trick to make it work:
``` python
id = (
     "line1 "
     "and line2 "
     "and more"
     )
```

As long as Python knows something is still on-going (e.g. the parentheses), it concatenates the strings for you.

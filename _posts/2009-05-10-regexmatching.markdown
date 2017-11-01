---
layout: post
title: "Regex to check matching parenthesises"
date: 2009-05-10 01:33:41 -0400
tags: code
---

From <http://perl.plover.com/yak/regex/samples/slide083.html>

```perl
  print "Matched\n" if ('( ( I ) ( l i k e ( p i e ) ) ! )' =~ /
     ^
     (?{ local $d=0 }) # Set depth to 0
     (?:                  
         \(            # When you see an open parenthesis...
         (?{$d++})     #  ...increment the depth
      |  
         \)            # or you could see a close parenthesis...
         (?{$d--})     #  ...in which case decrement the depth...
         (?            #  ...and check...
           (?{$d<0})   #  ...if there was no matching open paren...
           (?!)        #  ...then fail.
         )  
      |      
         (?> [^()]* )  # or you could see some non-parenthesis text
                       # (but don't bother backtracking into it)
     )* 
                       # After you match as much as possible...
     (?                # ...check to see if...
       (?{$d!=0})      # ...there were unmatched open parentheses...
       (?!)            # ...if so then fail.
     )
     $
  /x);
```

---
layout: post
title: "Threading performance checker using GDB"
date: 2010-04-13 23:22:18 -0400
tags: code
---
```sh
gdb -ex “set pagination 0″ -ex “thread apply all bt” –batch -p ${pid}  |
awk ‘BEGIN { s = “”; } 
         /Thread/ { print s; s = “”; }
         /^\#/ { if (s != “” ) { s = s “,” $4} else { s = $4 } } 
         END { print s }’ – | 
sort |
uniq -c |
sort -r -n -k 1,1
```

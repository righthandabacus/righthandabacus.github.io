---
layout: post
title: "Unicode in VIM"
date: 2004-11-02 22:20:19 +0800
tags: system
---

How to set VIM to make it knows unicode? It is unbelievably easy.

VIM nowadays already prepared to work with multi-byte character sets. What lacks
is just to enable it. I do in this way:

```
# cat >> /etc/vim/vimrc.local
set fileencodings=utf-8,big5,euc-jp,gbk,euc-kr,utf-bom,iso8859-1
set encoding=utf8
set tenc=utf8
^D
```

The first line is to tell VIM prepare for those encodings. The second line is to
ask VIM to assume the source file is in UTF-8 by default. The third line means
when VIM saves a file, make it encoded in UTF-8 as well.

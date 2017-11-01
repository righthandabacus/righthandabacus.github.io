---
layout: post
title: "Installing IEEEtrans style file on Debian"
date: 2004-12-29 10:48:29 +0800
tags: code
---

Debian can get LaTeX via apt-get. However, IEEE transactions' style file cannot be obtained in this way. If you want to get the style file (normally named IEEEtran.cls) working in Debian, use the following steps. It is unbelievably easy.
1. Make sure you have `tetex-bin` and others installed on Debian
2. Obtain the package `IEEEtran.tar.gz` from IEEE
3. Extract it
4. `mkdir /usr/share/texmf/tex/latex/ieee`
5. `cp IEEEtran.cls /usr/share/texmf/tex/latex/ieee`
6. `texhash`
Done.

For the BibTeX part, things are similar:
1. Obtain the package `IEEEtranBST.tar.gz` from IEEE
2. Extract it
3. `mkdir /usr/share/texmf/bibtex/bib/ieee`
4. `mkdir /usr/share/texmf/bibtex/bst/ieee`
5. `cp IEEEtran.bst IEEEtranS.bst /usr/share/texmf/bibtex/bst/ieee/`
6. `cp IEEEabrv.bib IEEEbcpat.bib IEEEexample.bib IEEEfull.bib /usr/share/texmf/bibtex/bib/ieee/`
7. `texhash `

---
layout: post
title: "Use Comic Sans Font in LaTeX"
date: 2005-04-09 02:59:20 +0800
tags: system
---
I've prepared a presentation for my supervisor using LaTeX. The presentation is
to mimic a version created using MS Powerpoint and in LaTeX, I used seminar
package for formatting.

The tricky part is, by default, LaTeX uses Knuth's CM fonts but I want to have
MS Comic Sans for the content, so to mimic powerpoint to a high degree.
Searching "comic sans latex" in google, you will find the link
<http://www.ctan.org/tex-archive/macros/latex/contrib/comicsans/> or
<http://www.tug.org/tex-archive/macros/latex/contrib/comicsans/> (both are the
same).

However, I cannot follow the instruction to install the CTAN package successfully. Finally, I do on my own way:

  - Find some way to get the MS Comic Sans TTF file: `comic.ttf` and `comicbd.ttf`
  - Get your `ttf2pt1` ready, with the following commands:
```
$ ttf2pt1 -e -a comic.ttf
$ ttf2pt1 -e -a comicbd.ttf
```
  - Now you've created four files: `comic.pfa` `comic.afm` `comicbd.pfa` `comicbd.afm`
  - Afterwards, copy *just* these four files to somewhere, I place them in `/usr/local/share/texmf/fonts/ttf/`
  - Next, download the comicsans package from the two links above. You can either use wget to recursively download, or get a zip/tar.gz file of them.
  - For the downloaded package, extract it and look for a directory called `texmf`. Copy the whole directory to somewhere like `/usr/local/share/texmf/*`
  - Overwrite `/usr/local/share/texmf/dvips/comicsans/comicsans.map` to following:
```
rcomic8r ComicSansMS "TeXBase1Encoding ReEncodeFont" <8r.enc </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomicbd8r ComicSansMS-Bold "TeXBase1Encoding ReEncodeFont" <8r.enc </usr/local/share/texmf/fonts/ttf/comicbd.pfa
rcomiccyr ComicSansMS "T2AAdobeEncoding ReEncodeFont" <t2a.enc </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomiccyrbd ComicSansMS-Bold "T2AAdobeEncoding ReEncodeFont" <t2a.enc </usr/local/share/texmf/fonts/ttf/comicbd.pfa
rcomic7m ComicSansMS "TeXMathItalicEncoding ReEncodeFont" <texmital.enc </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomicbd7m ComicSansMS-Bold "TeXMathItalicEncoding ReEncodeFont" <texmital.enc </usr/local/share/texmf/fonts/ttf/comicbd.pfa
rcomic7y ComicSansMS "TeXMathSymbolEncoding ReEncodeFont" <texmsym.enc </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomic9z ComicSansMS "ComicSansExtraEncoding ReEncodeFont" <csextras.enc </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomico8r ComicSansMS "0.167 SlantFont" </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomicbdo8r ComicSansMS "0.167 SlantFont" </usr/local/share/texmf/fonts/ttf/comicbd.pfa
rcomiccyro ComicSansMS "0.167 SlantFont" </usr/local/share/texmf/fonts/ttf/comic.pfa
rcomiccyrbdo ComicSansMS "0.167 SlantFont" </usr/local/share/texmf/fonts/ttf/comicbd.pfa
```
  - Then, do the following commands: (as root in Debian)
```
# cat >> /etc/texmf/updmap.d/00updmap.cfg
Map comicsans.map
(Ctrl-D)
# texhash
# update-updmap
# updmap
```
  - Now, finished. You can use the comicsans package in your LaTeX document as the manual says.

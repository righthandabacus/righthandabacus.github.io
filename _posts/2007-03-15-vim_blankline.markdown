---
layout: post
title: "Blank line handling in VIM"
date: 2007-03-15 16:00:19 +0800
tags: system
---

* Remove all real blank lines: `:g/^$/d` or `:v/./d`
* Remove all lines with only whitespace: `:g/^\s*$/d` (delete lines with regex match)
* Replacing multiple blank lines with one: `:v/./.,/./-1j`
* Replace multiple blank lines with one and delete single blank lines: `:g/\n\n/j` and then `:v/./.,/./-1j`
* Double space the file: `:g/^/pu =\"\n\"` or `:g/^/pu _`

Other tricks:

* Reverse line order: `:g/^/m0`
* Reverse two character: `xp`
* Reverse two lines: `ddp`

Notes:

- `:[range]:g` search for something and execute `ex` commands
- `:v` negate the search pattern
- Basic movements:
  - `m,ncot` means to copy lines m through n after line t
  - `m,nt$` means to copy lines m through n to end of file
  - `-t$` means to copy previous line to end of file
  - `m0` means to move current line to the top
  - `.,+sm$-t` means to move current line through (current+s) line to t-th last line of file


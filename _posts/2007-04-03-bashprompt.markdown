---
layout: post
title: "Bash Prompt stuff"
date: 2007-04-03 09:35:06 +0800
tags: system
---

The following is what Gentoo sets its bash prompt:

```
if [ `/usr/bin/whoami` = 'root' ]
then
        if [ "$SHELL" = '/bin/bash' ] || [ "$SHELL" = '/bin/sh' ]
        then
                export PS1='\[\033[01;31m\]\h \[\033[01;34m\]\W \$ \[\033[00m\]'
        fi
else
        if [ "$SHELL" = '/bin/bash' ] || [ "$SHELL" = '/bin/sh' ]
        then
                export PS1='\[\033[01;32m\]\u@\h \[\033[01;34m\]\W \$ \[\033[00m\]'
        fi
fi
```

One can read the manpage of bash to see what the `\u`, `\h` and `\W` means. Most
of the things on the setting of `PS1` is to turn on the color, for example, when
you are `root`, the hostname shall be in red but a normal user should see it
green. The number `01;32` is what means green and it is the ANSI code for
controlling that. (I miss the time when everyone is using 16-color EGA
display!).

We can even control the title of xterm using similar tricks, see the code
snippet I put below:

```
export PS1='\[\e]2;\u@\h \W\a\e[32m\]\u@\h \[\e[34m\]\W \$ \[\e[0m\]'

##################################################
# ANSI codes
##################################################
# Begin of control characters   \[
# End of control characters     \]
# XTerm titlebar text           \e]2;<text>\a
# Reset color to the default    \e[0m
# Foreground + Background       \e[<fg>;<bg>m
# Foreground w/o background     \e[<fg>m
#       Background: Black       40
#       Background: Red         41
#       Background: Green       42
#       Background: Brown       43
#       Background: Blue        44
#       Background: Purple      45
#       Background: Cyan        46
#       Background: Grey        47
#       Foreground: Black       0;30    Grey    1;30
#       Foreground: Lt Red      0;31    Red     1;31
#       Foreground: Lt Green    0;32    Green   1;32
#       Foreground: Lt Brown    0;33    Yellow  1;33
#       Foreground: Lt Blue     0;34    Blue    1;34
#       Foreground: Lt Purple   0;35    Purple  1;35
#       Foreground: Lt Cyan     0;36    Cyan    1;36
#       Foreground: Lt Grey     0;37    White   1;37
```

One thing to note: All the ANSI code for controlling colors (i.e. non-printing
stuff) *must be* escaped by a pair of `\[` and `\]`, otherwise your bash
prompt will be broken when wrapping a very long command line.

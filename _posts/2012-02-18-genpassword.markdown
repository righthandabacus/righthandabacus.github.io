---
layout: post
title: "Generating Random Password"
date: 2012-02-18 00:50:32 -0500
tags: system
---
Source: <http://www.linuxstall.com/command-line-password-generator/>

The random source in many UNIX system is `/dev/random` and `/dev/urandom`, but that's a binary random source. To convert it into something usable as password, we can use `tr`:

    tr -dc '!-z' < /dev/urandom | fold -w10 | head -1

The `fold` command folds the character stream into fixed line width. Similar use
would be `head -w10` but it would not give you the new line at the end. Maybe
use `[:alnum:]` is a better choice if password shall not contain any
punctuation.

Other methods include:

  * Base64 encoding of something:\\
    `dd if=/dev/urandom bs=1 count=32 2>/dev/null | base64 -w 0 | rev | cut -b 2- | rev`
  * Base64 of the md5sum/sha256sum of something:\\
    `date +%s | sha256sum | base64 | head -c 32 ; echo`
  * Using OpenSSL's random generator:\\
    `openssl rand -base64 32`


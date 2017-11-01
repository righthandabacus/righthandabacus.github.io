---
layout: post
title: "Thunderbird on Debian"
date: 2006-06-08 14:05:51 +0800
tags: system
---
Recently, I switched from Mozilla suite to Thunderbird and Firefox (reason: be realistic, Moz Team really abandoned the suite).

In Debian system, it is quite easy for you to do the transition, except you have to do some manual moving of files, but that's relatively easy. However, as Mozilla suite is one application and Thunderbird and Firefox are separate, they are not by-default interoperable.

To make Firefox open mailto: links in Thunderbird, it is very easy, put the following lines into your `prefs.js` which located in `~/.mozilla/firefox/somehash.default`:

    user_pref("network.protocol-handler.app.mailto", "/usr/bin/thunderbird");
    user_pref("network.protocol-handler.app.nntp", "/usr/bin/thunderbird");

However, to make Thunderbird recognize Firefox (or any default browser in Debian) and use it to open the web links, you have to set the following in the `prefs.js` in your home dir's `.mozilla-thunderbird/somehash.default`:

    user_pref("network.protocol-handler.expose-all", false);
    user_pref("network.protocol-handler.app.http","x-www-browser");
    user_pref("network.protocol-handler.app.https","x-www-browser");

The key point is the first line. You *must* set `expose-all` to `false` or otherwise, you can load the link but only a copy in your `/tmp`, not on the web directly. (Try out to get what I mean)

I used quite a number of hours to solve this problem.






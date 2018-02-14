---
layout: post
title: A game using Python speech recognition
date: Wed, 14 Feb 2018 09:46:18 -0500
tags: code
---

While trying with the Google AIY voice kit, I developed a math drill game. A
nice try, but the result is not as smooth as what I expected.

Source code is in repository: <https://github.com/righthandabacus/pydrill>

# Math drill game using speech recognition

This is a Python app using Tkinter and SpeechRecognition modules. It can run on
Raspberry Pi (if you specify the correct microphone device) or other machine
with a microphone.

![](https://github.com/righthandabacus/pydrill/raw/master/screenshot.png)

The drill game shows a single digit multiplication and wait for uttered answer
(within 10 seconds).  Then the answer will be checked and the number of
correct/wrong answer will be tallied. The voice recognition is using the
`speech_recognition` module, which in turn, passed on to Google speech
recognition service.

## User experience

The performance is not as smooth as expected to be a "game". Two reasons for
that: It doesn't seem to be a way to capture audio and do recognition
simultaneously. Also, Google voice recognition is not instantaneous but with
some processing and network delay. These two added up, the user will see
noticible delay between calling out the answer and the answer being checked.

## Quality of voice recognition

The speech recognition module provides multiple ways to recognize. I tried out
the PocketSphinx and Google service. There are noticible difference between the
two: Firstly, PocketSphinx has way lower accuracy than Google's voice
recognition service, even after providing a dictionary with weight and so forth.
Secondly, the result from PocketSphinx is in spelled English words for numbers,
e.g., "forty two" while Google gives you numerials, "42". Even PocketSphinx is
offline, it doesn't seem to be any faster than Google.

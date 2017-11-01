---
layout: post
title: "Python generators"
date: 2011-12-19 15:42:02 -0500
tags: code
---

Summary from reading: "Generator Tricks for Systems Programmers" and "A Curious
Course on Coroutines and Concurrency" by [David
Beazley](http://www.dabeaz.com/talks.html)

## Syntax

In Python, we have `[list]`, `{dictionary}`, and `(generator)`

## Iterators vs Generators

Iterator is an object with `ˍˍiterˍˍ()` and `next()` defined, and raise
`StopIteration` upon `next()` when there is no more things to iterate over, e.g.

```python
class countdown(object):
	def __init__(self,start):
		self.count = start
	def __iter__(self):
		return self
	def next(self):
		if self.count <= 0:
			raise StopIteration
		r = self.count
		self.count -= 1
		return r
c = countdown(5)
for i in c:
	print i
```

Generator is a function with `yield`, e.g.

```python
def countdown(n)
	while n > 0:
		yield n
		n -= 1
for i in countdown(5):
	print i
```

When a generator returns, it raises `StopIteration` implicitly. Similar to
iterators, we can call a generator with `next()`, e.g.

```python
c = countdown(5)
print c.next()
```

## Generators as a pipeline
An example to use a generator to analyse a web log:

An apache log file is lines of space-delimited fields, with the last field is
either the byte count or a "-" if the count if not available. The imperative
programming model suggests this solution:

```python
wwwlog = open("access-log")
total = 0
for line in wwwlog:
	bytestr = line.rsplit(None,1)[1]
	if bytestr != '-':
		total += int(bytestr)
print "Total", total
```

The generator expression solution is the following:

```python
wwwlog     = open("access-log")
bytecolumn = (line.rsplit(None,1)[1] for line in wwwlog)
bytes      = (int(x) for x in bytecolumn if x != '-')
print "Total", sum(bytes)
```

To extend this program, we can scan over a directory of log files, possibly
compressed, and print the sum of bytes of all files that patches a pattern:

```python
import os
import fnmatch
def gen_find(filepat,top):
	for path, dirlist, filelist in os.walk(top):
		# path,dirlist,filelist = Current dir, list of subdir, list of files
		for name in fnmatch.filter(filelist,filepat):
			yield os.path.join(path,name)  
import gzip, bz2
def gen_open(filenames):
	for name in filenames:
		if name.endswith(".gz"):
			yield gzip.open(name)
		elif name.endswith(".bz2"):
			yield bz2.BZ2File(name)
		else:
			yield open(name)
def gen_cat(sources):
	for s in sources:
		for item in s:
			yield item
import re
def gen_grep(pat, lines):
	patc = re.compile(pat)
	for line in lines:
		if patc.search(line): yield line

pat        = r"somepattern"
logdir     = "/some/dir/"

filenames  = gen_find("access-log*",logdir)
logfiles   = gen_open(filenames)
loglines   = gen_cat(logfiles)
patlines   = gen_grep(pat,loglines)
bytecolumn = (line.rsplit(None,1)[1] for line in patlines)
bytes      = (int(x) for x in bytecolumn if x != '-')

print "Total", sum(bytes)
```

## Killing Generators
A generator object can be forced to close before `StopIteration`, by calling
`object.close()`. In the meantime, that generator object receives a
`GeneratorExit` event, so that clean up can be done internally, e.g.

```python
import time
def follow(thefile):
	thefile.seek(0,2)	# go to the end of the file
	try:
		while True:
			line = thefile.readline()
			if not line:
				time.sleep(0.1)	# sleep briefly
				continue
			yield line
	except GeneratorExit:
		print "Shutting down"
```

But calling `close()` is supported only for generators in the same thread,
otherwise a user-provided semaphore is needed.

## Coroutines

New in Python 2.5, we have coroutines, which is same as generators, that uses `yield` command.

Instead of output by `yield`, coroutines are taking input from `yield`, example as follows:

```python
def grep(pattern):
	print "Looking for %s" % pattern
	while True:
		line = (yield)
		if pattern in line:
			print line,
g = grep("python")
g.next()
g.send("python generators rock!")
```

The coroutine is a function that takes `yield` as an expression. The value that
`yield` gives is the argument from the `send()` call from outside. Before one
can do `send()`, we need to call the coroutine with `next()` so as to *prime*
it, i.e. proceed the coroutine function to the position of `yield` expression.

Simply speaking, using generators is a "pull" of data but using coroutines is a
"push" of data.

Similar to generators, we can kill a coroutine with `close()` call and the
coroutine will receive a `GeneratorExit` event.

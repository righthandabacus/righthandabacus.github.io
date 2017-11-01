#!/usr/bin/python
# dedup.py - Sat, 23 Apr 2011 00:03:30 -0400
# As command line arguments are files of md5sum's. The md5sums are read one
# by one in their order and all duplicated files are removed, only the
# files of their first appearance are kept in their old path.

import sys;
import os.path;

f = {}	# File hashes

for file in sys.argv:
	md5file = open(file)
	for line in md5file.read().splitlines():
		if f.has_key(line[0:32]):
			print "Remove "+line[34:]
			os.remove(line[34:])
		else:
			print "Keep "+line[34:]
			f[line[0:32]] = line[34:]
	md5file.close()

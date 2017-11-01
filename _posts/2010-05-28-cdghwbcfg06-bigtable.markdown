---
layout: post
title: "Chang et al (2006) Bigtable: A Distributed Storage System for Structured Data (OSDI)"
date: 2010-05-28 19:38:39 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Bigtable: A Distributed Storage System for Structured Data"
  author: "Fay Chang and Jeffrey Dean and Sanjay Ghemawat and Wilson C. Hsieh and Deborah A. Wallach Mike Burrows and Tushar Chandra and Andrew Fikes and Robert E. Gruber"
  booktitle: "Proc OSDI'06"
  year: 2006
---
Bigtable is a key-value mapping storage. The major key is called the row key and there can also be some minor keys called the column key. Both key and value are in arbitrary size.

The Bigtable architecture is similar to Google File System. It has a master server to hold the metadata and a set of tablet server to hold tablets, components to build up a table. The client, whenever requests for a tablet, looks for the tablet server from the master server. Once the location is known, the client contacts the tablet server directly within the cache lifetime. The master server knows all the tablets but does not store the tablet-server association. It requests for the list of tablets from the server upon start up to build its directory.

---
layout: post
title: "NFS module"
date: 2007-08-10 08:06:57 +0800
tags: system
---

This is the NFS daemon module for Thecus N1200. I made it based on the version
1.1.00 of the N2100 NFSD module by Lzimmerma Mshapf, with the binary executables
replaced with the PowerPC counterpart from Debian woody, as well as with the
`install.rdf` file modified to fit the N1200 architecture.

There are two tarballs available:

  * [`n1200-nfsd-1.0.00.tar.gz`](/img/n1200-nfsd-1.0.00.tar.gz)\\
    designed for firmware 1.0.07.2, which I am using.
  * [`n1200-nfsd-1.0.01.tar.gz`](/img/n1200-nfsd-1.0.01.tar.gz)\\
    modified the `install.rdf` so that version 1.0.06 firmware can also
    install. Nothing else is changed.

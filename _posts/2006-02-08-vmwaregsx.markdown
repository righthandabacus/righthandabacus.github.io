---
layout: post
title: "Fix for Linux 2.6.15+VMware GSX 3"
date: 2006-02-08 12:47:15 +0800
tags: system
---
The point is, the VMware GSX server cannot compile correct kernel modules. The solution is as follows:

  - Prepare gcc 4.0 (verified, 3.3 don't work)
  - Download the kernel module update from <http://ftp.cvut.cz/vmware/vmware-any-any-update96.tar.gz>
  - Compile Linux kernel using gcc 4.0, and use it (requires reboot)
  - Find a convenient place and execute `tar zxf vmware-any-any-update96.tar.gz`
  - Change to the newly created directory `vmware-any-any-update96`
  - Run as root (i.e. sudo): `./runme.pl`
  - Call `vmware-config.pl` and do the rest as usual






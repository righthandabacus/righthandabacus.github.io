---
layout: post
title: "Jeong et al (2014) mTCP - A highly scalable user-level TCP stack for multicore systems"
date: 2019-06-15
category: paper
bibtex:
  type: inproceedings
  author: EunYoung Jeong and Shinae Wood and Muhammad Jamshed and Haewon Jeong and Sunghwan Ihm and Dongsu Han and KyoungSoo Park
  title: "mTCP: a Highly Scalable User-level {TCP} Stack for Multicore Systems"
  booktitle: "11th {USENIX} Symposium on Networked Systems Design and Implementation ({NSDI} 14)"
  year: 2014
  isbn: 978-1-931971-09-6
  address: "Seattle, WA"
  pages: "489--502"
  url: "https://www.usenix.org/conference/nsdi14/technical-sessions/presentation/jeong"
  pdf: "https://www.usenix.org/system/files/conference/nsdi14/nsdi14-paper-jeong.pdf"
  publisher: "{USENIX} Association"
---

A paper addresses the inefficiency of TCP in Linux kernel.

The authors investigated the Linux stack and a TCP user at user-mode. The
authors argue that the OS have to process TCP connections at high speed due to
modern usage patterns. Linux TCP performance peak at 0.3 million transactions
per second while we can have tens of millions packets I/O per second. The
difference is due to system overhead.

There are 4 major inefficiencies in Linux TCP stack:

1. lack of connection locality. A multithread application share a listen socket
across threads. Therefore threads will contend for a lock to access the socket.
Also, cache miss and cacheline sharing across CPU cores also hurts performance.
This can be improved by

2. shared file descriptor space. Sockets are file descriptors that goes through
the Linux virtual file system. When there are a large number of concurrent
connections, the search and locks on the VFS is a significant overhead

3. inefficient packet processing. Packets I/O are done per packet but it is
more efficient (less number of interrupts) if we can batch process packets of
the same TCP stream.

4. heavy system call overhead. BSD socket = switching between user-mode and
kernel-mode frequently, esp. when there are many short-lived connections

The proposal, mTCP, is to get rid of the TCP stack in kernel but use only its
packet I/O. The TCP layer is then implemented in user-mode as a library.
Experiment by the authors show that >80% CPU time is spent in kernel during
large number (8K to 48K) of concurrent TCP connections. Mostly due to lock
contention for shared in-kernel data structures, buffer management, and frequently
mode switch.

The mTCP library is a user-level TCP stack with packet I/O library. It provides
socket API and runs as a thread on each CPU core within the same application
process. Packets are sent/receive from the NIC using packet I/O library built
from PacketShader I/O (PSIO) engine [SIGCOMM 2010 paper] to avoid I/O by
polling, which is too CPU expensive. PSIO performs packet I/O in batches so we
can amortize the cost of PCI (e.g. DMA) operations. The TCP stack is implemented
as a thread. Application uses mTCP library functions to communicate with the
TCP thread via shared buffers (e.g., job queues). This way, requests from
multiple flows are processed in batch when the TCP thread regains the CPU.
mTCP uses lock-free data structures as there is always only one producer and one
consumer access to a queue. It also prioritize TCP control packets, and maintain
memory pool for connection control blocks to avoid on-demand memory allocation.

On the API side, it provide another set of functions similar to the BSD socket
interface. But because the socket descriptor space is available only to the
mTCP thread in a process, it avoids the system-wide lock contention. However,
as mTCP bypassed the kernel, it also bypassed the firewall and packet scheduler.

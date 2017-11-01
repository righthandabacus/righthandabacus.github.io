---
layout: post
title: "bash and TCP/UDP sockets"
date: 2012-02-19 21:06:14 -0500
tags: system
---

bash can do TCP and UDP sockets. They are represented as a special file in `/dev`, like

    /dev/tcp/192.168.0.1/1234
    /dev/udp/192.168.0.2/2345

even though those `/dev` files do not exist. However, bash only do active
connection but not listen. That is, we can use this to replace telnet. This is
an example:

On machine A, run netcat (say) and listen to a port:
 
    machine_a$ nc -l -p 1234

On machine B, for convenience, build the socket as a file handle, then cat the
file to stdout and at the same time, cat the stdin to the file:

    machine_b$ exec 9<>/dev/tcp/machine_a/1234
    machine_b$ cat <&9 &
    [1] 6543
    machine_b$ cat >&9

Then `machine_a` and `machine_b` are connected.

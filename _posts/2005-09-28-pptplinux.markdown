---
layout: post
title: "Connecting PPTP VPN in Linux"
date: 2005-09-28 23:22:55 +0800
tags: system
---

I am in CUHK. Hence I need to connect to VPN from time to time. The CUHK VPN
pool is using PPTP implemented by the Cisco routers or access servers (although
another using IPSec is also available, but I am not supposed to know about
its existence...) and I am using Debian as my primary operating system. This
guide is to record down how do I make a PPTP VPN in Debian Linux.

## Requirement

The required software is the `pppd` daemon and the `pptp-linux` suite, also your
kernel needs to support the point-to-point protocol in asynchronous mode. Here
are what I did:

    # apt-get install pppd pptp-linux

That's enough.

## Procedure

To connect via PPTP, you need to make sure your kernel support PPP because PPTP
is just using PPP over your existing network connection instead of your modem. I
am using the kernel 2.6.8.1 at the mean time and I do this to load the modules
for my kernel's support:

    # modprobe ppp-generic
    # modprobe ppp-async

Basically the last line will help you load the first line as well. You may need
to check your kernel's support and build options to see if you need to recompile
your kernel or use another name to call these modules or the modules are already
compiled-in.

Once you make sure your kernel supports "ppp-async", you can configure your PPP
scripts. The script is located in `/etc/ppp/peers`. You can just create a script
with any name, for example, mine is `/etc/ppp/peers/vpn`:

    user s034554
    require-pap
    noipdefault
    usepeerdns
    defaultroute
    persist
    noauth
    noaccomp
    default-asyncmap
    nopcomp
    noccp
    novj

The above is to specify that, I am using `s034554` as my login name and PAP as
the authentication method. Once connected, use the peer side's DNS and use the
PPTP connection as my default route. Furthermore, make this PPTP connection
persistent, which mean to reconnect upon disconnected. The rest options are
mainly configuring the PPTP not to do any encryption or compression. You can get
reference to these from the documentation of PPTP-Linux or PPP daemon.

After you've made this file, you can call like this:

    # pptp vpn.cuhk.edu.hk -- call vpn

The part after pptp is the hostname you are connecting to, and then a double
dash to separate the PPTP option and PPP options, which I tell PPP to use the
options described in the script called `vpn` in the `/etc/ppp/peers` directory.

Once you invoked this command, you will get a `ppp0` connection within a few seconds.

## Another Procedure

The above is not the only way to do. We can alternatively, invoke `pppd` instead
of pptp to bring up the connection. It is no difference, just see if you want
`pppd` to call pptp or pptp to call ppp to finish the whole thing. If you choose
to use the former method, see the following.

Your kernel also needs the asynchronous ppp support. Afterwards, create a option
script in `/etc/ppp/peers` like this:

    name s034554
    require-pap
    defaultroute
    persist
    pty "pptp vpn.cuhk.edu.hk --nolaunchpppd"
    remotename vpn.cuhk.edu.hk
    ipparam vpn.cuhk.edu.hk
    lock
    noauth
    nobsdcomp
    nodeflate

The key point here is you have already specify your VPN server (in PPP's
terminology, the concentrator) in the option file and then we can just connect
via the PPTP by invoking:

     # pppd call vpn

where I have assumed that `vpn` is the name of the script.

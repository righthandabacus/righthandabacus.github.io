---
layout: post
title: "NetInstall Debian using USB devices"
date: 2007-10-19 13:03:55 -0400
tags: system
---

Yesterday, I got a computer from my school, it is:

|Brand     |Dell Optiplex 740|
|Processor |AMD Athlon 64|
|Memory    |2 x 1GB DDR2 Kingston|
|Hard disk |250GB SATA|
|Periperals|USB x 7, Serial, Parallel|

The whole point is, firstly, it is an AMD64 machine; and secondly, it has no
optical drive. But anyway, what I need is just a Debian to replce the Windoze
system.

To install Debian, I used a USB thumb disk. I tried with a card reader and a
Sony memory stick but it doesn't work because of the card reader seems do not
support the bootstrap process. The way to use it, after troubleshooting for a
long time, is: (assuming your thumb disk is `/dev/sda`)

    # wget -nH -nd http://some.mirror/path/installer-amd64/current/images/hd-media/boot.img.gz
    # zcat boot.img.gz > /dev/sda
    # mount -t vfat /dev/sda /mnt
    # cp debian-40r1-amd64-netinst.iso /mnt
    # umount /mnt

The whole point is, you have to `zcat` to `/dev/sda` rather than `sda1`, because
otherwise you mean to rely on the crapy boot sector on your thumb disk. If you
have to do the other way, you may also use the `mbr` package in Debian to set a
bootable boot sector.

Once you have the thumb disk ready, plug it into the Dell box and boot. Remember
to set the boot device to be *USB ZIP Disk*.

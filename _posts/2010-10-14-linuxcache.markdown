---
layout: post
title: "Flushing cache in Linux to free up memory"
date: 2010-10-14 10:56:28 -0400
tags: system
---
To free page cache:

    echo 1 > /proc/sys/vm/drop_caches

To free d-entries and i-nodes:

    echo 2 > /proc/sys/vm/drop_caches

To free page cache, d-entries and i-nodes:

    echo 3 > /proc/sys/vm/drop_caches

Alternatively, we can do it using sysctl:

    sysctl -w vm.drop_caches=1

or we can ask the kernel to reclaim the memory used for caching more
aggressively:

    sysctl -w vm.vfs_cache_pressure=n  (n > 100)

But in case the cache is used up because of a program reading a huge file, we
can sacrifice some performance by asking the kernel to access the file
*directly* rather than cache it in the memory, by invoking `fcntl` with the file
descriptor `fd`:

    fcntl(fd, F_SETFL, O_DIRECT)

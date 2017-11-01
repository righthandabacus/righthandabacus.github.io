---
layout: post
title: "Snapshot backup"
date: 2012-02-20 01:43:12 -0500
lastedit: 2017-07-30
tags: system
category: blog
---

This is a method of backup that I invented many years ago and used at
workplace. I think it is quite useful because it abused the nature of hard
links in UNIX.

Consider the case that you have a repository that roughly do not grow in size
but changes slightly day by day, say, `/data`, and you want to make a
daily snapshot of it. The trivial way to do is to make a copy, e.g. using GNU's
`cp`:

    cp -a /data /backup/day01/

This is fine. Now you used (at least) a double of the storage space. Ten days'
snapshot will be ten times more storage space at least.  "At least" because if
you have hard links between different files in `/data`, `cp` will make multiple
copies instead of preserving hard links in the backup. To make the
backup preserve the hard link relationships, GNU's `cp` cannot help but we have
to use `rsync` with the `-H` option

    rsync -aH /data /backup/day01/

This the the first snapshot, e.g., on the first day. If we make another backup
on the second day, we can save the storage needed by making *hard link against first copy*:

    cp -al /backup/day01 /backup/day02
    rsync -aH --delete-after /data /backup/day02/

The above is, firstly, build `day02` snapshot from `day01` snapshot by
hard-linking all files from `day01` to `day02` (directories are not hard linked
due to OS restriction, but OK).  Then we use `rsync` to make `day02` snapshot
up to date. Since `rsync` updates a file by deleting the old one and creating a
new one unless we specified `--inplace` option, the files in `day01` is intact.
In this case, we can keep many days of snapshot without consuming much space.

A script to do the backup automatically is as follows:

```bash
#!/bin/bash

repository=/data
backup=/backup

# Get the name of the directory for the new snapshot and the latest snapshot
now=`date -u --rfc-3339=sec`
pushd $backup
latest=`ls -1 | sort | tail -1`

# Make a copy if we have a latest snapshot
if [ "latest" ne "" ]; then
  cp -al $latest $now
fi

# Build the current snapshot by rsync
rsync -aH --delete-after $repository/ $now/

# Optional: Remove the earliest snapshot, probably to be enclosed by some condition
earliest=`ls -1 | sort | head -1`
rm -rf $earliest
```

Using this method assumes that the snapshot backup is read-only, or otherwise
it is not a snapshot. This can be enforced by making the snapshot in a machine
not directly accessible by the end-users, but they can access it over, say, NFS
or Samba. Moreover, it may not work perfectly across different file system
because of some exotic file attributes.

Update (2017-07-30): I come across [a
post](https://lincolnloop.com/blog/detecting-file-moves-renames-rsync/) that
uses the similar trick, not for backup, but for synchronizing directories
across network with directory structure changes. Its procedure is as follows:

1. Assume directory containing everything is `/path/to/dir`, synchronized
   between local and remote hosts
2. Use `cp -al` to make a copy of `/path/to/dir` to `/path/to/workdir`
3. Modify `path/to/workdir`, including restructure dirs, renaming and
   relocating files, and modifying files
4. When done working on the dir, use the following `rsync` to synchronize:

       rsync -avHP --delete-after --no-inc-recursive /path/to/dir /path/to/workdir remotebox:parentdir/

Note the two dirs in the command above does not have trailing slash as we want to copy the
dirs, not only files beneath it. The option `--no-inc-recursive` is only
available in `rsync` version 3.0 above to retain older version's behaviour of
scanning directories recursively *at the beginning* of command run. This
command will make `rsync` to learn about the hard link in the source and retain
the hard link relationship in the destination. After this is completed, we can
delete the original path `/path/to/dir` and keep only `/path/to/workdir`.

---
layout: post
title: "Linux Kernel Calls"
date: 2006-03-14 02:17:18 +0800
tags: code
---

## Where to find?
To get a list of available Linux kernel calls, go to the src dir of your kernel
and find `include/asm/unistd.h`, that's the *UNIX Standard*, i.e. the kernel
call vs call number mapping. Alternative location is
`/usr/include/linux/unistd.h`.

For example, this is from kernel version 2.6.15

```c
#define __NR_restart_syscall      0
#define __NR_exit                 1
#define __NR_fork                 2
#define __NR_read                 3
#define __NR_write                4
#define __NR_open                 5
#define __NR_close                6
#define __NR_waitpid              7
#define __NR_creat                8
#define __NR_link                 9
#define __NR_unlink              10
#define __NR_execve              11
#define __NR_chdir               12
#define __NR_time                13
#define __NR_mknod               14
#define __NR_chmod               15
#define __NR_lchown              16
#define __NR_break               17
#define __NR_oldstat             18
#define __NR_lseek               19
#define __NR_getpid              20
#define __NR_mount               21
#define __NR_umount              22
#define __NR_setuid              23
#define __NR_getuid              24
#define __NR_stime               25
#define __NR_ptrace              26
#define __NR_alarm               27
#define __NR_oldfstat            28
#define __NR_pause               29
#define __NR_utime               30
#define __NR_stty                31
#define __NR_gtty                32
#define __NR_access              33
#define __NR_nice                34
#define __NR_ftime               35
#define __NR_sync                36
#define __NR_kill                37
#define __NR_rename              38
#define __NR_mkdir               39
#define __NR_rmdir               40
#define __NR_dup                 41
#define __NR_pipe                42
#define __NR_times               43
#define __NR_prof                44
#define __NR_brk                 45
#define __NR_setgid              46
#define __NR_getgid              47
#define __NR_signal              48
#define __NR_geteuid             49
#define __NR_getegid             50
#define __NR_acct                51
#define __NR_umount2             52
#define __NR_lock                53
#define __NR_ioctl               54
#define __NR_fcntl               55
#define __NR_mpx                 56
#define __NR_setpgid             57
#define __NR_ulimit              58
#define __NR_oldolduname         59
#define __NR_umask               60
#define __NR_chroot              61
#define __NR_ustat               62
#define __NR_dup2                63
#define __NR_getppid             64
#define __NR_getpgrp             65
#define __NR_setsid              66
#define __NR_sigaction           67
#define __NR_sgetmask            68
#define __NR_ssetmask            69
#define __NR_setreuid            70
#define __NR_setregid            71
#define __NR_sigsuspend          72
#define __NR_sigpending          73
#define __NR_sethostname         74
#define __NR_setrlimit           75
#define __NR_getrlimit           76     /* Back compatible 2Gig limited rlimit */
#define __NR_getrusage           77
#define __NR_gettimeofday        78
#define __NR_settimeofday        79
#define __NR_getgroups           80
#define __NR_setgroups           81
#define __NR_select              82
#define __NR_symlink             83
#define __NR_oldlstat            84
#define __NR_readlink            85
#define __NR_uselib              86
#define __NR_swapon              87
#define __NR_reboot              88
#define __NR_readdir             89
#define __NR_mmap                90
#define __NR_munmap              91
#define __NR_truncate            92
#define __NR_ftruncate           93
#define __NR_fchmod              94
#define __NR_fchown              95
#define __NR_getpriority         96
#define __NR_setpriority         97
#define __NR_profil              98
#define __NR_statfs              99
#define __NR_fstatfs            100
#define __NR_ioperm             101
#define __NR_socketcall         102
#define __NR_syslog             103
#define __NR_setitimer          104
#define __NR_getitimer          105
#define __NR_stat               106
#define __NR_lstat              107
#define __NR_fstat              108
#define __NR_olduname           109
#define __NR_iopl               110
#define __NR_vhangup            111
#define __NR_idle               112
#define __NR_vm86old            113
#define __NR_wait4              114
#define __NR_swapoff            115
#define __NR_sysinfo            116
#define __NR_ipc                117
#define __NR_fsync              118
#define __NR_sigreturn          119
#define __NR_clone              120
#define __NR_setdomainname      121
#define __NR_uname              122
#define __NR_modify_ldt         123
#define __NR_adjtimex           124
#define __NR_mprotect           125
#define __NR_sigprocmask        126
#define __NR_create_module      127
#define __NR_init_module        128
#define __NR_delete_module      129
#define __NR_get_kernel_syms    130
#define __NR_quotactl           131
#define __NR_getpgid            132
#define __NR_fchdir             133
#define __NR_bdflush            134
#define __NR_sysfs              135
#define __NR_personality        136
#define __NR_afs_syscall        137 /* Syscall for Andrew File System */
#define __NR_setfsuid           138
#define __NR_setfsgid           139
#define __NR__llseek            140
#define __NR_getdents           141
#define __NR__newselect         142
#define __NR_flock              143
#define __NR_msync              144
#define __NR_readv              145
#define __NR_writev             146
#define __NR_getsid             147
#define __NR_fdatasync          148
#define __NR__sysctl            149
#define __NR_mlock              150
#define __NR_munlock            151
#define __NR_mlockall           152
#define __NR_munlockall         153
#define __NR_sched_setparam             154
#define __NR_sched_getparam             155
#define __NR_sched_setscheduler         156
#define __NR_sched_getscheduler         157
#define __NR_sched_yield                158
#define __NR_sched_get_priority_max     159
#define __NR_sched_get_priority_min     160
#define __NR_sched_rr_get_interval      161
#define __NR_nanosleep          162
#define __NR_mremap             163
#define __NR_setresuid          164
#define __NR_getresuid          165
#define __NR_vm86               166
#define __NR_query_module       167
#define __NR_poll               168
#define __NR_nfsservctl         169
#define __NR_setresgid          170
#define __NR_getresgid          171
#define __NR_prctl              172
#define __NR_rt_sigreturn       173
#define __NR_rt_sigaction       174
#define __NR_rt_sigprocmask     175
#define __NR_rt_sigpending      176
#define __NR_rt_sigtimedwait    177
#define __NR_rt_sigqueueinfo    178
#define __NR_rt_sigsuspend      179
#define __NR_pread64            180
#define __NR_pwrite64           181
#define __NR_chown              182
#define __NR_getcwd             183
#define __NR_capget             184
#define __NR_capset             185
#define __NR_sigaltstack        186
#define __NR_sendfile           187
#define __NR_getpmsg            188     /* some people actually want streams */
#define __NR_putpmsg            189     /* some people actually want streams */
#define __NR_vfork              190
#define __NR_ugetrlimit         191     /* SuS compliant getrlimit */
#define __NR_mmap2              192
#define __NR_truncate64         193
#define __NR_ftruncate64        194
#define __NR_stat64             195
#define __NR_lstat64            196
#define __NR_fstat64            197
#define __NR_lchown32           198
#define __NR_getuid32           199
#define __NR_getgid32           200
#define __NR_geteuid32          201
#define __NR_getegid32          202
#define __NR_setreuid32         203
#define __NR_setregid32         204
#define __NR_getgroups32        205
#define __NR_setgroups32        206
#define __NR_fchown32           207
#define __NR_setresuid32        208
#define __NR_getresuid32        209
#define __NR_setresgid32        210
#define __NR_getresgid32        211
#define __NR_chown32            212
#define __NR_setuid32           213
#define __NR_setgid32           214
#define __NR_setfsuid32         215
#define __NR_setfsgid32         216
#define __NR_pivot_root         217
#define __NR_mincore            218
#define __NR_madvise            219
#define __NR_madvise1           219     /* delete when C lib stub is removed */
#define __NR_getdents64         220
#define __NR_fcntl64            221
/* 223 is unused */
#define __NR_gettid             224
#define __NR_readahead          225
#define __NR_setxattr           226
#define __NR_lsetxattr          227
#define __NR_fsetxattr          228
#define __NR_getxattr           229
#define __NR_lgetxattr          230
#define __NR_fgetxattr          231
#define __NR_listxattr          232
#define __NR_llistxattr         233
#define __NR_flistxattr         234
#define __NR_removexattr        235
#define __NR_lremovexattr       236
#define __NR_fremovexattr       237
#define __NR_tkill              238
#define __NR_sendfile64         239
#define __NR_futex              240
#define __NR_sched_setaffinity  241
#define __NR_sched_getaffinity  242
#define __NR_set_thread_area    243
#define __NR_get_thread_area    244
#define __NR_io_setup           245
#define __NR_io_destroy         246
#define __NR_io_getevents       247
#define __NR_io_submit          248
#define __NR_io_cancel          249
#define __NR_fadvise64          250
#define __NR_set_zone_reclaim   251
#define __NR_exit_group         252
#define __NR_lookup_dcookie     253
#define __NR_epoll_create       254
#define __NR_epoll_ctl          255
#define __NR_epoll_wait         256
#define __NR_remap_file_pages   257
#define __NR_set_tid_address    258
#define __NR_timer_create       259
#define __NR_timer_settime      (__NR_timer_create+1)
#define __NR_timer_gettime      (__NR_timer_create+2)
#define __NR_timer_getoverrun   (__NR_timer_create+3)
#define __NR_timer_delete       (__NR_timer_create+4)
#define __NR_clock_settime      (__NR_timer_create+5)
#define __NR_clock_gettime      (__NR_timer_create+6)
#define __NR_clock_getres       (__NR_timer_create+7)
#define __NR_clock_nanosleep    (__NR_timer_create+8)
#define __NR_statfs64           268
#define __NR_fstatfs64          269
#define __NR_tgkill             270
#define __NR_utimes             271
#define __NR_fadvise64_64       272
#define __NR_vserver            273
#define __NR_mbind              274
#define __NR_get_mempolicy      275
#define __NR_set_mempolicy      276
#define __NR_mq_open            277
#define __NR_mq_unlink          (__NR_mq_open+1)
#define __NR_mq_timedsend       (__NR_mq_open+2)
#define __NR_mq_timedreceive    (__NR_mq_open+3)
#define __NR_mq_notify          (__NR_mq_open+4)
#define __NR_mq_getsetattr      (__NR_mq_open+5)
#define __NR_sys_kexec_load     283
#define __NR_waitid             284
/* #define __NR_sys_setaltroot  285 */
#define __NR_add_key            286
#define __NR_request_key        287
#define __NR_keyctl             288
#define __NR_ioprio_set         289
#define __NR_ioprio_get         290
#define __NR_inotify_init       291
#define __NR_inotify_add_watch  292
#define __NR_inotify_rm_watch   293
#define NR_syscalls 294
```

Indeed, more detail of the kernel calls can be found in intro(2) manpage. The
method described in the intro(2) manpage is to use a macro to build a C-style
function prototype for you to use to call a syscall *directly*. Here the
directly means really raw, you will have the macro expanded into a bunch of
assembly commands with an `int 0x80`.

Another method is to *indirectly* call the syscall, via the syscall() function
(declared in `sys/syscall.h`). One can read the syscall(2) manpage for an
example. This is nicer. Please note that the syscall() function requires the
syscall number, which is the use of the mappings in the above code segment.

Yet another method for calling a syscall is to use the glibc wrapped calls. For
example, the following two segments are doing exactly the same thing:

```c
#include <syscall.h>        /* for syscall.h */
#include <unistd.h>         /* for not writing the numbers directly */
#include <stdio.h>          /* for standard things from glibc */

int main(void) {
    long pid1, pid2;
    pid1 = syscall(__NR_getpid);     /* using syscall() */
    pid2 = getpid();                 /* using glibc wrapped calls */

    printf("syscall pid = %ld\nglibc pid = %ld\n",pid1,pid2);
}
```

## Adding one
It is quite a lot of resources on Google that teaches you how to add a syscall to the Linux kernel. The following is just one of the possibilities.

First of all, get the kernel source tree, and in the `kernel/` subdir, create a file and implement your function, such as:

```c
/* myfunc.c: A new syscall */
#include <linux/linkage.h>    /* Essential for syscalls */
#include <linux/kernel.h>     /* To get some functions, e.g. printk */

asmlinkage int sys_myfunc(void) {
    printk("Hi, this is my syscall!");
    return(1);
}
```

Then, you have to give this call a syscall number, to do so, edit `include/asm/unistd.h` and define something, then increase the total number of syscalls by one:

```c
#define __NR_myfunc 294
#define NR_syscalls 295
```

Next, you have to define a point to reference to the call function. Edit `arch/i386/kernel/syscall_table.S`, where you can find the list of all syscalls. Get to the correct position and add a line:
```c
.long sys_myfunc   /* call no. 294 */
```
(note: many sources say that you should modify `arch/i386/kernel/entry.S`, but as of kernel version 2.6.15 that I am using, the syscall table is separated from `entry.S`)

So that's it. We go to find the kernel's makefile and add an object for that. Find an appripriate position in the make file and add this line:

```c
obj-y += myfunc.o
```
and then, make and use the new kernel.

On using the newly-created call, we just have to use the `_syscall0(type,name)` macro to define a function first. If your syscall have some parameters, you probably need `_syscall1` or `_syscall2` or so on. Example as follows:

```c
#include <linux/unistd.h>

_syscall0(int,myfunc);

int main(int argc, char *argv[])
{
    myfunc(buf);
}
```

## Partial list of system calls sorted in category

### Process control
  - exit (1) - Terminate the current process immediately
  - fork (2) - Create child process
  - waitpid (7) - Wait for a child process' state change, e.g. SIGSTOP, SIGCONT, SIGKILL
  - execve (11) - Execute another program **in place of** the current one
  - getpid (19) - Return the PID of the current process
  - setuid (23) - Set the effective UID of the current process
  - getuid (24) - Get the real UID of the current process
  - ptrace (26) - Parent process tracing child for every signal it received
  - nice (34) - Run a process with a specified priority
  - kill (37) - Send signal to process
  - times (43) - Get process times
  - brk (45) - Change data segment size
  - getgid (46) - Get the real GID of the current process
  - setgid (47) - Set the effective GID of the current process
  - geteuid (49) - Get the effective UID
  - getegid (50) - Get the effective GID
  - setpgid (56) - Set the process group ID of a specified process
  - getppid (64) - Get parent PID
  - getpgrp (65) - Get the process group ID of the current process
  - setsid (66) - Create a session and set the process group ID
  - setreuid (70) - Set real and effective UID of the current process
  - setregid (71) - Set real and effective GID of the current process
  - getgroups (80) - Get the list of supplementary group IDs
  - setgroups (81) - Set the list of supplementary group IDs
  - uselib (86) - Load a shared library
  - getpriority (96) - Get the priority of current process
  - setpriority (97) - Set the priority of current process

### File I/O
  - read (3) - Read from a file descriptor
  - write (4) - Write to a file descriptor
  - open (5) - Open a file and return the file descriptor
  - close (6) - Closes a file descriptor
  - lseek (18) - Position-seek in a file descriptor
  - dup (41) - Duplicate a file descriptor
  - pipe (42) - Create an unnamed pipe
  - fcntl (55) - Do low level manipulation on file descriptors
  - dup2 (63) - Duplicate a file descriptor to another one, closing the one to be modified if necessary
  - select (82) - Select a file descriptor for synchronous I/O
  - ftruncate (93) - Truncate a file by file descriptor to a specified length
  - fchmod (94) - Change mode of a file by file descriptor
  - fchown (95) - Change the owner of a file by file descriptor
  - fstatfs (100)- Get file system statistics
  - fstat (108) - Get statistics of a file by file descriptor

### File systems
  - creat (8) - Create a file and return the file descriptor
  - link (9) - Create hard link
  - unlink (10) - Delete a file
  - mknod (14) - Make a file system node (can be a file, device, or pipe)
  - chmod (15) - Change a file's mode
  - lchown (16) - Change the owner of a file (does not follow symlinks)
  - mount (21) - Mount a file system
  - umount (22) - Unmount a file system
  - utime (30) - Change the atime or mtime of a file
  - access (33) - Check if a file is allowed to be read/written or exists
  - sync (36) - Commit buffer cache to disk
  - rename (38) - Change a file's name
  - mkdir (39) - Create a directory
  - rmdir (40) - Remove a directory
  - umask (60) - Set file creation mask
  - ustat (62) - Get stats of a mounted file system
  - symlink (83) - Create a symlink
  - readlink (85) - Read a symlink
  - swapon (87) - Turn on swapping
  - readdir (89) - Read a directory
  - truncate (92) - Truncate a file to specified length
  - statfs (99) - Get file system statistics
  - stat (106) - Get statistics of a file, link followed
  - lstat (107) - Get statistics of a file, symlink not followed
  - swapoff (115) - Turn off swapping

### Operation
  - chdir (12) - Change the CWD
  - chroot (61) - Change root (as of chroot-jail)


### Miscellaneous
  - time (13) - Get the number of seconds since epoch
  - stime (25) - Set the clock
  - alarm (27) - Set alarm clock to fire SIGALARM
  - pause (29) - Cause a process to sleep until a signal
  - signal (48) - Register signal action
  - acct(51) - Toggle on/off process accounting
  - ioctl (54) - Do low level control on devices
  - sigaction (67) - Examine and change a signal action
  - sigsuspend (72) - Suspend the current process and wait for a signal
  - sigpending (73) - Examine signals pending for delivery to the calling thread
  - sethostname (74) - Set the hostname
  - setrlimit (75) - Set resource (CPU, mem, etc) limits (soft and hard)
  - getrlimit (76) - Get resource limits
  - getrusage (77) - Get resource usages
  - gettimeofday (78) - Get the time of the day
  - settimeofday (79) - Set the time of the day
  - reboot (88) - Reboot the system or enable/disable Ctrl-Alt-Delete
  - mmap (90) - Map a file/device into memory
  - munmap (91) - Unmap a file/device from memory
  - ioperm (101) - Set port I/O permissions
  - socketcall (102) - Socket system calls
  - syslog (103) - Read the kernel log
  - getitimer (104) - Get the interval timer
  - setitimer (105) - Set the interval timer

## Links
- [Linux Assembly](http://asm.sourceforge.net/)
- [Reference to Linux 2.{2,4,6} System Calls for Assembly Level Access](http://www.lxhp.in-berlin.de/lhpsyscal.html)
- [System calls table](http://docs.cs.up.ac.za/programming/asm/derick_tut/syscalls.html)
- [Tutorial - Linux System Call](http://kt.squeakydolphin.com/linux_syscall.jsp

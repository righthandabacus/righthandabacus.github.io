---
layout: post
title: "Hackintosh"
date: 2012-02-17 22:51:57 -0500
tags: system
category: blog
---
Successfully built a Hackintosh. This is the notes:

  - System: Asus P6X58D Premium, Quad-core i7, 12GB RAM, dual Radeon HD 5770
  - Use iBoot and Snow Leopard DVD to install. Follow the instruction on <http://tonymacx86.blogspot.com/2010/04/iboot-multibeast-install-mac-os-x-on.html>, i.e.
      - Set SATA to ACHI mode in BIOS
      - Remove memory to only 6GB, and keep only one Radeon card
      - Boot with iBoot, replace with SL DVD, type `GraphicsEnabler=No` to start the installation
      - Format the hard disk with GUID partition table, then install Snow Leopard
      - After installation, during reboot, boot with iBoot again, and still use `GraphicsEnabler=No` option
      - The screen now is at 1024x768, as the video driver not found
      - The complete the installation, and update SL to 10.6.8 using the [combo update](http://support.apple.com/kb/DL1399) from Apple
      - Run MultiBeast before the combo update reboots. Use EasyBeast install with System Utilities only, otherwise the system will break (a 32-bit vs 64-bit problem on kext?)
      - Reboot, the video works, but Ethernet/Audio is not
  - To fix Ethernet, follow <http://www.insanelymac.com/forum/index.php?showtopic=120647>:
      - modify `/System/Library/Extensions/IONetworkingFamily.kext/Contents/PlugIns/AppleYukon2.kext/Contents/Info.plist`, copy the block of Yukon-88E8053 to Yukon-88E8056 and change `0x436211ab` to `0x436411ab`, also the name string
  - To fix audio, follow <http://www.insanelymac.com/forum/index.php?showtopic=222761>, get the zip file and put the content to `/Extra`, then rebuild the kexts with kext utility
  - Reboot put the RAM and Radeon card back, then everything's ready.

Update: After putting the second Radeon card back, the boot was very slow. But subsequent boot is just fine. Probably due to cache rebuilding after hardware change.

Edit: On a second installation on the same hardware, I found that the fix of Ethernet can be done using the zip file `P6X58D Premium Extra.zip` from <http://www.insanelymac.com/forum/index.php?showtopic=222761>, but the audio never works (strange!). The fix of the audio, however, is to use `LegacyHDA_ALC889a-1.kext.zip` (put to `/Extra/Extensions`) and `AppleHDA.kext.zip` (replace the kext at `/System/Library/Extensions`) from <http://www.osx86.net/downloads.php?do=file&id=1214>. Afterwards, run kext utility to rebuild the cache, then they works immediately without reboot.

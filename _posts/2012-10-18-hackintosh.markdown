---
layout: post
title: "Hackintosh on Mountain Lion"
date: 2012-10-18 15:45:42 -0400
tags: system
category: blog
---
To prepare for evacuation from my current work location, I decided to polish the computer that I intended to bring it back home. I got a Western Digital 3TB hard disk and want to install Mountain Lion on it.

Installing Hackintosh is always a time-consuming task. Here is what I found:

My system is Asus P6X58D Premium. It comes with Realtek ALC889 audio and Marvell Yukon 88E8056 Gigabit Ethernet. To install, get the Mountain Lion's installation app (the one that you can download from Apple AppStore) and have [myHack](http://myhack.sojugarden.com/guide/) ready. The motherboard should contain dual Gigabit Ethernet but in Mountain Lion, you should enable only the *second* Ethernet and disable the first. The reason it is necessary is unknown. But enabling the first Ethernet will always have the finished Mountain Lion (i.e. after you installed the Ethernet driver, either by modifying AppleYukon.kext in Terminal or install a new kext) failed to boot.

First, follow myHack's guide to make the USB installation drive. Then open the drive and put two more files into it:

  - Multibeast, I am using version 5.1, available <http://www.tonymacx86.com/downloads.php?do=file&id=147>
  - The file boot1h from Chimera boot loader. Chimera is available <http://www.tonymacx86.com/downloads.php?do=file&id=136>. We use [unpkg](http://www.timdoug.com/unpkg/) to unpack Chimera.app and obtain boot1h from it. This is necessary only if you are using 4096-byte sector hard disk.

Once the USB drive is ready, boot with it and install into the 3TB hard disk. For a fresh drive, we need to create a partition for it with GUID partition table. Once the installation is almost done, myHack will ask for the additional installation. Just follow the guide to install the off-the-shelf `/Extra` directory and remove all the three unnecessary kext.

On the first boot, the system will fail because the 3TB hard disk is running 4096-byte sectors instead of the traditional 512-byte one. Therefore, boot with the USB installation disk again. Right after you select the language, launch disk utility to unmount the hard disk. Then launch terminal to

    dd if=boot1h of=/dev/disk0s2

where `/dev/disk0s2` is the hard disk partition that Mountain Lion is installed, and the file boot1h is the one obtained from Chimera and put into the USB drive. This makes the boot loader knows about the 4096-byte sector and then the system can successfully boot.

Afterwards, use Multibeast to install *just* the audio driver, Non-DSDT Realtek ALC889. Doing so needs to modify the system security to allow "any application" to run. Then use Terminal to edit `/System/Library/Extensions/IONetworkingFamily.kext/Contents/PlugIns/AppleYukon2.kext/Contents/Info.plist` to enable the Marvell Yukon 88E8056 Ethernet, i.e. find for `88E8053` and make a copy of the block

```html
<key>Yukon-88E8053</key>
<dict>
        <key>CFBundleIdentifier</key>
        <string>com.apple.iokit.AppleYukon2</string>
        <key>EnableLowPwr</key>
        <integer>1</integer>
        <key>IOClass</key>
        <string>yukon2osx</string>
        <key>IOPCIPrimaryMatch</key>
        <string>0x436211ab</string>
        <key>IOProviderClass</key>
        <string>IOPCIDevice</string>
        <key>MACNumber</key>
        <integer>1</integer>
        <key>Model</key>
        <string>Yukon Gigabit Adapter 88E8053 Singleport Copper SA</string>
        <key>NetworkNumber</key>
        <integer>1</integer>
        <key>RxDeadman</key>
        <integer>0</integer>
        <key>RxRingGrowOnPause</key>
        <integer>10</integer>
        <key>RxRingSize</key>
        <integer>0</integer>
        <key>RxRingSize_100MBit</key>
        <integer>128</integer>
        <key>RxRingSize_10MBit</key>
        <integer>64</integer>
        <key>RxRingSize_GigaBit</key>
        <integer>256</integer>
        <key>TxRingSize</key>
        <integer>256</integer>
        <key>Vendor</key>
        <string>Marvell</string>
</dict>
```

make a duplicate and modify it into

```html
<key>Yukon-88E8056</key>
<dict>
        <key>CFBundleIdentifier</key>
        <string>com.apple.iokit.AppleYukon2</string>
        <key>EnableLowPwr</key>
        <integer>1</integer>
        <key>IOClass</key>
        <string>yukon2osx</string>
        <key>IOPCIPrimaryMatch</key>
        <string>0x436411ab</string>
        <key>IOProviderClass</key>
        <string>IOPCIDevice</string>
        <key>MACNumber</key>
        <integer>1</integer>
        <key>Model</key>
        <string>Yukon Gigabit Adapter 88E8056 Singleport Copper SA</string>
        <key>NetworkNumber</key>
        <integer>1</integer>
        <key>RxDeadman</key>
        <integer>0</integer>
        <key>RxRingGrowOnPause</key>
        <integer>10</integer>
        <key>RxRingSize</key>
        <integer>0</integer>
        <key>RxRingSize_100MBit</key>
        <integer>128</integer>
        <key>RxRingSize_10MBit</key>
        <integer>64</integer>
        <key>RxRingSize_GigaBit</key>
        <integer>256</integer>
        <key>TxRingSize</key>
        <integer>256</integer>
        <key>Vendor</key>
        <string>Marvell</string>
</dict>
```

i.e. change the two occasions of the name `88E8053` into `88E8056`, and change the code `0x436211ab` into `0x436411ab`. Then use disk utility to fix permissions and reboot the system. At this point the system is ready.

Update: After a while (install apps, update to 10.8.2, etc) the Hackintosh hangs and cannot boot with the Ethernet. The solution is to drain all the power from the motherboard before you boot again. That is, unplug the power cord and wait for a while. Obviously Apple's Yukon2 driver is buggy.

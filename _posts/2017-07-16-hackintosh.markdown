---
layout: post
title: Hackintosh Update - Sierra
date: Sun, 16 Jul 2017 23:48:52 -0400
tags: system
category: blog
---

Updated the hackintosh system, from mountain lion directly to Sierra. It is
much much easier to do than previously. Some points to note:

- Update is performed by USB drive. Create one using Unibeast and the Sierra
  installer downloaded from Apple store
  - I zip the installer app for archive, but it turns out that you should only
    zip and unzip it using GUI finder. The command line zip cannot do that
    correctly
- Boot with the USB: My motherboard Asus P6X58D do not recognize the USB drive
  as removable device, but instead, it is an ordinary hard disk. I should
  change the boot disk from the SATA drive to USB drive in the BIOS setting,
  instead of changing the boot device.
- Installation over the existing OS goes fine. But I am not sure everything work
  out of box if I installed from scratch.
- Unibeast will use Clover as the bootloader. The USB version can successfully
  boot the system but the version installed by Unibest to the hard disk does
  not. The issue is on the ACPI configuration. Furthermore, that Clover does not
  boot the default disk upon time out; a press of Enter key is required.
- Update of Clover: It seems that the version of Clover doesn't matter. All
  complains can be fixed by a proper Clover config.
  - There is a config in `/EFI/CLOVER/config.plist` but it is not the one used
    by Clover at boot
  - The config in effect is in EFI partition: `/CLOVER/config.plist`
  - To update, use Clover Configurator, then:
    1. Let Clover Configurator detect for the EFI partitions in all disks
    2. Select the boot disk, mount the EFI partition
    3. Load the config file from the mounted EFI partition
    4. Update parameters, save
  - As a record, this is set in the Clover config:
    - Boot args: `dart=0` and `nv_disable=1` (boot args are optional)
    - ACPI: Table drop = nothing, Drop OEM = nothing, DSDT fix: `0x9051` (Add
        DTGP, Fix HPET, Fix IPIC, Fix USB, Fix HDA), this is important
    - Default boot partition: Instead of "Last Boot", must say the partition's
        name, or otherwise the timeout is meaningless. Once this is set, the
        partition will be boot upon timeout
- Post installation: Audio is soft, need to adjust the boost in system
    preference, Voodoo HDA

Some other fine tuning for SSH in macOS:

- Pass on locale:
   - /etc/ssh/ssh_config: Add `SendEnv LANG LC_*`
   - /etc/ssh/sshd_config: Add `AcceptEnv LANG LC_*`
   - Alternative: instead of ssh_config, do it in ~/.ssh/config under repsective Host section, e.g.

```
Host *,!<somehost>
    SendEnv LANG LC_*
```

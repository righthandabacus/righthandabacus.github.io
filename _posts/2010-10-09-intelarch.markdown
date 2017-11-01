---
layout: post
title: "Computer Architecture (x86)"
date: 2010-10-09 00:14:48 -0400
tags: system
---

## CPU and Peripherals
The architecture of a CPU and its connection to peripherals: (source - Wikipedia)

![](http://upload.wikimedia.org/wikipedia/commons/thumb/b/bd/Motherboard_diagram.svg/500px-Motherboard_diagram.svg.png)

The northbridge (aka MCH) is usually responsible for high-speed devices such as
memory and southbridge (aka ICH), as further away from CPU, is responsible for
slower devices. In modern Intel architecture, such as Core2 systems, the memory
bus is the DDR2/DDR3 channels. The internal bus connecting the northbridge and
the southbridge is usually proprietary to the chipset maker. Traditionally that
was PCI but the chipset maker invent their own for higher speed connections.

## Memory and Peripherals

The devices that connects to southbridge, such as BIOS ROM and keyboard, are
accessible through *memory-mapped IO*. In other words, when the CPU access to
certain particular physical memory address, it is actually accessing the
peripheral hardware. Whether a particular address should correspond to the real
RAM module or should be redirected to a peripheral is handled by the
northbridge.

In the good old days of DOS we have the following separation of memory under the 1MiB range:

| 0 -- 640KB    | Main memory |
| 640 -- 768KB  | Video card access |
| 768 -- 896KB  | Expansion area, which mapped to peripherals |
| 896 -- 960KB  | Extended system BIOS |
| 960 -- 1024KB | System BIOS |

To check such memory-mapped IO locations in Linux, we can issue

    cat /proc/iomem

This is what I can see from my AMD64 system:

```
00000000-0009efff : System RAM
0009f000-0009ffff : reserved
000ce200-000cffff : pnp 00:0a
000f0000-000fffff : reserved
00100000-7fecffff : System RAM
  01000000-012af442 : Kernel code
  012af443-01472baf : Kernel data
  01510000-015d6973 : Kernel bss
7fed0000-7fed2fff : ACPI Non-volatile Storage
7fed3000-7fedffff : ACPI Tables
7fee0000-7fefffff : reserved
7ff00000-7fffffff : RAM buffer
e0000000-efffffff : PCI Bus 0000:03
  e0000000-efffffff : 0000:03:00.0
f0000000-f3ffffff : PCI MMCONFIG 0000 [bus 00-3f]
  f0000000-f3ffffff : reserved
    f0000000-f3ffffff : pnp 00:09
fa000000-fcffffff : PCI Bus 0000:03
  fa000000-faffffff : 0000:03:00.0
    fa000000-faffffff : nvidia
  fb000000-fbffffff : 0000:03:00.0
  fc000000-fc01ffff : 0000:03:00.0
fd900000-fd9fffff : PCI Bus 0000:02
  fd9f0000-fd9fffff : 0000:02:00.0
    fd9f0000-fd9fffff : tg3
fda00000-fdafffff : PCI Bus 0000:01
fdb00000-fdbfffff : PCI Bus 0000:04
fdc00000-fdcfffff : PCI Bus 0000:04
fdd00000-fddfffff : PCI Bus 0000:01
fde00000-fdefffff : PCI Bus 0000:02
fe02c000-fe02cfff : 0000:00:0f.0
  fe02c000-fe02cfff : sata_nv
fe02d000-fe02dfff : 0000:00:0e.0
  fe02d000-fe02dfff : sata_nv
fe02e000-fe02e0ff : 0000:00:0b.1
  fe02e000-fe02e0ff : ehci_hcd
fe02f000-fe02ffff : 0000:00:0b.0
  fe02f000-fe02ffff : ohci_hcd
fec00000-ffffffff : reserved
  fec00000-fec003ff : IOAPIC 0
  fec80000-fecbffff : pnp 00:01
  fee00000-feefffff : pnp 00:0a
    fee00000-fee00fff : Local APIC
  fefe0000-fefe01ff : pnp 00:01
  feff0000-feff03ff : HPET 0
  fefff000-feffffff : pnp 00:0a
  fff80000-fff80fff : pnp 00:0a
  fff90000-fffbffff : pnp 00:0a
  fffed000-fffeffff : pnp 00:0a
  ffff0000-ffffffff : pnp 00:0a
```

## Modes

In the 8088 days, there is only one mode: the real mode. But now we have 32-bit
protected mode (since 80386) and 64-bit protected mode.

In real mode, the memory are accessed in 16 bit. As in my undergraduate
microprocessor textbook, the memory is expressed like 1000:64EF. The number
before the colon is a segment base address and the one after the colon is the
offset. The calculation is to shift the base address by 4 bits and add up the
offset. So that address means the byte of 0x164EF in physical address.

In real mode, only 16-bit segment address and 16-bit offset is allowed. It makes
the addressable space up to 1MiByte. Moreover, as a legacy from 16-bit real
mode, we have a near pointer and far pointer in programming jargon to mean
whether we specify the segment base address or just the offset address in
describing the memory. So when we use a near pointer (which supposed to be
faster in CPU processing), the maximum memory that we can use is only 64KiB.
This is also the size limitation of *.COM commands in DOS.

In 32-bit protected mode, however, we cannot specify a physical address directly
(hence the physical memory is protected). The memory in protected mode is
segmented. There would be a table of segment descriptors, of which is 64-bits
each. In the segment descriptor, there is a 32-bit base address and 20-bit
segment size. The exact format of a segment descriptor is as follows: (This is
the segment descriptor in 80386. In 80286, which is the first one to support
protected mode, the last two bytes are unused)

```
     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
0x00|                 limit (bit 0-15)              |
0x10|            base addr (bit 0-15)               |
0x20| base addr (bit 16-23)  |     attribute #1     |
0x30|lim(b16-19)| attrib #2 | base addr (bit 24-31) |
```

When access to memory is requested, there should be a 16-bit segment selector
and a 32-bit offset. It is called segment selector because it is not directly
specifying an address but, instead, specifies which segment descriptor the
offset is relative to.

Among the 16-bit segment selector, there are 2 bits of a privilege level (lower
number is higher privilege), 1 bit of table indicator (global descriptor table
or local descriptor table), and 13 bits of descriptor ID in the table (actually
the serial number of descriptor in the table, first one is zeroth). The
privilege level controls whether an application can access to a piece of memory.
In the segment descriptor, there is a descriptor privilege level (DPL, in bit
45-46). The application also has a current privilege level (CPL, stored in the
code segment selector). What the segment selector supplied is the requested
privilege level (RPL). Access is granted only if DPL ≥ max(CPL,RPL) and CPL ≤
max(DPL,RPL); or general page fault is issued otherwise.

The segment selector has the following format:

```
 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
| RPL |TI|             segment ID              |
```

When the access is eligible, the physical address accessed is computed by adding
the base address to the offset address, resulting in another 32-bit location.
However, the offset address must not be larger than the limit or a GPF is
issued.

## References
  * [Intel® 64 and IA-32 Architectures Software Developer's Manuals](http://www.intel.com/products/processor/manuals/index.htm)
  * http://duartes.org/gustavo/blog/post/2008/06



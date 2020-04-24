---
layout: post
title: A strange case in DD-WRT
date: Fri, 24 Apr 2020 12:56:54 -0400
tags: system
---

The Linux kernel in DD-WRT is a customized one. Some patch is applied and not
used elsewhere, not to mention on the upstream. This is what bothered me a
whole night to debug on a strange case in networking.

Normally my home network is all connected to a single switch so everything is
in a flat network (AP isolation is used for a guest-only WiFi, but not an issue
here). But I have a machine that has only wired Ethernet connection but I
cannot pull a wire to it due to physical obstacles. So the easy way out is to
go wireless. I have a spare WiFi router sitting around. So I make my machine
connected to this router and make it run in [client bridge
mode](https://wiki.dd-wrt.com/wiki/index.php/Client_Bridged) instead of
acting as an AP. The diagram is as follows:

```
[machine A] ---- [router 1] ~~~~ [router 2]---[other machines]
```

which the connection between routers 1 and 2 are wireless. Router 2 is the only
AP in the entire network. Both routers are running DD-WRT.

So far so good. Machine A can access to everywhere via the wireless connection
between routers 1 and 2. Essentially I made machine A and router 1 combined as
a machine with WiFi interface. But the problem arise when I try to access to
machine A from any node directly connected to router 2 (let's call it machine
B). What I found is the following:

1. Machine B cannot find machine A via ARP
2. If we add a static ARP entry on machine B, then A and B can talk
3. Running tcpdump on machine B sees ARP request for machine A but no reply
4. If ssh into router 2, we can ping machine A. So the ARP request-reply is fine there
5. Running tcpdump on router 2 while machine B trying to do ARP for machine A can see both request and reply

In the DD-WRT, all LAN connections are bound as a bridge device (`br0`) and
therefore the ARP packets should be forwarded over bridge. But it did not.
Either, I did not see any issue with the `iptables`. And there were no
`ebtables` or `arptables` turned on (the kernel modules for those are not
loaded, according to `lsmod`). The `sysctl` attributes are all fine -- all
ARP-related entries are at their default value (indeed, no `sysctl` command in
DD-WRT, but we can check those values in `/proc/sys/`).

Finally, I checked on the UI and something alerted me: ARP spoofing protection
in the firewall settings page. I see nothing on `iptables` related to ARP so it
should not be a feature implemented there. But how can I do ARP spoofing in
Linux?

Further investigation with `dmesg` on router 2 reveals the hint: ARP reply
packets are dropped for the kernel believes those are spoofed, because the MAC
address in the reply payload does not match the packet senders' MAC. But who did that?

Searching on the kernel error message point only to the DD-WRT pages. And the
following is found in the DD-WRT source code, at `src/linux/universal/linux-4.9/net/ipv4/arp.c`:

```c
#ifdef CONFIG_KERNEL_ARP_SPOOFING_PROTECT
int arp_spoofing_protect(struct sk_buff *skb)
{
	struct arphdr *arp;
	unsigned char *arp_ptr;
	unsigned char *sha;
	__be32 sip;
	struct net_device *dev = skb->dev;
	int i = 0;

	arp = arp_hdr(skb);

	/*
	*      Extract fields
	*/
	arp_ptr = (unsigned char *)(arp + 1);
	sha     = arp_ptr;
	arp_ptr += dev->addr_len;
	memcpy(&sip, arp_ptr, 4);

	for(i = 0;  i < MAX_ARP_SPOOFING_TABLE; i++) {
		if(szIPMac[i].szIPaddr == 0) {
			break;
		}
		if(sip == szIPMac[i].szIPaddr) {
			if(szIPMac[i].szMac[0]== *sha && szIPMac[i].szMac[1]== *(sha+1) &&
				szIPMac[i].szMac[2]== *(sha+2) && szIPMac[i].szMac[3]== *(sha+3) &&
				szIPMac[i].szMac[4]== *(sha+4) && szIPMac[i].szMac[5]== *(sha+5) ) {
				return 0;
			} else {
				unsigned char *ip = (unsigned char*)&sip;
				printk(KERN_ERR "arp spoofing detected ip=%d.%d.%d.%d mac is %02x:%02x:%02x:%02x:%02x:%02x but should be %02x:%02x:%02x:%02x:%02x:%02x\n", ip[0]&0xff,ip[1]&0xff,ip[2]&0xff,ip[3]&0xff,
				szIPMac[i].szMac[0], szIPMac[i].szMac[1], szIPMac[i].szMac[2], szIPMac[i].szMac[3], szIPMac[i].szMac[4], szIPMac[i].szMac[5],
				*sha, *(sha+1), *(sha+2), *(sha+3), *(sha+4), *(sha+5));
				return 1;
			}
		}
	}
	return 0;
}

EXPORT_SYMBOL(arp_spoofing_protect);
#endif
```

The `printk()` is why I see the message in `dmesg`. And I see this in the bottom of the same file:

```c
#ifdef CONFIG_KERNEL_ARP_SPOOFING_PROTECT
	if (!proc_create("arp_spoofing_enable", S_IRUGO, net->proc_net, &arp_spoofing_enable_fops))
		return -ENOMEM;
	if (!proc_create("arp_spoofing_table", S_IRUGO, net->proc_net, &arp_spoofing_table_fops))
		return -ENOMEM;
#endif
```

Therefore, this is how it is toggled: `echo 1 > /proc/net/arp_spoofing_enable`.
These code is not in the [vanilla Linux
kernel](https://github.com/torvalds/linux/blob/master/net/ipv4/arp.c). So
DD-WRT specific. And that is why Google is not helpful in answering my
question.

So the quick solution is to turn off the ARP spoofing protection on my AP
router. But of course, the other way is to make the ARP reply that forwarded by
the client bridge to carry the original L2 addresses. I am not sure this should
happen, however, as the machines are indeed in two different L2 network.

---
layout: post
title: WireGuard VPN
date: 2023-02-15
tags: system
---

WireGuard is regarded as the only right way to do VPN. There are quite many
benefit of using that. While the configuration is easy, you need to understand
what it is doing to properly set it up.

What WireGuard provide is an encrypted tunnel over UDP port. Packets are sent
over this UDP tunnel. In Linux, WireGuard is implemented as a kernel module. In
Mac, for example, you need to install the backend. But that's it. You don't
have a choice of the cipher. Therefore, to set up this UDP tunnel, all you need
is the peer's IP addres/port and three keys: The private/public key pair of
yourself and the public key of the remote peer. Same is true on the remote
peer: It should know its private/public key pair and your public key. There's
no server/client distinction on the tunnel, except one side should listen on
the UDP port and the other side is to initiate the connection.

So far so good, but how's the routing? That's not part of WireGuard indeed. As
long as the UDP tunnel is created, you have a virtual network device on your
machine. So you have to tell what should be routed over the tunnel, *on both
sides*.

Let's say, we make a WireGuard *server* on OpenWRT. It will listen on a UDP
port (e.g., 54321). To make it a *VPN server*, you should expose this port to
the Internet on OpenWRT.

These are the steps (for OpenWRT):

1. Packages should be installed on OpenWRT: `kmod-wireguard`, `wireguard-tools`, `luci-app-wireguard`, `luci-proto-wireguard`
   - implementation is `kmod-wireguard` (the kernel module)
   - the `wireguard-tools` is the command line tool; the `luci-*-wireguard` are for LuCI point and click
2. Run command line to generate a private/public key pair for this server:

    ```
    wg genkey | tee mywgserver.priv | wg pubkey > mywgserver.pub
    ```

3. At LuCI, create a new interface of type "WireGuard VPN"
   - read the two keys generated above, enter them in this interface
   - set IP address and subnet mask for this device, e.g., 192.168.0.1/24
   - pick the UDP port to listen on, e.g., 54321. The port is listened on 0.0.0.0 wildcard address, hence it is exposed to the Internet if the firewall allows
   - uncheck "Use DNS server advertised by peer" and optionally assign DNS servers. This is because as VPN server, it is not going to change the DNS server after connected by a client
   - at peer tab, add each peer: You will need,
     - the public key (private key is optional, if you want to generate QR code for easier connection)
     - assign "Allowed IPs" as the IP address for this peer, e.g., 192.168.0.123/32
     - check the box "Route Allowed IPs" for adding host route for this peer
     - suggested to set the "Persistent keepalive" to 25 seconds, especially if the server is behind NAT
   - assign this interface to LAN domain
4. Set firewall rule to allow all incoming packets from WAN to the WireGuard UDP port

On the client side (e.g., MacOS)

1. Download and install the client. The Mac client is available on Apple AppStore
2. Set up the tunnel by creating a new config as follows

```
# define the local WireGuard interface (client)
[Interface]
PrivateKey = <client private key here>
Address = 192.168.0.123/32
DNS = 192.168.1.1

# define the remote WireGuard interface (server)
[Peer]
PublicKey = <server public key here>
AllowedIPs = 192.168.0.1/32, 192.168.1.0/24
Endpoint = wireguard.example.com:54321
PersistentKeepalive = 25
```

Some other options are available but this is usually sufficient. The key here
is the `Address` and `AllowedIPs` on both end. You created a subnet when you
set up WireGuard. On server side, you probably assume the subnet is local but
the exception is the particular IP address of the peer side, in which you must
route over the UDP tunnel to reach that one. Similarly on the client side, when
you reach out to 192.168.0.1, it means you need to go via the UDP tunnel.
Similarly, for the network 192.168.1.0/24 behind the WireGuard sever, you also
need to go over the tunnel.

Ignoring the name, the configuration `AllowedIPs` is indeed list of reachable
IP or subnet that needs to route over the WireGuard tunnel. Hence you should
use it as above. If you want the client to connect to the entire 192.168.0.0/24
subnet instead of only the WireGuard server, you can do so by modifying this
option.

Also note that all IP addresses are hardcoded and no DHCP will be provided over
WireGuard. It is so because part of the encryption in WireGuard depends on the
IP address.

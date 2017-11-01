---
layout: post
title: "Al-Qudah et al (2010) Web Timeouts and Their Implications (PAM)"
date: 2010-06-03 12:50:56 -0400
category: paper
bibtex:
  type: inproceedings
  title: "Web Timeouts and Their Implications"
  author: "Zakaria Al-Qudah and Michael Rabinovich and Mark Allman"
  booktitle: "Proc PAM"
  year: 2010
---
Take a number of public web servers as subject, evaluates how they handles timeout events.

The web servers include some "top 500 sites", which are high volume sites, and 15K regular sites. It is reported that these set of web servers comprises of 53% Apache, 12% IIS, and 10% Google GWS in the high volume group, and 68% Apache, 19% IIS in the regular group.

The following are the measurements done:

**TCP timeout.**
Connect TCP, send HTTP request to server but no further ACK to the replied data. The servers will end the connection eventually by any of three methods. A majority (61%) of servers end the connection after retransmission ceasing. TCP retransmission only repeated 15 times in Linux default, for a total of 13-30 minutes. For 30% of the servers, the connection is end by a RST; and for 9% of the servers, the connection is end by a FIN. Because FIN is triggered by application to end the connection, it has a much shorter timeout, at around 10s on average. The timeout of retransmission ceasing and RST are both above 100s on average.

**Application timeout.**
Connect TCP without sending HTTP request. There are 36% of servers do not end the connection even after 20min. A reason for this may be due to the TCP_DEFER_ACCEPT option in Linux that does not promote a connection from SYN_RCVD to ESTABLISHED until the first data byte is received. In this case, the application never knows about the connection. For those which terminates connection, a mode is found around 120s and 300s, which are the defaults for IIS and Apache respectively. Another mode at 240s is found for GWS.

**Request timeout.**
Connect TCP and send a 1000-byte request at the rate of 1 byte/sec. This checks how long the server waits for the request to arrive. The connection is not dropped for 58% regular sites and 51% high volume sites. For those terminated the connection, more than 90% of sites have the timeout period of over 30s.

**Response timeout.**
Connect TCP and request a web page, but ACK at a slow rate of 100 bytes/sec. Only in IIS, there is a response timeout enforced. Its default minimum transfer rate is 240 bytes/sec. Amongst the sites, less than 25% has limit on transfer rates. Amongst them, 59% in the group of regular sites and 33% in the group of high volume sites are identified as IIS. Note, the response timeout behaviour can be obscured by a transparent proxy.

**Keep-alive timeout.**
It measures, after a request is completed, how long a server will keep the connection open. Some sites does not maintain keep-alive connections, which will terminate the connection right after the reply. There are 65% amongst the high volume sites and 76% regular sites maintain persistent connections. Apache defaults to 15s timeout and more than 60% high volume sites set the timeout to 30s. Regular sites has 30s timeout is only 32%.

The paper found that, with only 1 Mbps consumption of bandwidth, it is effective to launch claim-and-hold attack to bring down a web server. This is done by making use of the various timeout in the server.

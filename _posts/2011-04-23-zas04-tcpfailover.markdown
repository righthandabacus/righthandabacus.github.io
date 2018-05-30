---
layout: post
title: "Zhang et al (2004) Efficient TCP Connection Failover in Web Server Clusters (INFOCOM)"
date: 2011-04-23 03:18:48 -0400
category: paper
bibtex:
  type: inproceedings
  title: Efficient TCP Connection Failover in Web Server Clusters
  author: Ronghua Zhang and Tarek E Abdelzaher and John A. Stankovic  
  booktitle: Proc INFOCOM
  year: 2004
---
Brief read.

This paper is about TCP failover in a web environment. It assumes that a web
service is provided by one frontend and $$N$$ backends. The backend servers are
identical and in a ring structure so that one covers each other for the
failure. Any one of the $$N$$ backend alive can respond to the request. The
frontend receives requests from the client and forward to the backends. All
backend receives the same request but only one is really handling it as the
frontend only allows the response packet from one of them to pass through.

According to TCP state machines, all backend receives the SYN packet from
client but only one of them would have its SYN/ACK packet returned to the
client. Upon client's ACK packet arrives, those who are not active as
determined by the frontend will find the ACK packet out of syn and dropped its
connection.

The frontend keeps track on the healthiness of backends, e.g. by heartbeat.
Failover is enabled by replay of data packets from client: When a backend
fails, the frontend selects another one and resend all the client packets to
the new backend. After the frontend rebuilds the connection, the data are
forwarded to the client as if the connection is not broken. Therefore, before
the TCP connection is finished, the frontend will hold the full transaction of
packets.

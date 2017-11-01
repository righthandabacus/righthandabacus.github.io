---
layout: post
title: "Floyd et al (2007) Quick-Start for TCP and IP (RFC4782)"
date: 2012-02-01 22:10:40 -0500
category: paper
bibtex:
  type: misc
  title: "Quick-Start for TCP and IP"
  author: "S. Floyd and M. Allman and A. Jain and P. Sarolahti"
  howpublished: "IETF RFC4782"
  url: "http://www.ietf.org/rfc/rfc4782.txt"
  month: "Jan"
  year: "2007"
---
Quick start is an option in IP layer of a TCP SYN packet so that any router that forwards such IP packet may answer the request of bandwidth in it. The other side of TCP returns the router's answer. Quick start is in effect only when all the intermediate routers recognizes this option.

The quick start option is as follows:

     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Option      |  Length=8     | Func. | Rate  |   QS TTL      |
    |               |               | 0000  |Request|               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                        QS Nonce                           | R |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

The rate request is encoded into 4 bits, which expresses the rate from 80 Kbps (1) to 1310720 Kbps (15), with each code point doubles the previous. The rate request 0 is specially assigned to 0 Kbps, which allows the Quick-Start router to reject any rate request.

When a sender is performing Quick-Start rate request, it puts a QS TTL in the option, which is independent of the IP TTL. Any intermediate router must decrement both IP TTL and QS TTL at the same rate. The router may approve a lower rate by decreasing the rate request. When the Quick-Start request returned, the initial difference between IP TTL and QS TTL must be maintained. Otherwise, it is an indication that some intermediate router does not honor QS, hence the rate returned cannot be used.

When the Quick-Start rate request reaches the destination, the result is returned in the returning TCP acknowledgement packet. Afterwards, when the sender received the TCP return, it must send a "rate report" packet in a similar fashion as the previous IP option, to let all intermediate routers know about the final rate agreed.

     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Option      |  Length=8     | Func. | Rate  |   Not Used    |
    |               |               | 1000  | Report|               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                        QS Nonce                           | R |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

After the TCP sender received the response of rate request, it must use the rate at the first RTT after received it. The TCP implements a Quick-Start congestion window, which is set to

`QS-cwnd = (R * T * MSS) / (MSS + H)`

with R is the respond rate, T is the RTT, MSS+H is the maximum segment size plus the header size. The QS-cwnd shows how much bytes the TCP can send in an RTT. If QS-cwnd is smaller than the default cwnd, it is ignored. Otherwise, it is used as the effective congestion window.
When QS-cwnd is used, the TCP is in Quick-Start mode, instead of slow start. During quick start mode, any ACK received for packets sent before quick start is processed as normal. But any ACK received for packets sent during quick start ends the quick start, and TCP returns to the default congestion control mechanism, i.e. slow start if it was slow start before and no packet loss detected ever, or congestion avoidance otherwise.

Essentially QS means to allow TCP to jump to certain agreed rate at a RTT without the need to probe for the available bandwidth.

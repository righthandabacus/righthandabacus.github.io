---
layout: post
title: "ACID, BASE, and CAP theorem"
date: 2012-11-21 15:48:06 -0400
tags: code
---

## ACID

In the database world, ACID is an acronym that says, all database transactions should be:
  * atomic: transaction is either succeed or entirely rolled back
  * consistent: transaction never invalidates the database state
  * isolated: transactions should not interfere with each other but done as if they are sequential
  * durable: completed transactions persist, i.e. what you wrote is what you read

The ACID property is a strict requirement for database. It impairs the availability and performance. For example, ACID makes database making frequent locks on tables to guarantee strong consistency. The lock hurts I/O throughput and make the database not responsive until the lock is over.

## BASE

BASE is described as the other end of a spectrum by Eric Brewer is his 2000 PODC keynote speech. It is:
  * basically available: system looks to work and responsive at any time
  * soft-state: no need to be consistent at all time
  * eventually consistent: but become consistent at some later time

BASE means that, consistency after every transaction is not required, as long as the database is eventually in a consistent state. Therefore, using stale data or providing approximate answers are tolerated. In essence, BASE is a best-effort design that gives up strong consistency for weak consistency.

## CAP Theorem

The CAP theorem is given out by Eric Brewer in his PODC keynote speech and proved by Gilbert and Lynch in 2002. It says that it is impossible to achieve all three of:
  * consistency: i.e., atomicity in ACID
  * availability: i.e., responsiveness of the system
  * partition tolerance: i.e., system works (amid partially) even with partial failure

Examples of systems applying CAP theorem:
  * single-site database: Forfeits P for C and A
  * distributed database with distributed locking: Forfeits A for C and P
  * web caching and DNS: Forfeits C for A and P

Therefore, in dealing with the CAP theorem, the strategy in designing a distributed system are:
  - Drop partition tolerance:
    * put everything into one machine
    * imposes scaling limits
  - Drop availability
    * upon partition event (i.e. partial system failure), affected service simply wait
    * until data is consistent, system remain unavailable/irresponsive
  - Drop consistency
     * accept things will become eventually consistent

Brewer in his speech uses the yield/harvest to illustrate the trade-offs: His $$DQ$$ principle says that, data per query $$D$$ times query per second $$Q$$ is a constant ($$DQ$$). And he considers *yield* ($$Q$$) as the fraction of queries answered (i.e. received and processed successfully) and *harvest* ($$D$$) the fraction of complete result. In an ACID database, we have 100% harvest for lower yield, i.e. trade $$Q$$ for highest $$D$$. In the internet, we maintain high availability $$Q$$ for a reduced $$D$$. If we design a database to ignore lost nodes so that small part of the database is forfeit, we trade $$D$$ for higher $$Q$$, or give up consistency to maintain availability. If we pair up nodes to be resilient on single node failure, we trade $$Q$$ for higher $$D$$, or give up availability for consistency.

## Reference
  * <http://www.johndcook.com/blog/2009/07/06/brewer-cap-theorem-base/>
  * <http://www.julianbrowne.com/article/viewer/brewers-cap-theorem>
  * E. A. Brewer (2000) Towards robust distributed systems. In Proceedings of the 19th Annual ACM Symposium on Principles of Distributed Computing (July 16-19, Portland, Oregon), p.7. Available <http://www.cs.berkeley.edu/%7Ebrewer/cs262b-2004/PODC-keynote.pdf>
  * S. Gilbert and N. Lynch (2002) Brewer's conjecture and the feasibility of consistent, available, partition-tolerant Web services. ACM SIGACT News 33(2). Available <http://portal.acm.org/citation.cfm?doid=564585.564601>

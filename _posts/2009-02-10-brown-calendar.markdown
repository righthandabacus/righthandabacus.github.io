---
layout: post
title: "Brown (1988) Calendar Queues: A Fast O(1) Priority Queue Implementation for the Simulation Event Set Problem"
date: 2009-02-10 23:47:01 -0500
category: paper
bibtex:
  type: article
  title: "Calendar Queues: A Fast O(1) Priority Queue Implementation for the Simulation Event Set Problem"
  author: "Randy Brown"
  journal: "Communications of the ACM"
  volume: 31
  number: 10
  pages: "1220--1227"
  month: October
  year: 1988
---
This is the seminal paper that proposes the calendar queues which can be used for DES. Events are classified into "days" and different "days" are put into an array as a "year". When we insert, put the event into an appropriate day according to the event's deadline. When we dequeue an event, look for the events in today's day-bucket. This is virtually a divide and conquer approach which reduce the number of events in a bucket and hence speed up the process.

This paper also pointed out that when the "year" is too big or too small, or a "day" has too much event, the performance is not optimal. The former is because when we scan for an event, we may encounter too much empty "days" or encounter too much non-empty days but the events are scheduled for the next year. When it is the latter, however, there are too much events in the bucket and hence it is not good. This paper proposes to resize the calendar queue when necessary, i.e. the total number of event and the number of days in the year are not in proportion. It is proposed that resize occur when number of event exceeds twice the number of day buckets or when it is less than half the number of day buckets.

Another issue with the calendar queue is that, if currently the time is year 0 and the next event is in year 100, then the normal operation will scan for the queue for 100 times before finding an event. This is improved if, when we search for an event and found no scheduled event for one whole year, instead of searching for next year, we search for the highest priority event by a second scan. This can make sure we found a event within two year's term.

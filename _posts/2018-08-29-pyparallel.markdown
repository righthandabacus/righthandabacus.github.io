---
layout: post
title: Parallel computing in Python standard library
date: Wed, 29 Aug 2018 10:40:37 -0400
tags: code
category: blog
---

There are multiple ways to do parallel computing using only the standard
library in Python. There are vastly more way to do [parallel processing and
multiprocessing](https://wiki.python.org/moin/ParallelProcessing) if third-party
modules are used.

# subprocess

`subprocess` module is not for SMP but allow to run a command line in a separate process

```python
import subprocess

# no output capture
subprocess.run(["ls","-l"])
# with output capture
output = subprocess.check_output(["ls","-l"]).decode("utf-8")
```

# Threads

The `threading` module allows a job defined as a thread class and run thread
(beware of GIL)

```python
import threading
import time

class mywork(threading.Thread):
   def __init__():
       super(self).__init__()
   def run(self):
       time.sleep(10)

jobs = [mywork() for _ in range(1,10)]
[t.start() for t in jobs]
[t.join() for t in jobs]
```

For simple function that do not need a class, we can create thread
objects directly from a function:

```python
import threading
threads = [threading.Thread(target=time.sleep, args=(10,)) for _ in range(1,10)]
[t.start() for t in threads]
[t.join() for t in threads]
```

# multiprocessing

Multiprocessing do not limited by the GIL problem. Easiest way
to run multiprocess is to set up a process pool:

```python
import multiprocessing

def cube(x):
    return x**3

pool = multiprocessing.Pool()
result = pool.map(cube, range(1,7))
# above is identifical to below
result = [pool.apply(cube, args=(x,)) for x in range(1,7)]
```

This will run in parallel equivalent to

    result = [cube(x) for x in range(1,7)]	

alternative to `pool.map()` and `pool.apply()` we also have
`pool.map_async()` and `pool.apply_async()`. The async version
is non-blocking, i.e., after param are submitted to function
in another process, the control is back to the main
program without waiting for the result to get back. To use
async version,

```python
import multiprocessing

def cube(x):
    return x**3

pool = multiprocessing.Pool()
processes = [pool.apply_async(cube, args=(x,)) for x in range(1,7)]
result = [p.get() for p in processes]
```

Without pool, we can run as much processes as created, all
in parallel without staging. Note this is like a drop-in replacement
of the threading code above.

```python
import multiprocessing
processes = [multiprocessing.Process(target=process_function, args=(42, False)) for _ in range(N)]
[p.start() for p in processes]
[p.join() for p in processes]
```

# Using job queue

The threading module do not have thread pool like the
multiprocessing pool. So
the orthodox way is to use a job queue to hold arguments and let
the functions in threads to consume the queue in a loop. We may
need other tools like semaphore and locks.

```python
import time
from queue import Queue
from threading import Thread

NUM_WORKERS = 4
num_queue = Queue()

def worker():
    while True:
        num = num_queue.get()
        cube(num)
        num_queue.task_done() # mark the num we get() is done
threads = [Thread(target=worker) for _ in range(NUM_WORKERS)]
[num_queue.put(item) for item in range(1,10)]
[thread.start() for thread in threads]
num_queue.join() # block and wait until all num are marked as done
```

Formally, the code above should add a condition to make the while
loop to end and then join all threads.

# concurrent.futures

If we need a thread pool from standard library, we use the higher
level `concurrent` module. Thread pool executor:

```python
import concurrent.futures
import urllib.request

URLS = ['http://www.foxnews.com/',
        'http://www.cnn.com/',
        'http://europe.wsj.com/',
        'http://www.bbc.co.uk/',
        'http://some-made-up-domain.com/']

# Retrieve a single page and report the URL and contents
def load_url(url, timeout):
    with urllib.request.urlopen(url, timeout=timeout) as conn:
        return conn.read()

# We can use a with statement to ensure threads are cleaned up promptly
with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
    # Start the load operations and mark each future with its URL
    future_to_url = {executor.submit(load_url, url, 60): url for url in URLS}
    for future in concurrent.futures.as_completed(future_to_url):
        url = future_to_url[future]
        try:
            data = future.result()
        except Exception as exc:
            print('%r generated an exception: %s' % (url, exc))
        else:
            print('%r page is %d bytes' % (url, len(data)))
```

Also we have process pool executor:

```python
import concurrent.futures
import math

PRIMES = [
    112272535095293,
    112582705942171,
    112272535095293,
    115280095190773,
    115797848077099,
    1099726899285419]

def is_prime(n):
    if n % 2 == 0:
        return False

    sqrt_n = int(math.floor(math.sqrt(n)))
    for i in range(3, sqrt_n + 1, 2):
        if n % i == 0:
            return False
    return True

def main():
    with concurrent.futures.ProcessPoolExecutor() as executor:
        for number, prime in zip(PRIMES, executor.map(is_prime, PRIMES)):
            print('%d is prime: %s' % (number, prime))
```

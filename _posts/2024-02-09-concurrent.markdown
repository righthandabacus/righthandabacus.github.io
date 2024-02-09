---
layout: post
title: concurrent.futures in Python
date: 2024-02-09
tags: code
category: blog
---

The Python standard library `concurrent.futures` is the easiest way to run
parallel jobs in the best-effort manner. In case the heavy jobs are run
off-interpreter (e.g., NumPy) using the thread pool from `concurrent.futures`
can give you some noticeable performance benefit.

One example of `concurrent.futures` is as follows:

```python
import concurrent.futures
import datetime
import math
import resource

def worker(n):
    print(f"{datetime.datetime.now()}: worker {n} started")
    x = []
    for i in range(10_000_000):  # just some slow operation
        x.append(math.sin(i))
    print(f"{datetime.datetime.now()}: worker {n} return")
    return x

def main():
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as executor:
        jobs = [executor.submit(worker, n) for n in range(50)]
        for future in concurrent.futures.as_completed(jobs):
            _ = future.result()
            print(f"{datetime.datetime.now()}: Memory usage {resource.getrusage(resource.RUSAGE_SELF).ru_maxrss}")

if __name__ == "__main__":
    main()
```

This is a simplest way of using `concurrent.futures`: First create the thread
pool executor, then run jobs via the executor by specifying the worker function
and the argument (`executor.submit(fn, *args, **kwargs)`). We can send a lot of
jobs to the executor, but the thread pool may only run a limited number of jobs
concurrently. In the example above, we submit 50 jobs in one shot but run only
3 jobs at a time. Running the above code give you the following:

```text
2024-02-09 15:19:54.083284: worker 0 started
2024-02-09 15:19:54.083414: worker 1 started
2024-02-09 15:19:54.095935: worker 2 started
2024-02-09 15:19:57.032095: worker 0 return
2024-02-09 15:19:57.032184: worker 3 started
2024-02-09 15:19:57.039709: worker 2 return
2024-02-09 15:19:57.039772: worker 4 started
2024-02-09 15:19:57.046087: Memory usage 1039187968
2024-02-09 15:19:57.069717: worker 1 return
2024-02-09 15:19:57.082388: Memory usage 1054179328
2024-02-09 15:19:57.099680: worker 5 started
2024-02-09 15:19:57.130026: Memory usage 1074479104
2024-02-09 15:19:59.946041: worker 3 return
2024-02-09 15:19:59.946126: worker 6 started
2024-02-09 15:19:59.975450: worker 4 return
2024-02-09 15:19:59.975521: worker 7 started
2024-02-09 15:19:59.983047: Memory usage 2039955456
2024-02-09 15:19:59.994621: Memory usage 2044674048
...
2024-02-09 15:20:38.770484: worker 42 return
2024-02-09 15:20:38.770559: worker 45 started
2024-02-09 15:20:38.775674: Memory usage 14824030208
2024-02-09 15:20:38.779237: worker 43 return
2024-02-09 15:20:38.812374: worker 46 started
2024-02-09 15:20:38.824755: Memory usage 14840037376
2024-02-09 15:20:38.979729: worker 44 return
2024-02-09 15:20:38.979810: worker 47 started
2024-02-09 15:20:38.986076: Memory usage 14882963456
2024-02-09 15:20:41.812591: worker 45 return
2024-02-09 15:20:41.812663: worker 48 started
2024-02-09 15:20:41.818818: Memory usage 15393980416
2024-02-09 15:20:41.840824: worker 46 return
2024-02-09 15:20:41.853475: worker 49 started
2024-02-09 15:20:41.859693: Memory usage 15393980416
2024-02-09 15:20:41.977037: worker 47 return
2024-02-09 15:20:41.996684: Memory usage 15393980416
2024-02-09 15:20:43.886611: worker 48 return
2024-02-09 15:20:43.899240: Memory usage 15393980416
2024-02-09 15:20:43.917258: worker 49 return
2024-02-09 15:20:43.917344: Memory usage 15393980416
```

We can see that job 3 is not started until one of jobs 0, 1, 2 has finished.
Therefore, `concurrent.futures` maintains the workload within the specified
bound.

The worker in the above generates a list of 10 million floats, which assumes
32-bit float, would be 40MB, before adding the overhead that Python introduced
to the data structures. It is obvious that the memory usage is climbing up
while the jobs are executed by the thread pool. The reason for the "memory
leak" is at the `future` object: You can always get back the return value using
`future.result()`, and the `future` objects are those created by
`executor.submit()` --- they are in the `jobs` list created before the
for-loop.

Therefore, in case we can discard the `future` object that completed, we can
save some memory. It is easy to do in Python: `as_completed()` function needs
only an iterable. Hence we can use a generator instead of a list, like the
following:

```python
import concurrent.futures
import datetime
import math
import resource

def worker(n):
    print(f"{datetime.datetime.now()}: worker {n} started")
    x = []
    for i in range(10_000_000):  # just some slow operation
        x.append(math.sin(i))
    print(f"{datetime.datetime.now()}: worker {n} return")
    return x

def main():
    with concurrent.futures.ThreadPoolExecutor(max_workers=3) as executor:
        jobs = (executor.submit(worker, n) for n in range(50))
        for future in concurrent.futures.as_completed(jobs):
            _ = future.result()
            print(f"{datetime.datetime.now()}: Memory usage {resource.getrusage(resource.RUSAGE_SELF).ru_maxrss}")

if __name__ == "__main__":
    main()
```

In this case, the memory consumption stayed flat:

```text
2024-02-09 15:25:37.403959: worker 0 started
2024-02-09 15:25:37.404110: worker 1 started
2024-02-09 15:25:37.416765: worker 2 started
2024-02-09 15:25:40.267059: worker 0 return
2024-02-09 15:25:40.267160: worker 3 started
2024-02-09 15:25:40.279696: Memory usage 1044201472
2024-02-09 15:25:40.347980: worker 2 return
2024-02-09 15:25:40.348048: worker 4 started
2024-02-09 15:25:40.421628: worker 1 return
2024-02-09 15:25:40.421696: worker 5 started
2024-02-09 15:25:40.432426: Memory usage 1066827776
2024-02-09 15:25:40.542792: Memory usage 1066827776
2024-02-09 15:25:43.285008: worker 3 return
2024-02-09 15:25:43.285091: worker 6 started
2024-02-09 15:25:43.387106: Memory usage 1385267200
2024-02-09 15:25:43.447038: worker 4 return
2024-02-09 15:25:43.447112: worker 7 started
2024-02-09 15:25:43.521575: worker 5 return
2024-02-09 15:25:43.521615: Memory usage 1385267200
...
2024-02-09 15:26:23.008981: worker 42 return
2024-02-09 15:26:23.021109: worker 45 started
2024-02-09 15:26:23.110888: Memory usage 1512325120
2024-02-09 15:26:23.193712: worker 43 return
2024-02-09 15:26:23.193791: worker 46 started
2024-02-09 15:26:23.275376: Memory usage 1512325120
2024-02-09 15:26:23.379593: worker 44 return
2024-02-09 15:26:23.391285: worker 47 started
2024-02-09 15:26:23.466048: Memory usage 1512325120
2024-02-09 15:26:26.115932: worker 46 return
2024-02-09 15:26:26.116017: worker 48 started
2024-02-09 15:26:26.199723: Memory usage 1512325120
2024-02-09 15:26:26.210967: worker 45 return
2024-02-09 15:26:26.211051: worker 49 started
2024-02-09 15:26:26.297508: Memory usage 1512325120
2024-02-09 15:26:26.433841: worker 47 return
2024-02-09 15:26:26.519633: Memory usage 1512325120
2024-02-09 15:26:28.275848: worker 49 return
2024-02-09 15:26:28.345301: Memory usage 1512325120
2024-02-09 15:26:28.345372: worker 48 return
2024-02-09 15:26:28.399259: Memory usage 1512325120
```

I believe using a generator is the cleanest way to do. Otherwise, you can
always remove the item in the list using `jobs.remove(future)` at the end of
the for-loop, or something similar if you use set or dict instead of a list.

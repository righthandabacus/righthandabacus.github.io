#!/usr/bin/env python
r'''Test out multiprocessing behavior in Python

The following piece of code will print out this normally:

    got args -i 2
    2
    0 - 0
    1 - 2
    2 - 4
    4 - 8
    5 - 10
    3 - 6
    6 - 12
    7 - 14
    8 - 16
    9 - 18
    [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]

but in Windows environment (not Cygwin), this will be

    got args -i 2
    2
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    got args -i 2
    6 - 12
    got args -i 2
    0 - 0
    1 - 2
    2 - 4
    3 - 6
    4 - 8
    5 - 10
    7 - 14
    8 - 16
    9 - 18
'''

import argparse
import multiprocessing

def parsearg():
    parser = argparse.ArgumentParser(description="mp test")
    parser.add_argument("-i", "--input", default=0, type=int, help="some input")
    args = parser.parse_args()
    print("got args -i %s" % args.input)
    return args

def worker(n):
    print("%d - %d" % (n, n*args.input))
    return n*argcopy

def main():
    pool = multiprocessing.Pool(4)
    print(args.input)
    output = list(pool.map(worker, range(10)))
    print(output)

args = parsearg()
argcopy = 0
if __name__ == "__main__":
    argcopy = args.input
    main()

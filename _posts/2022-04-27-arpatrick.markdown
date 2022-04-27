---
title: "Arpa's trick explained"
layout: post
date: 2022-04-27
tags: code
---

Recently I encountered with the problem of range minimum query (RMQ) and there
are many ways to solve it, depending on the trade off between preprocessing
complexity and per-query runtime complexity. Block-paritioning and a sparse
table would be clever techniques. A hybrid of both would need some more code
but brings great performance.

For reference, this is how we can do a RMQ with different algorithms:

```python
import math
import random
import sys

def minargmin(arr, L, R):
    """Reusable function to find the min element in arr between indices L and R inclusive

    Returns the min value and the index
    """
    try:
        return min((v,i) for i,v in enumerate(arr[L:R+1], L))
    except ValueError:
        return (float("inf"), -1)

def naive(arr, queries):
    """Complexity: < O(1), O(n) >

    Search on array directly every time, O(1) preprocessing as none is done,
    O(n) time for each query"""
    ans = []
    for L,R in queries:
        ans.append(minargmin(arr, L, R))
    return ans

def lookup(arr, queries):
    """Complexity < O(n^2), O(1) >

    Build lookup table of O(n^2) size for all possible queries, then lookup
    immediately for answer
    """
    # preprocess a lookup table
    table, N = [], len(arr)
    for i in range(N):
        table.append([0] * N)
        idx, val = i, arr[i]
        table[i][i] = i
        for j in range(i+1, N):
            if arr[j] < val:
                idx, val = j, arr[j]
            table[i][j] = idx
    # find answer for each query
    ans = []
    for L,R in queries:
        idx = table[L][R]
        val = arr[idx]
        ans.append((val,idx))
    return ans

def blockpartition(arr, queries):
    """Complexity < O(n), O(sqrt(n)) >

    Build a block-partition table of minimum and combine the array search with table for answer.
    Need to consider the corner case that the query range covers no block
    """
    # build a block-partition table
    N = len(arr)
    sqrtN = int(math.sqrt(N))
    block = [minargmin(arr, i, i+sqrtN-1) for i in range(0, N, sqrtN)]
    def blockmin(begin, end):
        try:
            return min(block[begin:end])
        except ValueError:
            return float("inf"), -1
    # search on query
    ceildiv = lambda a,b: -(a // -b)  # ceiling division
    ans = []
    for L, R in queries:
        block_begin = ceildiv(L, sqrtN)
        block_end = R // sqrtN
        if block_begin < block_end:
            # we have one block at least!
            L_end = block_begin * sqrtN
            R_begin = block_end * sqrtN
            candidate = [
                minargmin(arr, L, L_end-1),
                blockmin(block_begin, block_end),
                minargmin(arr, R_begin, R)
            ]
            ans.append(min(candidate))
        else:
            # range too small to use any block, find minimum directly
            ans.append(minargmin(arr, L, R))
    return ans

def sparsetable(arr, queries):
    """Complexity < O(n log n), O(1) >

    Build a sparse table for lookup, table only supports range of 2^n but can
    start anywhere. Any range (L,R) can be a combination of two range at most.
    """
    # preprocess a sparse table
    N = len(arr)
    log2 = N.bit_length()
    table = [[0]*log2 for _ in range(N)]
    # fill the first column
    for i in range(N):
        table[i][0] = (arr[i], i)
    # fill the subsequent columns
    for j in range(1,log2):
        size = 1 << j
        halfsize = size >> 1
        for i in range(N):
            if i+size > N:
                break # unusable range
            table[i][j] = min(table[i][j-1], table[i+halfsize][j-1])
    # search on query
    ans = []
    for L, R in queries:
        length = R-L+1
        log2 = length.bit_length() - 1
        pow2 = 1 << log2
        R2 = R - pow2 + 1
        # (L,R) as union of (L,L+pow2-1) and (R2,R)
        ans.append(min(table[L][log2], table[R2][log2]))
    return ans

def test(N, Q):
    # Generate random array of N integers
    arr = [random.randint(0, 5000) for _ in range(N)]
    # Generate queries (L,R) for finding minimum in arr[L:R+1]
    queries = [sorted([random.randint(0,N-1), random.randint(0,N-1)]) for _ in range(Q)]
    print(arr)
    print(f"Queries: {queries}")
    print("Answers (naive):")
    ans_1 = naive(arr, queries)
    print(", ".join(f"arr[{idx}]={val}" for val,idx in ans_1))
    print("Answers (lookup):")
    ans_2 = lookup(arr, queries)
    print(", ".join(f"arr[{idx}]={val}" for val,idx in ans_2))
    assert ans_1 == ans_2
    print("Answers (block partition):")
    ans_3 = blockpartition(arr, queries)
    print(", ".join(f"arr[{idx}]={val}" for val,idx in ans_3))
    assert ans_1 == ans_3
    print("Answers (sparse table):")
    ans_4 = sparsetable(arr, queries)
    print(", ".join(f"arr[{idx}]={val}" for val,idx in ans_4))
    assert ans_1 == ans_4

if __name__ == "__main__":
    try:
        N = int(sys.argv[1])
    except:
        N = 100
    try:
        Q = int(sys.argv[2])
    except:
        Q = 30
    test(N, Q)
```

There is Fischer-Heun structure too, but it would be more code to implement and harder to explain.

Now here's the [Arpa's trick](https://codeforces.com/blog/entry/48994). First
we use a disjoint set union (DSU) for each element of a unsorted array `arr`. A
DSU is merely some tree structure stored as an array, zero-indexed with length
N, that each element is an integer of 0 to N such that it is a pointer to
another element in the array. An element may point to itself, which is the root
of a tree. Every element in the array is an arc in a digraph and no cycle should
be formed. It is called DSU because all elements in the same tree will be
considered as in the same set. The tree would not necessarily binary. In fact,
for optimal use, it should be a very fat tree with high fan-out at root and
depth 1.

What the DSU does here is to tell on each element, what is the next number
smaller than myself. It is used like this (original was in C++, converted the
code into Python):

```python
import numpy as np

arr = [3928,   53, 3093, 4657, 2209, 1823, 3613, 1018,  129,   32,  # 10
       3585,  903, 1538, 2462, 2092, 2093, 2230, 3209, 2800, 1689,  # 20
       4938, 3443,  386, 2725, 3363, 2351, 2696, 1641, 3931, 1073,  # 30
       3121, 2160, 1132, 2829, 2447, 2411,  381, 3528, 3309, 1496,  # 40
       4439, 4848, 4050, 2572,  158, 1076, 4222,  662, 3294, 4084,  # 50
       4312, 2752, 4420,  210, 4073, 1403,  800,  766, 2433, 1255,  # 60
       4260, 1391,  215, 1826,  488, 4379, 2582, 4896, 1245, 1328,  # 70
       1093, 2146, 1081,   48, 4918, 1037, 2653, 2201, 2080,  656,  # 80
       1124, 2575, 2037,  183, 2912, 2952, 2409, 1323, 1764, 2647,  # 90
       2035, 1950, 4997,  844, 2437, 2825, 4001, 3263, 3897, 2227]  # 100
# variables
N = len(arr)                 # size of input
dsu = list(range(N))         # DSU
stack = []                   # stack
# algorithm
for i in range(N):
    while stack and arr[stack[-1]] >= arr[i]:
        dsu[stack.pop()] = i
    stack.append(i)
# print answer
with np.printoptions(precision=2, linewidth=80, suppress=True, threshold=1000):
    print("Array")
    print(np.array(arr).reshape(10,-1))
    print("DSU")
    print(np.array(dsu).reshape(10,-1))
    print("Stack")
    print(stack)
```

The use of numpy above is just for aesthetic display. Initially the DSU makes
every node a tree of itself. The for loop moves the cursor `i` from index 0
till the end of the array `arr`. All indices will be pushed into the stack. But
when we are at an index, we check with the elements at the top of the stack and
rewrite the DSU to the current index if the array's element is greater. This
for loop essentially makes the DSU tells what is the next element that is
smaller than myself.

The above code runs to produce the following:

```
Array
[[3928   53 3093 4657 2209 1823 3613 1018  129   32]
 [3585  903 1538 2462 2092 2093 2230 3209 2800 1689]
 [4938 3443  386 2725 3363 2351 2696 1641 3931 1073]
 [3121 2160 1132 2829 2447 2411  381 3528 3309 1496]
 [4439 4848 4050 2572  158 1076 4222  662 3294 4084]
 [4312 2752 4420  210 4073 1403  800  766 2433 1255]
 [4260 1391  215 1826  488 4379 2582 4896 1245 1328]
 [1093 2146 1081   48 4918 1037 2653 2201 2080  656]
 [1124 2575 2037  183 2912 2952 2409 1323 1764 2647]
 [2035 1950 4997  844 2437 2825 4001 3263 3897 2227]]
DSU
[[ 1  9  4  4  5  7  7  8  9  9]
 [11 22 22 14 19 19 19 18 19 22]
 [21 22 36 25 25 27 27 29 29 36]
 [31 32 36 34 35 36 44 38 39 44]
 [42 42 43 44 73 47 47 53 51 51]
 [51 53 53 73 55 56 57 62 59 62]
 [61 62 73 64 73 66 68 68 70 70]
 [72 72 73 73 75 79 77 78 79 83]
 [83 82 83 83 86 86 87 93 93 90]
 [91 93 93 93 99 99 97 99 99 99]]
Stack
[9, 73, 83, 93, 99]
```

We can see that `dsu[0]==1` as the next element in `arr` that is smaller than
`arr[0]` is `arr[1]`. Similarly `dsu[8]==9`. But `dsu[9]==9` because itself is
the smallest element in the array. We also have `dsu[73]==73` because
`arr[73]==48`, the second smallest element in the array. We have `dsu[73] != 9`
because we always have `dsu[i] >= i` for it points to the *next* smaller
element. Upon finish, the stack has all the elements that has `dsu[i]==i`,
i.e., the root of trees. The DSU partitions nodes into index ranges 0 to
$$k_1$$, then $$(k_1+1)$$ to $$k_2$$, etc., with final partition
$$(k_{n-1}+1)$$ to $$k_n=N-1$$. Each partition is one set such that if we
navigate from index `i` according to the index pointed by `dsu[i]`, we
eventually reached and stay at $$k_m$$ which `i` was started in the same
partition. For example, let's pick `i==23`. We see this:

```
dsu[23] == 25
dsu[25] == 27
dsu[27] == 29
dsu[29] == 36
dsu[36] == 44
dsu[44] == 73
dsu[73] == 73
```

and we will never jump to below 23 or above 73. Now we can see that the line
`dsu[stack.pop()] = i` is to merge an existing set to a new root in DSU.

This is a powerful structure because for any range $$(L,R)$$ with $$R$$ at the
end of the array (or DSU), we just need to start with the index $$L$$ of the
DSU and navigate until we meet the root. Then it is the minimum of the range.
In fact, the DSU we built in the above loop is incremental. When the loop index
is at `i`, we processed the input array `arr` up to index `i` and the DSU works
as we described in the above paragraph for up to index `i` too.

Here is how we can use this DSU trick to do the RMQ. Consider the queries and
additional code below:

```python
queries = [[61, 78], [53, 74], [14, 26], [15, 96], [63, 80],
           [ 3, 62], [ 1, 49], [ 2, 57], [ 9, 33], [16, 83],
           [69, 80], [62, 84], [25, 58], [29, 75], [28, 55],
           [12, 53], [52, 97], [11, 96], [66, 98], [ 9, 27],
           [39, 86], [23, 88], [22, 96], [66, 68], [56, 83],
           [ 3,  7], [31, 44], [ 9, 88], [ 5, 60], [18, 71]]
Q = len(queries)             # number of queries
qu = [[] for _ in range(N)]  # 2D array to group queries by R-end
ans = [None]*Q               # to hold answers
# set up 2D array of queries
for i,(L,R) in enumerate(queries):
    qu[R].append(i)
# helper function on DSU
def root(k):
    if dsu[k] == k:
        return k
    dsu[k] = root(dsu[k])
    return dsu[k]
# algorithm
for i in range(N):
    while stack and arr[stack[-1]] >= arr[i]:
        dsu[stack.pop()] = i
    stack.append(i)
    for j in qu[i]:
        idx = root(queries[j][0])
        ans[j] = [idx, arr[idx]]
```

The function `root(k)` is to get the root of a tree that element `k` belongs in
the DSU. When it is invoked, it may mutate the DSU to make it more efficient by
bringing nodes to direct element of the root (a.k.a. *path compression*). Hence
after we called `root(23)`, we will see `dsu[23]==73` so as `dsu[25]==73` etc.,
due to the recursive nature of the function. This is allowed because the
ultimate use of DSU is to find the root, i.e., the minimum element found so far
starting from index `i`, rather than what is the next smaller element.

Range queries are presented as a pair $$(L,R)$$ which, in Python notation, is
to find `min(arr[L:R+1])`. We assumed $$L\le R$$. We used a kind of radix sort
to organize the queries by the $$R$$ term. And we answer those queries right
after we updated the DSU up to index $$R$$. We can't wait until the DSU is
fully processed for the entire array or otherwise the DSU will reflect the
minimum in ranges of $$(L,N-1)$$ instead.

The following is the complete code, with the model answer to verify the
correctness of the code:

```python
import numpy as np

# input array
arr = [3928,   53, 3093, 4657, 2209, 1823, 3613, 1018,  129,   32,  # 10
       3585,  903, 1538, 2462, 2092, 2093, 2230, 3209, 2800, 1689,  # 20
       4938, 3443,  386, 2725, 3363, 2351, 2696, 1641, 3931, 1073,  # 30
       3121, 2160, 1132, 2829, 2447, 2411,  381, 3528, 3309, 1496,  # 40
       4439, 4848, 4050, 2572,  158, 1076, 4222,  662, 3294, 4084,  # 50
       4312, 2752, 4420,  210, 4073, 1403,  800,  766, 2433, 1255,  # 60
       4260, 1391,  215, 1826,  488, 4379, 2582, 4896, 1245, 1328,  # 70
       1093, 2146, 1081,   48, 4918, 1037, 2653, 2201, 2080,  656,  # 80
       1124, 2575, 2037,  183, 2912, 2952, 2409, 1323, 1764, 2647,  # 90
       2035, 1950, 4997,  844, 2437, 2825, 4001, 3263, 3897, 2227]  # 100
# queries, (L,R) inclusive
queries = [[61, 78], [53, 74], [14, 26], [15, 96], [63, 80],
           [ 3, 62], [ 1, 49], [ 2, 57], [ 9, 33], [16, 83],
           [69, 80], [62, 84], [25, 58], [29, 75], [28, 55],
           [12, 53], [52, 97], [11, 96], [66, 98], [ 9, 27],
           [39, 86], [23, 88], [22, 96], [66, 68], [56, 83],
           [ 3,  7], [31, 44], [ 9, 88], [ 5, 60], [18, 71]]
# model answers, (idx, value)
model_answer = [[73,  48], [73,  48], [22, 386], [73,  48], [73,  48],
                [ 9,  32], [ 9,  32], [ 9,  32], [ 9,  32], [73,  48],
                [73,  48], [73,  48], [44, 158], [73,  48], [44, 158],
                [44, 158], [73,  48], [73,  48], [73,  48], [ 9,  32],
                [73,  48], [73,  48], [73,  48], [68,1245], [73,  48],
                [ 7,1018], [44, 158], [ 9,  32], [ 9,  32], [44, 158]]
# variables
N = len(arr)                 # size of input
Q = len(queries)             # number of queries
stack = []                   # stack
dsu = list(range(N))         # DSU
ans = [None]*Q
qu = [[] for _ in range(N)]  # 2D array of queries
# set up 2D array of queries
for i,(L,R) in enumerate(queries):
    qu[R].append(i)
# helper function on DSU
def root(k):
    if dsu[k] == k:
        return k
    dsu[k] = root(dsu[k])
    return dsu[k]
# algorithm
for i in range(N):
    while stack and arr[stack[-1]] >= arr[i]:
        dsu[stack.pop()] = i
    stack.append(i)
    for j in qu[i]:
        idx = root(queries[j][0])
        ans[j] = [idx, arr[idx]]
# print answer
with np.printoptions(precision=2, linewidth=80, suppress=True, threshold=1000):
    print("Array")
    print(np.array(arr).reshape(10,-1))
    print("Queries")
    print(np.array(queries))
    print("Answers")
    print(np.array(ans))
    print("(correct)" if ans == model_answer else "(incorrect)")
```

This code processed the input array sequentially. When the DSU is built, the
while-loop will run in the order of the size of the stack. Similarly at the
inner for-loop, the DSU root query will be done in the order of the depth of a
tree ($$O(\log n)$$). When the path compression is performed, a DSU should be
run in the order of $$O(\alpha(n))$$ for $$\alpha(n)$$ the inverse Ackermann
function, which is very close to $$O(1)$$. Hence the total overall time
complexity (so as space complexity) should be close to $$O(N+Q)$$.

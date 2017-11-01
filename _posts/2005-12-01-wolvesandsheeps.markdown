---
layout: post
title: "An Interesting Problem: Wolves and Sheeps"
date: 2005-12-01 12:42:47 +0800
lastedit: Thu, 19 Oct 2017 18:23:24 -0400
tags: math
---

In a field of geometry 5x5, put down 3 wolves and 5 sheeps. However, the wolves will kill the sheeps that they see in any diagonal or axial (horizonal and vertical) directions.

Actually, there is only one solution (ignoring transpose).

$$
\begin{array}{ccccc}
W & W & \_ & \_ & \_  \\
\_ & \_ & \_ & S & S  \\
\_ & \_ & \_ & \_ & S  \\
W & \_ & \_ & \_ & \_  \\
\_ & \_ & S & S & \_
\end{array}
$$

How to solve: (1) By enumeration.

```python
import itertools

GRIDSIZE = 5
WOLVES = 3
SHEEPS = 5

solution = [['_' for j in range(GRIDSIZE)] for i in range(GRIDSIZE)]
coordinates = list(itertools.product(range(GRIDSIZE), range(GRIDSIZE)))

def getTransforms(coords):
    yield [(x, GRIDSIZE-1-y) for x, y in coords] # mirror vertically, aka rotate transpose 90 degree
    yield [(y, GRIDSIZE-1-x) for x, y in coords] # rotate 90 degree, aka mirror of transpose vertically
    yield [(y, x) for x, y in coords] # transpose
    yield [(GRIDSIZE-1-x, y) for x, y in coords] # mirror horizontally, aka rotate transpose 270 degree
    yield [(GRIDSIZE-1-x, GRIDSIZE-1-y) for x, y in coords] # rotate 180 degree
    yield [(GRIDSIZE-1-y, x) for x, y in coords] # rotate 270 degree, aka rotate transpose horizontally
    yield [(GRIDSIZE-1-y, GRIDSIZE-1-x) for x, y in coords] # rotate transpose 180 degree
def sameSet(coords1, coords2):
    coords1 = sorted(coords1)
    return any(coords1 == sorted(coords) for coords in getTransforms(coords2))
def wolfSee(wolfcoord):
    wx, wy = wolfcoord
    coords = [
        (wx,y) for y in range(GRIDSIZE) # coordinates of same row
    ] + [
        (x,wy) for x in range(GRIDSIZE) # coordinates of same column
    ] + [
        (wx+i,wy+i) for i in range(-GRIDSIZE+1,GRIDSIZE) # coordinates of forward diagonal
    ] + [
        (wx+i,wy-i) for i in range(-GRIDSIZE+1,GRIDSIZE) # coordinates of backward diagonal
    ]
    return set((x,y) for x,y in coords)
if __name__=='__main__':
    checked = set([])
    for wolves in itertools.combinations(coordinates, WOLVES):
        if any(sameSet(wolves, old_wolves) for old_wolves in checked):
            continue # just a transformation, skip
        checked.add(wolves)
        goodcoordinates = set(coordinates)
        for wolf in wolves:
            goodcoordinates = goodcoordinates - wolfSee(wolf)
            if len(goodcoordinates) < SHEEPS:
                break # no way to place sheeps in this config
        else:
            print "Solution found:"
            print "  Wolves: %s" % list(wolves)
            print "  Sheeps: %s" % list(goodcoordinates)
```

Also, (2) there is an analytical method to solve (in MathProg):

```perl
param N, integer, >0; /* grid size */

param W, integer, >0; /* number of wolves */
param S, integer, >0; /* number of sheeps */

var WOLF {1..N, 1..N}, binary; /* WOLF[j,k]=1 iff a wolf is in coordinate (j,k) */
var SHEEP {1..N, 1..N}, binary; /* SHEEP[j,k]=1 iff a sheep is in coordinate (j,k) */

var hb{1..N} binary; /* constraint helper */
var vb{1..N} binary;
var db{1-N..N-1} binary;
var bb{2..N*2} binary;

/* Every wolf has a unique coordinate */
s.t. wolf_coordinate: sum{x in 1..N, y in 1..N} WOLF[x,y] = W;

/* Every sheep has a unique coordinate */
s.t. sheep_coordinate: sum{x in 1..N, y in 1..N} SHEEP[x,y] = S;

/* No wolf see any sheep horizontally */
s.t. horizontals{y in 1..N}: sum{x in 1..N} SHEEP[x,y] <= (S+W)*hb[y];
s.t. horizontalw{y in 1..N}: sum{x in 1..N} WOLF[x,y] <= (S+W)*(1-hb[y]);
    /* On every horizontal scan, we either find sheeps or wolves or neither but not both */

/* No wolf see any sheep vertically */
s.t. verticals{x in 1..N}: sum{y in 1..N} SHEEP[x,y] <= (S+W)*vb[x];
s.t. verticalw{x in 1..N}: sum{y in 1..N} WOLF[x,y] <= (S+W)*(1-vb[x]);
    /* On every vertical scan, we either find sheeps or wolves or neither but not both */

/* No wolf see any sheep forward diagonally */
s.t. diagonals{i in 1-N..N-1}: sum{x in 1..N, y in 1..N: x-y=i} SHEEP[x,y] <= (S+W)*db[i];
s.t. diagonalw{i in 1-N..N-1}: sum{x in 1..N, y in 1..N: x-y=i} WOLF[x,y] <= (S+W)*(1-db[i]);
    /* On every diagonal scan, we either find sheeps or wolves or neither but not both */

/* No wolf see any sheep backward diagonally */
s.t. backwards{i in 2..N*2}: sum{x in 1..N, y in 1..N: x+y=i} SHEEP[x,y] <= (S+W)*bb[i];
s.t. backwardw{i in 2..N*2}: sum{x in 1..N, y in 1..N: x+y=i} WOLF[x,y] <= (S+W)*(1-bb[i]);
    /* On every diagonal scan, we either find sheeps or wolves or neither but not both */

solve;

printf "Wolves: ";
for {x in 1..N} {
    for {y in 1..N: WOLF[x,y] == 1} {
        printf "(%d,%d) ", x, y;
    }
}
printf "\n";
printf "Sheeps: ";
for {x in 1..N} {
    for {y in 1..N: SHEEP[x,y] == 1} {
        printf "(%d,%d) ", x, y;
    }
}
printf "\n";

data;

param N := 5;
param W := 3;
param S := 5;

end;
```

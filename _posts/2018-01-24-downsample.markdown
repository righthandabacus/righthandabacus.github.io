---
layout: post
title: Downsampling algorithms
date: Wed, 24 Jan 2018 14:55:45 -0500
lastedit: Fri, 26 Jan 2018 13:22:32 -0500
tags: code
---
This is from a master thesis that try to downsample a line chart (time
series) but retain all visual characteristics. What I know about downsampling is
to assume the input is a signal and pass it through a low-pass filter, as we
have learned in the undergraduate signal & system course. But we can't guarantee
the visual characteristics will not be distorted (Gibbs effect!), especially we
can't sure the maxima and minima are retained.

The intuitive way to do downsampling is to split the $$x$$ domain of the line
chart into some equal segments, and pick one point from each segment. Each
segment is named a *bucket* in the thesis. We can pick the points by taking the
mean, mode, median, min, or max of the $$y$$ values in each bucket.  Strictly
speaking, data points after downsample should be subset of input. If this rule
applies, we cannot use mean as the method of selection (nor distorting the $$x$$
value of the selected data point). But each of these simple selection rule will
have certain weakness, for example,

- cannot retain local peak and troughs in downsample
- cannot keep both peak and troughs from the same bucket in the downsample
- cannot retain fluctuation pattern in downsample

Such simple intuitive way of downsample (using mode or median as selection
rule) is named "mode-median-bucket" in the thesis.

An improved method is named "longest line bucket" (LLB). Similar bucketing is
applied but with the leftmost and rightmost data point in their own bucket. And
an edge is connecting every point pairs across the bucket boundary, for
example, if bucket $$i$$ and bucket $$i+1$$ has $$N_i$$ and $$N_{i+1}$$ points
respectively, there will be $$N_iN_{i+1}$$ edges created. Such edges are directed
from left to right, hence the whole thing becomes a DAG. Each edge carries a
weight, defined as the Euclidean distance between the points. The downsampling
is to find the path from first to last data point on the DAG that maximize the
sum of edge weights, e.g. using Dijkstra's algorithm. The LLB algorithm can
retain most visual characteristics by a high likelihood of including local
maxima and minima. However, finding the path with max weight involves dynamic
algorithm and this could be slow.

<svg width="300" height="150">
   <g transform="translate(0,-257.31248)">
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="M 35.647399,348.86347 8.3312922,339.30284" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="M 8.3312922,339.30284 54.76867,320.18157" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1;fill:none"
       d="M 119.87204,339.30284 54.76867,320.18157 134.8959,307.43405" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1;fill:none"
       d="M 119.87204,339.30284 35.647399,348.86347 134.8959,307.43405" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="M 263.28159,341.57919 289.68713,319.7263" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 228.68119,314.26309 61.00594,5.46321" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 164.03308,369.80583 99.24851,-28.22664" />
    <path
       style="stroke:#e97f8b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 193.62553,350.68456 35.05566,-36.42147" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 119.87204,339.30284 44.16104,30.50299" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 119.87204,339.30284 73.75349,11.38172" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 134.8959,307.43405 29.13718,62.37178" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 134.8959,307.43405 58.72963,43.25051" />
    <path
       style="stroke:#8374de;stroke-width:2;fill:none"
       d="M 289.68713,319.7263 250.53407,287.40224 205.46249,388.01655 134.8959,307.43405 71.158326,387.10602 8.3312922,339.30284 v 0" />
    <path
       style="stroke:#555555;stroke-width:1;stroke-dasharray:1,2"
       d="M 214.87465,259.70622 V 406.5032" />
    <path
       style="stroke:#555555;stroke-width:1;stroke-dasharray:1,2"
       d="m 20.754866,259.7062 v 146.797" />
    <path
       style="stroke:#555555;stroke-width:1;stroke-dasharray:1,2"
       d="M 150.16804,259.70617 V 406.50325" />
    <path
       style="stroke:#555555;stroke-width:1;stroke-dasharray:1,2"
       d="m 85.461462,259.7062 v 146.797" />
    <path
       style="stroke:#555555;stroke-width:1;stroke-dasharray:1,2"
       d="m 279.58126,259.7062 v 146.797" />
    <path
       style="stroke:#e97f8b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 205.46249,388.01655 57.8191,-46.43736" />
    <path
       style="stroke:#e97f8b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 205.46249,388.01655 23.2187,-73.75346" />
    <path
       style="stroke:#e0808b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 164.03308,369.80583 64.64811,-55.54274" />
    <path
       style="stroke:#e97f8b;stroke-width:1;stroke-dasharray:3, 1"
       d="m 193.62553,350.68456 69.65606,-9.10537" />
    <circle cx="7.4863319" cy="339.8812" fill="#000000" r="2" />
    <circle cx="70.541542" cy="387.01645" fill="#000000" r="2" />
    <circle cx="55.089241" cy="320.0564" fill="#000000" r="2" />
    <circle cx="35.773842" cy="349.67331" fill="#000000" r="2" />
    <circle cx="134.92621" cy="308.46713" fill="#000000" r="2" />
    <circle cx="118.1862" cy="339.3718" fill="#000000" r="2" />
    <circle cx="163.5166" cy="369.74304" fill="#000000" r="2" />
    <circle cx="193.64734" cy="350.66272" fill="#000000" r="2" />
    <circle cx="228.9278" cy="314.90561" fill="#000000" r="2" />
    <circle cx="262.40778" cy="341.94724" fill="#000000" r="2" />
    <circle cx="205.74933" cy="388.30414" fill="#000000" r="2" />
    <circle cx="289.44934" cy="320.05643" fill="#000000" r="2" />
    <circle cx="251.5309" cy="287.86404" fill="#000000" r="2" />
  </g>
</svg>

A similar problem from cartography is to *simplify* a polyline on map. The
thesis mentioned two algorithms to do so:

1. Douglas-Peucker algorithm (1973)
  - start: draw line segments from first to last point
  - select the point that is furtherest away, and the distance is above certain
    threshold, from the existing line segments
  - revise the line segment(s) to include that point
  - call recursively until all points not include in the line segment has
    distance below threshold
2. Visvalingam-Whyatt algorithm (1993)
  - define effective area of a point as the area of triangle constructed by
    that point and its two adjacent points (considered both distance and the
    angle between points)
  - assign each point the weight of its effective area
  - recursively exclude the points of least weight, then recalculate the
    effective area and reassign the weight

We may apply the Visvalingam-Whyatt algorithm to downsample a line chart but we
have to restrict it from skipping too many consecutive data points, or
otherwise we are very likely to replace a region of minor fluctuation with a
line segment. Bucketing is the solution to guarantee at least one data point is
kept in the downsample output.

Assigning effective area to each point as the weight and select the point with
greatest weight in each bucket into the downsample will produce visually
comparable result to LLB method above, without the Dijkstra's algorithm. This
is named the largest-triangle-one-bucket (LTOB) method. Keeping only the point
with largest weight per bucket will implicitly exclude all points that are
colinear with adjacent points.

Improving the LTOB method is to change the effective area definition to be not
the adjacent points of a point, but any point in adjacent bucket. Assume we
have $$N_{i-1}$$, $$N$$, $$N_{i+1}$$ points respectively in adjacent buckets $$i-1$$,
$$i$$, $$i+1$$. For each point in bucket $$i$$, we have $$N_{i-1}N_{i+1}$$ possible
triangle formation and we assign the maximum of them as the weight to such
point. However, assuming we are scanning the line chart from left to right, and
we have the first and last data point in its own bucket, we always have a
point selected in bucket $$i-1$$ when we are working on bucket $$i$$. Therefore, we
only need to find the max effective area among $$N_{i+1}$$ options and the
complexity for selecting a point in each bucket is $$O(N^2)$$ only. A way to
further improve this to $$O(N)$$ complexity is to use a fixed imaginary point in
bucket $$i+1$$ for the effective area calculation for points in bucket $$i$$.
The imaginary point is defined as the mean of all points in that bucket. Since
we are working on three buckets each time, it is largest-triangle-three-bucket
(LTTB) method.

Above assumed we divide the buckets into equal size of interval on $$x$$-axis.
Dynamic bucketing can be applied: We start with equal interval bucketing. Next,
consider the sum of squared error (SSE) of the $$y$$-value of all points in a
bucket to their mean, we split the bucket into two if this is too large and
merge with neighboring bucket if this is too low. By running certain iterations
on this splitting/merging of buckets, the bucketing should be more frequent in
region of high fluctuations. Number of iterations to run is suggested as
$$10 \times \frac{\textrm{size of input sample}}{\textrm{size of downsample}}$$
in the thesis.

Reference
=========
- William Ferguson, [Sampling large datasets in d3fc](http://blog.scottlogic.com/2015/11/16/sampling-large-data-in-d3fc.html)
- Sveinn Steinarsson (2013) [Downsampling time series for visual representation](http://skemman.is/stream/get/1946/15343/37285/3/SS_MSthesis.pdf). U. Iceland MS Thesis[^1]
    - Javascript code: <https://github.com/sveinn-steinarsson/flot-downsample>
- Implementation of [largest-triangle-three-buckets](https://github.com/devoxi/lttb-py) in Python

[^1]: [Ben Lau](https://github.com/benlau) point this out to me.

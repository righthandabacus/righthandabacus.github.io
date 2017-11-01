---
layout: post
title: "Solving Steiner Tree Problem as a MILP"
date: 2012-04-10 11:47:38 -0400
tags: math
---

Consider a network $G(V,E)$, with $W_0 \subset V$ is a set of focus vertices.
Let's call $s\in W_0$ the multicast sender and the rest $W = W_0 \backslash
\{s\}$ the multicast receivers. Given the edges of unit weight, how to find the
multicast tree that connects $s$ to $W$ with minimum total edge weight? This is
a generalized minimal spanning tree problem, or a Steiner tree problem in graph.

I made two attempts to solve it as a MILP, the second one is successful. First one,
in MathProg modelling language for GLPK, is as follows:

```perl
############
# Parameters: To be assigned by the data section
param V, integer;	# Total number of nodes

# The set of nodes and edges are those that connecting any two nodes
set nodes := 1..V;
set edges within nodes cross nodes;
set clients within nodes;
param sender in nodes;

############
# Defining the network topology
param e{i in nodes,j in nodes} binary :=
	if (i,j) in edges or (j,i) in edges then 1 else 0;
			# The incident matrix of (directed) edges in the graph
param c{i in nodes,j in nodes: e[i,j] = 1} default 1;
			# Edge capacities

############
# Sanity checks
param Reachable{i in nodes} binary :=
    if i = 1 then 1 else ( if exists{j in nodes: Reachable[j] = 1} e[i,j] then 1 else 0 );
check: sum{i in nodes} Reachable[i] = V;
	# check the network is connected: node 1 can reach every other node

############
# Variables
var t{i in nodes,j in nodes} binary;      # multicast tree
var r{i in nodes} binary;                 # reachability from sender
var rt{i in nodes, j in nodes} integer;   # r[i]*t[i,j]

############
# Constraints
s.t. tree_over_edge {i in nodes, j in nodes}: t[i,j] <= e[i,j];
	# multicast tree is a subset of the specified edges
s.t. from_source: sum{i in nodes} t[sender,i] >= 1;
s.t. to_source: sum{i in nodes} t[i,sender] = 0;
        # multicast tree shall root at the sender
s.t. to_node {i in nodes}: sum{j in nodes} t[j,i] <= 1;
        # tree shall have at most one incident edge to any node
s.t. to_client {i in clients}: sum{j in nodes} t[j,i] = 1;
        # tree shall cover all the clients
s.t. from_node {i in nodes, j in nodes: i != sender}: sum{k in nodes} t[k,i] >= t[i,j];
        # except the sender, no emerging edge if no incident edge
s.t. sender_reachable: r[sender] = 1;
        # by definition, sender is reachable
s.t. client_reachable: sum{i in clients} r[i] = card(clients);
	# multicast tree shall reach all the receivers
s.t. def_rt1 {i in nodes, j in nodes}: rt[i,j] <= t[i,j];
s.t. def_rt2 {i in nodes, j in nodes}: rt[i,j] <= r[i];
s.t. def_rt3 {i in nodes, j in nodes}: rt[i,j] >= t[i,j] + r[i] - 1;
s.t. def_rt4 {i in nodes, j in nodes}: rt[i,j] >= 0;
        # linearization: defining rt[i,j] = r[i] * t[i,j]
s.t. reachability {i in nodes: i != sender}: r[i] = sum{j in nodes} rt[j,i];
	# if i has an incident edge (j,i), r[i] = r[j]

############
# Objective
minimize totallinks: sum{i in nodes, j in nodes} t[i,j];
        # minimize the total number links used in the multicast tree
solve;

############
# Print result
#
printf "Total links used: %d\n", totallinks;
printf "Multicast Tree:\n";
for {i in nodes, j in nodes: t[i,j]=1} {
	printf "(%d,%d) ", i, j;
}
printf "\n";

############
# Data section: The network
data;
param V := 24;	# Total number of vertices
set edges :=    # The edges specified as undirected (unordered pairs)
	(1,2) (1,6) (2,3) (2,6) (3,4) (3,5) (3,7) (4,7) (4,5) (5,8) (6,7) (6,9)
	(6,11) (7,8) (7,9) (8,10) (9,10) (9,11) (9,12) (10,13) (10,14) (11,12)
	(11,15) (11,19) (12,13) (12,16) (13,14) (13,17) (14,18) (15,16) (15,20)
	(16,17) (16,21) (16,22) (17,18) (17,22) (17,23) (18,24) (19,20) (20,21)
	(21,22) (22,23) (23,24);
param sender := 1;  # Multicast sender
set clients :=      # Multicast receivers
        4 5 8 24 23 9 11 16 21 22 20 19;
end;
```

This method defines a tree as one that comes out from the sender and reaches every clients. However, the output of GLPK turns out to be disconnected and with loops. The reachability variable `r` does not work as expected. In fact, there is no good way to enforce loop-free: One way to check loop-free condition is to enumerate all subset of nodes to verify the number of nodes is at least one more than the number of edges in the subgraph:

```perl
set S := 0 .. (2**V-1);       # enumerate all subset of nodes
set nodeset {i in S} := {n in nodes: (i div 2**(n-1)) mode 2 == 1};

s.t. no_cycle {s in S}:
    sum{i in nodeset[s], j in nodeset[s]: e[i,j]=1} t[i,j] <= card(nodeset[s]) - 1;
        # trees shall be free of any cycles: for any subset of nodes, link count <= node count - 1
```

However, this introduces an expoential number of constraints, hence practically impossible.

The second attempt is to consider trees as just a collection of paths. For the paths with a common source vertex, and we minimize the number of links, the tree *shall be* free of loops:

```perl
############
# Parameters: To be assigned by the data section
param V, integer;	# Total number of nodes

# The set of nodes and edges are those that connecting any two nodes
set nodes := 1..V;
set edges within nodes cross nodes;
set clients within nodes;
param sender in nodes;

############
# Defining the network topology
param e{i in nodes,j in nodes} binary :=
	if (i,j) in edges or (j,i) in edges then 1 else 0;
			# The incident matrix of (directed) edges in the graph
param c{i in nodes,j in nodes: e[i,j] = 1} default 1;
			# Edge capacities

############
# Sanity checks
param Reachable{i in nodes} binary :=
    if i = 1 then 1 else ( if exists{j in nodes: Reachable[j] = 1} e[i,j] then 1 else 0 );
check: sum{i in nodes} Reachable[i] = V;
	# check the network is connected: node 1 can reach every other node

############
# Variables
var t{i in nodes,j in nodes} binary;               # multicast tree
var p{i in nodes,j in nodes,k in clients} binary;  # path connecting sender to client k

############
# Constraints
s.t. tree_from_path {i in nodes,j in nodes, k in clients}: t[i,j] >= p[i,j,k];
        # tree is the union of all the paths
s.t. path_on_edge {i in nodes, j in nodes, k in clients}: p[i,j,k] <= e[i,j];
	# paths are edges
s.t. source1 {k in clients}: sum{j in nodes} p[sender,j,k] = 1;
s.t. source2 {k in clients}: sum{i in nodes} p[i,sender,k] = 0;
	# paths originated from sender
s.t. sink1 {k in clients}: sum{j in nodes} p[k,j,k] = 0;
s.t. sink2 {k in clients}: sum{i in nodes} p[i,k,k] = 1;
	# paths destinated at client k
s.t. connect {j in nodes, k in clients: j != sender and j != k}:
    sum{i in nodes} p[i,j,k] = sum{i in nodes} p[j,i,k];
	# paths are connected on all other intermediate nodes

############
# Objective
minimize totallinks: sum{i in nodes, j in nodes} t[i,j];
        # minimize the total number links used in the multicast tree
solve;

############
# Print result
printf "Total links used: %d\n", totallinks;
printf "Multicast Tree:\n";
for {i in nodes, j in nodes: t[i,j]=1} {
	printf "(%d,%d) ", i, j;
}
printf "\n";

############
# Data section: The network
data;
param V := 24;	# Total number of vertices
set edges :=    # The edges specified as undirected (unordered pairs)
	(1,2) (1,6) (2,3) (2,6) (3,4) (3,5) (3,7) (4,7) (4,5) (5,8) (6,7) (6,9)
	(6,11) (7,8) (7,9) (8,10) (9,10) (9,11) (9,12) (10,13) (10,14) (11,12)
	(11,15) (11,19) (12,13) (12,16) (13,14) (13,17) (14,18) (15,16) (15,20)
	(16,17) (16,21) (16,22) (17,18) (17,22) (17,23) (18,24) (19,20) (20,21)
	(21,22) (22,23) (23,24);
param sender := 1;  # Multicast sender
set clients :=      # Multicast receivers
        4 5 8 24 23 9 11 16 21 22 20 19;
end;
```

This one successfully find the tree. In this example, 14 edges for the sender to reach 12 vertices. In a more general case (e.g. with different link weights), we may need to introduce the constraints that, no more than one incident edge on any node in the tree, and no edge on the tree shall be used in both direction:

```perl
s.t. to_node {i in nodes}: sum{j in nodes} t[j,i] <= 1;
        # tree shall have at most one incident edge to any node
s.t. direction {i in nodes, j in nodes}: t[i,j] + t[j,i] <= 1;
        # no edge shall be used in both direction in a tree
```

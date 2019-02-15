---
layout: post
title: "DeCandia et al (2007) Dynamo"
date: Thu, Feb 14, 2019 11:10 PM
category: paper
bibtex:
  type: inproceedings
  title: "Dynamo: Amazon's Highly Available Key-value Store"
  author: "Giuseppe DeCandia and Deniz Hastorun and Madan Jampani and Guna vardhan Kakulapati and Avinash Lakshman and Alex Pilchin and Swam inathan Sivasubramanian and Peter Vosshall and Werner Vogels"
  booktitle: "Proc. SOSP'07"
  page: 205--220
  address: "Stevenson, WA"
  month: October
  year: 2007
---

Objective of dynamo

- dealing with failures in an infrastructure comprised of millions of components
- data store that only need primary-key access
  - use key value store, as RDBMS will limit scale and availability

Techniques used

- data partitioned and replicated using consistent hashing
- object versioning to maintain consistency
- quorum-like technique and synchronization protocol to maintain consistency among replicas

**§2.1 System Assumption & Requirement**

- Query: Key → Blob (blob size < 1 MB)
- Give up ACID because of poor availability
  - no isolation as in ACID, allow only single key update
- Use 99.9th percentile of latency as efficiency metric and SLA definition
- Assume to use in internal, non-hostile environment

**§2.3 Design considerations**

Synchronous replica coordination = Strong consistency = Trade-off availability under failure scenario
- dynamo maintain only eventual consistency

Update conflict resolution at write vs at read
- traditional: at write time, but failing write availability if cannot reach all or most of the replica
- dynamo: conflict resolution at read

Conflict resolution scheme
- at data store: we can only have last write wins
- at application: allows app-specific merge algorithm

Cluster design:
- incremental scale out
- symmetry: all nodes have same responsibilities
- decentralization

**§3 Related work**

- P2P network with enough routing information at each peer can employ O(1) routing
- Design to be robust against network partition
- Do not focus on data integrity and security, but target for always writable

**§4.1 System interface**

`get(key)`: return single object or multiple objects of conflicting versions

`put(key, object)`: write replica to disk, with metadata like versions

**§4.2 Partitioning**

Partition of data over many storage nodes: use consistent hashing
- ring of hash values, and "virtual nodes" placed on the ring
- virtual nodes assigned a random value in the ring
- coordinator node responsible for the range between it and its predecessor
- each range is replicated to next $$N-1$$ nodes in the ring in clockwise direction
- each instance of dynamo host has multiple virtual nodes

Physical node = multiple virtual nodes for uniform load distribution even if the hash value is not uniform

Always $$N$$ replicas for each data object: for high availability

**§4.3 Replication**

Every key has $$N$$ replicas in the system. Coordinator node replicates a key
to $$N-1$$ clockwise successor nodes in the ring

**§4.4 Data versioning**

- for eventual consistency
- example: shopping cart is data, add to cart or remove from cart are put requests that create new version of the data
- divergent versions are reconciled later
  - syntactic reconciliation: system knows one version is older than another
  - semantic reconciliation: only client knows how to collapse multiple branches of data evolution into one

**§4.5 Get and put operations**

Quorum system:
- get or put requests are sent to top $$N$$ reachable nodes in the preference
  list and wait for $$W$$ or $$R$$ responses before returns to client
- successful read need $$R$$ nodes' response
- successful write need $$W$$ nodes' response
- $$R+W > N$$, typically $$R=W=2$$, $$N=3$$

**§4.6 Hinted hand-off**

Always maintain $$N$$ replicas to $$N$$ healthy nodes from preference list of a key

Example: key of an object should go to node A, and replica to B and C. But if
node A is unreachable, a replica will sent to D as well. All replicas (esp. the
one to D) will hint on their metadata that A should be the coordinator. So when
A restored, D will forward the object back to A.

**§4.7 Replica synchronization**

Merkle tree: Hash tree such that
- leaves are hashes of values of individual keys
- parent nodes = hashes of their children

Dynamo node maintain a separate Merkle tree for each key range and use tree
traversal to determine if there are any difference and perform synchronization

**§4.8.1 Ring membership**

*Explicit* mechanism to add or remove node from a dynamo ring.

Persistent store is used to hold membership. Gossip-based protocol to propagate
membership changes and maintain an eventual consistent view of membership.

- each node randomly connect another node in the range and synchronize their membership change histories
- partitioning and placement also through gossip protocols

**§4.8.2 External discovery**

- some nodes are "seed nodes" that all node to join a ring will know them and sync membership with them
- seed nodes are discovered by external mechanism such as configuration service

**§4.8.3 Failure detection**

- failure detection to avoid attempt to communicate with unreachable peers
  - local notation of failure: no response to messages sent
  - decentralized failure detection: reported over gossip protocol

Only temporary failure should be detected and learned from gossip protocol.
Permanent node add/removal should be done using explicit mechanism to avoid
confusion.

**§4.9 Add/remove storage nodes**

New node into system:

- assigned with a few new tokens randomly scattered on the ring
- key range changes, some nodes have to transfer the range to other nodes
- transfer offer and confirmation messages between nodes
  - confirmation avoids duplicated transfers

**§5 Implementation**

Each dynamo node has the following function:
- request coordination (handles get and put)
- membership & failure detection
- local persistence engine (non-volatile storage of key-value data)

Local persistence: use MySQL, Berkeley DB, etc.

Request coordination: Read write requests
- event-driven messaging substrate used
- communication = Java NIO channels
- example of read
  1. send read request to nodes
  2. wait for min number of required responses
  3. if too few responses within time bound, fail the request
  4. gather all responses and determine which one to return
  5. syntactic reconciliation if needed and generate write request
- coordinator do "read repair" if stale versions are found
- optimize for read-after-write: write coordinator is chosen to be the one
  replied fastest to the previous read

**§6 Performance**

Write latency is longer than read latency because of disk access for persistent
storage. The 99.9th percentile latency is around 200ms.

**§6.2 Uniform load distribution**

Different partitioning scheme of key ranges in ring evaluated (see fig 3 of the paper):

Strategy 1: $$T$$ random tokens per node, partition by token value
- when nodes joins/leaves, ranges changes
- space at each node to maintain membership is linear to the number of nodes in system
- node join = steal key range from other nodes
  - nodes to scan persistent store to hand off data
  - slow bootstrapping, recalculation of Merkle tree on many nodes

Strategy 2: $$T$$ random tkoens per node, equal sized partition
- hash space divided to $$Q$$ equal sized partitions
- Each node assigned $$T$$ tokens, $$S$$ nodes in the system
- $$Q \gg N$$ and $$Q \gg ST$$, $$Q$$ is a fixed constant

Strategy 3: $$Q/S$$ tokens per node, equal sized partition
- each node has $$Q/S$$ tokens
- node leaves the system = redistribute its tokens to remaining nodes
- node joins the system = steal tokens from other nodes to maintain $$Q/S$$ tokens per node

Strategy 3 has best load balancing efficiency
- fast to bootstrap
- easy to archive

Because nodes now handles the $$Q$$ partitions separately, we can assume and
renounce responsibility of a partition like adding and removing a file. No need
to recalculate the Merkle tree.

**§6.3 Divergent versions**

Divergent versions due to (1) node failure and (2) concurrent write

**§6.4 Coordination**

Request coordination component: client vs server

Server: client send request to any node on the ring
- send by load balancer
- any dynamo node can be a coordinator for read and any node on the key's
  preference list can be coordinator for write

Client: At application library to implement the request coordination logic
- client needs to sync membership state with a random dynamo node periodically
- better performance in latency
- efficiency depends on how fresh the membership information is at the client

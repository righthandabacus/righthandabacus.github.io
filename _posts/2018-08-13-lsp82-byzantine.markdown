---
layout: post
title: "Lamport et al (1982) The Byzantine Generals Problem"
date: 2018-08-13 12:00:00 -0400
category: paper
bibtex:
  type: article
  title: "The Byzantine generals problem"
  author: "Leslie Lamport and Robert Shostak and Marshall Pease"
  journal: "ACM Transactions on Programming Languages and Systems"
  volume: 4
  number: 3
  month: July
  pages: 382--401
  year: 1982
---

About fault tolerance in a distributed system. Using the metaphor of a number of
Byzantine generals have to come to a consensus of action, while the
communication may not be accurate. For example, the sender of a message may be a
traitor such that the message may be different to different receivers; or the
messenger may be a traitor such that the message it delivers may be forged or
discarded.

The consensus is decided distributedly using the same algorithm, e.g., simple
majority vote of the same set of input available to all participants. Thus the
problem reduce to how we can make the same input available to all Byzantine
generals.

The paper first investigated the scenario of "oral messages", i.e., messages are
not signed and easy to forge. We can simplify the problem into the following:
Assume we have a general that issue a command of action, either attack or
retreat, to all its lieutenants. The lieutenants have to come to the same
consensus of whether to attack or retreat. The general, or any of the
lieutenants, may be a traitor. If the general is a traitor, he will send
different action to different lieutenants; and if a lieutenant is a traitor, he
may report a different action than the one he received from the general to
another lieutenant. If the total number of participant (general and lieutenants)
is $$n$$, and the number of traitors in the group is $$m$$, the paper proved that
there is no way the loyal generals can have a consensus if $$n\le 3m$$. The
prove is first look for $$n=3, m=1$$ and there are 1 general with two
lieutenants. We find that we cannot distinguish the case between the general is
the traitor and one of the lieutenant is a traitor, given the communication of
message is first the general send to two lieutenants and then the two
lieutenants report to each other the message he received from the general. Then,
assume we can put the traitors into a group and loyal lieutenants into another
groups, we can create $$n=3$$ groups and perform induction on $$m$$.

The paper proposed an oral message algorithm, $$OM(m)$$ to reach consensus of
action in case of $$n>3m$$. The algorithm is as follows:

1. $$OM(0)$$ is to let the general distribute its action to all lieutenants and
   the lieutenants exchanged message; then the action is decided on simple
   majority vote on the perceived message sent by the general. There is a default
   action (e.g., retreat) in case of tied vote.
2. $$OM(m)$$ is to let the general 1 distribute its action $$v$$ to all
   $$n-1$$ lieutenants $$2,3,\cdots,n$$; and each lieutenant $$i$$ will forward
   the action it received $$v_i$$ to all other $$n-2$$ lieutenants by pretending
   he is the general and using algorithm $$OM(m-1)$$;

Using an example can make this algorithm easier to understand. Assume we have 2
traitors and 5 loyal generals. If the loyal generals want to obtain consensus of
the message from a loyal general (general 1), after mutually exchanged messages
received from general 1, each will have 5 identical copy of the message (one
from general 1 directly and 4 other from exchanges) and 2 copies from traitors
which may be different from the true message. Under majority voting, the
original message can be recovered. The more complicated case would be the
message from a traitor (general 7): He send out attack (1) message to generals
1 to 3 and retreat (0) message to generals 4 to 6.

Let us define $$v$$ to be the message in concern (the message from general 7)
and $$v_p$$ be the message as traversed in path $$p$$. The first step of
$$OM(2)$$ produces the following:

$$\begin{align}
v_{71} &= 1 \\
v_{72} &= 1 \\
v_{73} &= 1 \\
v_{74} &= 0 \\
v_{75} &= 0 \\
v_{76} &= 0
\end{align}$$

Then each of the node will exchange the message they received, which the loyal
lieutenants will forward the message faithfully but the traitorous lieutenant
may alter the message, e.g.,

$$\begin{align}
v_{71} &=1 & v_{712} &=1 & v_{713} &=1 & v_{714} &=1 & v_{715} &=1 & v_{716} &=1 \\
v_{721} &=1 & v_{72} &=1 & v_{723} &=1 & v_{724} &=1 & v_{725} &=1 & v_{726} &=1 \\
v_{731} &=1 & v_{732} &=1 & v_{73} &=1 & v_{734} &=1 & v_{735} &=1 & v_{736} &=1 \\
v_{741} &=0 & v_{742} &=0 & v_{743} &=0 & v_{74} &=0 & v_{745} &=0 & v_{746} &=0 \\
v_{751} &=0 & v_{752} &=0 & v_{753} &=0 & v_{754} &=0 & v_{75} &=0 & v_{756} &=0 \\
v_{761} &=0 & v_{762} &=1 & v_{763} &=0 & v_{764} &=1 & v_{765} &=0 & v_{76} &=0 \\
\end{align}$$

Now consider node 1. It received $$v_{761}=0$$ but it is not sufficient for him
to confirm $$v_{76}=0$$. Therefore, he perform $$OM(1)$$ with this, pretending
node 6 is the general in this round sending out $$v_{76}=v_{761}$$. Similarly
for nodes 2 to 5, and we obtained:

$$\begin{align}
v_{761} &=0 & v_{7612} &=0 & v_{7613} &=0 & v_{7614} &=0 & v_{7615} &=0 \\
v_{7621} &=1 & v_{762} &=1 & v_{7623} &=1 & v_{7624} &=1 & v_{7625} &=1 \\
v_{7631} &=0 & v_{7632} &=0 & v_{763} &=0 & v_{7634} &=0 & v_{7635} &=0 \\
v_{7641} &=1 & v_{7642} &=1 & v_{7643} &=1 & v_{764} &=1 & v_{7645} &=1 \\
v_{7651} &=0 & v_{7652} &=0 & v_{765} &=0 & v_{7654} &=0 & v_{765} &=0 \\
\end{align}$$

Therefore, nodes 1 to 5 deduced, using $$OM(0)$$, that:

$$\begin{align}
v'_{761} &= \mathrm{majority}(v_{761}, v_{7621}, v_{7631}, v_{7641}, v_{7651}) = 0 \\
v'_{762} &= \mathrm{majority}(v_{7612}, v_{762}, v_{7632}, v_{7642}, v_{7652}) = 0 \\
v'_{763} &= \mathrm{majority}(v_{7613}, v_{7623}, v_{763}, v_{7643}, v_{7653}) = 0 \\
v'_{764} &= \mathrm{majority}(v_{7614}, v_{7624}, v_{7634}, v_{764}, v_{7654}) = 0 \\
v'_{765} &= \mathrm{majority}(v_{7615}, v_{7625}, v_{7635}, v_{7645}, v_{765}) = 0 \\
\end{align}$$

Thus the consensus reached for $$v'_{76} = 0$$. Similarly for the unambiguous
value of $$v_{71}, v_{72}, v_{73}, v_{74}, v_{75}$$. Then all loyal lieutenant
will reach the conclusion of

$$v'_{7} = \mathrm{majority}(v'_{71}, v'_{72}, v'_{73}, v'_{74}, v'_{75}, v'_{76})$$

which is a tied vote and default decision is applied.

The paper then proceed to the case of signed message, which the authenticity of
message cannot be forged. In such case, after all lieutenants exchange messages
using signed channel, a lieutenant can confirm if the general who send out
messages are traitorous.

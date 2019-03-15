---
layout: post
title: "David et al (2016) DeepChess: End-to-end deep neural network for automatic learning in chess"
date: 2019-03-13
category: paper
bibtex:
  type: inproceedings
  title: "DeepChess: End-to-End Deep Neural Network for Automatic Learning in Chess"
  author: "Eli (Omid) David and Nathan S. Netanyahu and Lior Wolf"
  booktitle: "Proc. International Conference on Artificial Neural Networks (ICANN), Springer LNCS Volume 9887"
  pages: "88--96"
  address: "Barcelona, Spain"
  year: 2016
  doi: "10.1007/978-3-319-44781-0_11"
  arxiv: "arXiv:1711.09667 [cs.NE]"
  url: "https://arxiv.org/abs/1711.09667"
---

Goal of the paper is to derive the evaluation function for chess from scratch
using machine learning techniques. From scratch means not even input the chess
rule to the evaluation function.

The evaluation function for chess usually takes chess position as input and a
score as output. As a convention, score is from white's perspective and it is a
linear combination of all selected feature from the position.

The paper use a neural network and its training method is as follows: The model
to train receives two positions as input and learns to predict which position
is better (i.e., the output is binary). The data is from CCRL
(www.computerchess.org.uk/ccrl). There are 640K chess games with 221695 white
won and 164387 black won. The authors randomly extract 10 positions from each
game which the positions are not from the first 5 moves and not a capture. Each
position is converted into 773 bits:

- bitboard representation: two sides, six piece types, 64 squares = $$2\times 6\times 64=768$$ bits
- additional 5 bits of state: which side to move (white = 1), ability to castle
  (black and white, king- and queen- side castling)

One random position each from the game that white won and black won is paired
up as training data. If there are 1M position on each side, together with
swapping position of a pair, there will be 2 billion possible training data.

The neural network has two stages: The Pos2Vec stage is a deep autoencoder
network and used as nonlinear feature extractor. We expect that it converts a
chess position to a vector of values of high level features. The Pos2Vec
network has *five* layers of size 773-600-400-200-100 and using rectified
linear unit (ReLU) with no regularization.

The second stage is DeepChess, which is on top of two side-by-side Pos2Vec
networks and output layer has 2 softmax values to predict which of the two side
will win. The DeepChess network has 200 inputs (100 each from the two Pos2Vec
network) and has *four* layers of size 400-200-100-2, using ReLU with no
regularization, to compare the features of the positions from the two disjoint
Pos2Vec networks to determine which one is better.

The Pos2Vec network is trained for 200 epochs over 2M positions, which 1M are
white win and 1M are black win. The network is trained with first layer,
773-600-773, then second layer 600-400-600, and so on to complete the five
layers. The learning rate start from 0.005 and multiplied by 0.98 at the end of
each epoch.

The DeepChess network is supervised. It use previously trained Pos2Vec network
as the initial weights and after adding the four layers on top of the two
Pos2Vec networks, the whole network is trained again with 1M random input pairs
for 100 epochs. There are 100K positions each from white won and black won to
serve as validation set. Cross entropy is used. The learning rate starts from
0.01 and multiplied by 0.99 after each epoch.

No regularization is needed as the author claims that there are orders of
magnitude more potential training pairs than the one used. So we can always use
new training samples in each epoch.

The network is found to produce accuracy of 98.2% on training data and 98.0% on
validation data.

Figure 1 on page 3 of the paper shows the diagram of the network.

The paper also proposed some possible improvements. The alternative
configuration of 773-100-100-100 for Pos2Vec, and 100-100-2 instead of 4 layers
for DeepChess is called network distillation (use a smaller network). This
  sacrificed the accuracy a bit.

To use the neural network, we need a chess engine that does alpha-beta search.
But instead of evaluating for numerical value of each position, we store the
whole position in $$\alpha_{pos}$$ and $$\beta_{pos}$$. At each new position,
we compare it with existing $$\alpha_{pos}$$ and $$\beta_{pos}$$, to check:

- if the position is better than $$\alpha_{pos}$$, it would become the new $$\alpha_{pos}$$
- if the position is better than $$\beta_{pos}$$, the current node is pruned


# Further reading
- references 3, 4, 5, 6: genetic algorithms for auto evaluation function tuning when features are initialized randomly
- reference 10: deep reinforcement learning to play chess

---
layout: post
title: "Silver et al (2017) Mastering the game of Go without human knowledge"
date: 2019-03-12
category: paper
bibtex:
  type: article
  title: "Mastering the game of Go without human knowledge"
  author: "David Silver and Julian Schrittwieser and Karen Simonyan and Ioannis Antonoglou and Aja Huang and Arthur Guez and Thomas Hubert and Lucas Baker and Matthew Lai and Adrian Bolton and Yutian Chen and Timothy Lillicrap and Fan Hui and Laurent Sifre and George van den Driessche and Thore Graepel and Demis Hassabis"
  journal: Nature
  volume: 550
  pages: "354--359"
  month: Oct
  year: 2017
  url: http://dx.doi.org/10.1038/nature24270
---


This is the AlphaGo Zero paper that gave out detail on how the reinforcement learning is done.

The predecessor AlphaGo Fan was a success on Oct 2015, so as AlphaGo Lee. It was implemented as two deep neural networks, a policy network (that give out move probabilities) and a value network (that outputs position evaluation). The policy network was trained initially by *supervised learning* to predict human expert moves, and then refined by policy gradient reinforcement learning. The value network was trained to predict the winner of the game, by playing with the policy network against itself. After the two neural networks are trained, they are combined with Monte Carlo tree search to provide a lookahead search.

AlphaGo zero skipped the supervised learning part. It was trained solely by self-play reinforcement learning, started from random play. The result will be a single neural network instead of separated policy and value network. And it uses only black and white stones on the board as input features. When it is used, it will only be simple tree search without Monte Carlo rollouts. The reinforcement learning is performed as follows:

Let $$f_{\theta}$$ be a deep neural network with parameters $$\theta$$ and it takes the raw board representation $$s$$ of the position as input. The neural network outputs move probabilities and a value $$(\mathbf{p}, v) = f_{\theta}(s)$$, where:
- $$\mathbf{p}$$: vector of move probabilities, which prob of selecting each   move $$a$$ is $$p_a = \Pr[a\mid s]$$
- $$v$$: scalar value, estimating the prob of current player winning from position $$s$$

At each position $$s$$, an MCTS is guided by the neural network $$f_{\theta}$$ to find probabilities $$\pi$$ of playing each move and value (game winner) $$z$$. $$\pi$$ usually select much stronger moves than the raw move probabilities $$\mathbf{p}$$. The MCTS is a policy improvement operator and $$z$$ a policy evaluation. Then we update the parameters $$\theta$$ of the neural network to make $$(\mathbf{p}, v) = f_{\theta}(s)$$ more closely match $$(\pi, z)$$

The MCTS always start from root state and select to move to maximize the upper confidence bound $$Q(s,a)+U(s,a)$$, where $$U(s,a)\propto P(s,a)/(1+N(s,a))$$, until leaf node $$s'$$ is reached. Here,
- edge of the game tree is denoted by $$(s,a)$$, with $$s$$ the board state and $$a$$ the action
- $$P(s,a)$$ = prior probability
- $$N(s,a)$$ = visit count
- $$Q(s,a)$$ = action value

At $$s'$$, we then evaluate the prior probabilities and evaluate $$(P(s',\cdot), V(s')) = f_{\theta}(s')$$. We then update each edge $$(s,a)$$ traversed to increment its visit count $$N(s,a)$$ and to update its action value

$$Q(s,a) = \frac{1}{N(s,a)}\sum_{s'\mid s,a\to s'} V(s')$$

which the summation above is over all simulations that start from position $$s$$ taking move $$a$$ eventually reached $$s'$$.

The neural network is trained as follows:

The initial weights $$\theta_0$$ are random. In each subsequent iteration $$i\ge 1$$, games of self-play, where each move is identified by subscript $$t$$, are generated. At time $$t$$, MCTS outputs $$\mathbf{\pi}_t = \alpha_{\theta_{i-1}}(s_t)$$ using neural network $$f_{\theta_{i-1}}$$ and moves by sampling the search probabilities $$\mathbf{\pi}_t$$. The game terminates only it exceeds a maximum length, or when both players pass. A player pass when the search value below a resignation threshold. Therefore, at each time step, we collected data $$(s_t, \mathbf{\pi}_t, z_t)$$, which $$z_t \in \{-1, +1\}$$ is the game winner from the perspective of the current player at step $$t$$. The a new $$\theta_i$$ is trained from $$(s,\mathbf{\pi},z)$$ sampled uniformly among all time steps of iteration $$i-1$$.

The neural network $$(p,v) = f_{\theta_i}(s)$$ is adjusted to minimize the error between the predicted value $$v$$ and the self-play winner $$z$$, and to maximize the similarity between vectors of move probabilities $$p$$ and search probabilities $$\pi$$. It is done using gradient on loss function

$$\ell = (z-v)^2 - \pi^T \log p + c \lVert\theta\rVert^2$$

for some parameter $$c$$ of regularization to prevent overfitting. The above loss function is summing over mean-squared error and cross-entropy loss.

Training of AlphaGo Zero: 4.9M games of self-play generated, using 1600 simulations for each MCTS, which corresponds to 0.4s thinking time per move.

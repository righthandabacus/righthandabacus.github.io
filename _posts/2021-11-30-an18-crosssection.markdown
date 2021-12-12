---
layout: post
title: "Abe & Nakayama (2018) Deep Learning for Forecasting Stock Returns in the Cross-Section"
date: Tue, 30 Nov 2021 22:01:11
category: paper
bibtex:
  type: inproceedings
  title: "Deep Learning for Forecasting Stock Returns in the Cross-Section"
  author: "Masaya Ave and Hideki Nakayama"
  booktitle: "Proc. PAKDD"
  year: 2018
---

A paper to study cross-section return, i.e., return of multiple securities at
the same point in time. The models are trivial but good to learn about its
approach to the problem.

The evaluation in this paper is in two parts. First is to consider the
constituents of the MSCI Japan index in Jan 2017, which there are 319 stocks
covering 85% of the free float-adjusted market cap. A total of 25 factors are
collected, including the accounting ratios and market metrics (PB, PE,
earning/price, sales/price, ROE, ROA, currenct rati, equity ratioo, asset
growth, EPS, MV, beta, volatility, past year return, trading turnover, etc). To
make things realistic, some of these features are lagged for 4 months. The goal
is to make a regression model, that use data from $$T,T-3,T-6,T-9,T-12$$ to
predict for the return of $$T+1$$. So there will be $$25\times 5=125$$ input
features to predict for a single dimensional output.

The training and prediction are using 120+1 months data. The author collected
25 years of data and with a sliding window of 121 months. A model is trained
using first 120 months and predict for the final 1 month. There are 180 windows
for the evaluation. MSE and Spearman correlation coefficients are used as the
metric to compare models. The use of MSE is natural for a regression problem.
The Spearman is to consider that a long-short portfolio strategy is to
rebalance by swaping the underperformers with overperformers. Hence the ranking
is more important than the actual level of return.

Models considered are fully-connected neural networks (various depth and
configuration), SVR, and random forest. Traditional 3-layer network are
involved with varying hidden units of 70 to 120. Also tried are 5-layer and
8-layer networks in pyramid order of hidden neurons. For deep network, 50% drop
out is applied. So as in 3-layer network, the paper also tried with 244 to 399
neurons in hidden layer and applied 50% dropout. The networks are using tanh
activation. SVR are using RBF kernel with all combinations of $$C=0.1,1,10$$
and $$\gamma=10^{-4}, 10^{-3}, 10^{-2}, 10^{-1}$$ and
$$\epsiloon=10^{-2},10^{-1}$$. Random forest are to tried with 5 to 35 features
andd 3 to 20 depth. Both SVR and RF are using sklearn while neural network is
using Tensorflow.

The result is to prefer a 8-layer deep network with 120-120-70-70-20-20 neurons
over other neural network configurations, based on correlation and MSE. Note
that the correlation is only at a bit less than 0.06, which is not particularly
strong. SVR prefers $$C=0.1,\gamma=0.01,\epsilon=0.1$$ and RF prefers 25
features with 7 depth.

The paper also evaluated the classification problem (market direction). All
models can produce an accuracy of around 52 to 54 percent. It is not too high
but confirmed (using t-test?) to be significantly above the null hypothesis of
50%. The paper als verified that making an ensemble of the best NN, SVR and RF
model outperforms any of them.

The second part of the evaluation is applying the long-short portfolio
strategy. Portfolio is measured using the Sharpe ratio (return/risk). It is
using the net-zero investment by buying the top tertile (3) or quintile (5) and
selling the bottom tertile or quintile. It turns out the different machine
learning model achieved similar return and risk level but neural network can do
slightly better.

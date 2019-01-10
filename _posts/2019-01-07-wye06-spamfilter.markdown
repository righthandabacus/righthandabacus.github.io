---
layout: post
title: "Wang, Youssef, & Elhakeem (2006) On some feature selection strategies for spam filter design"
date: Mon, 07 Jan 2019 19:59:26 -0500
category: paper
bibtex:
  type: inproceedings
  title: "On some feature selection strategies for spam filter design"
  author: "Ren Wang and Amr M. Youssef and Ahmed K. Elhakeem"
  booktitle: IEEE Canadian Conference on Electrical and Computer Engineering
  month: May
  year: 2006
  pages: 2155--2158
  url: https://users.encs.concordia.ca/~youssef/Publications/Papers/On%20Some%20Feature%20Selection%20Strategies%20for%20Spam%20Filter%20Design.pdf
---

A short paper on comparing different algorithms to do feature selection in spam
filtering problems.

The paper describe spam filtering based on textual content as a text
categorization problem. The text is represented by a vector space model but a
naive way of doing so will result in a vector of very high dimension and
sparse. *Feature selection* is to choose only the most powerful discriminatory
terms from the vector.

This paper is just report of experimental result. The settings are as follows:
- email converted into vectors by TF-IDF
- 1099 emails from [Pu1Corpus](http://iit.demokritos.gr/skel/i-onfig/downloads),
  with 481 marked as spam
- K-nearest neighbor is used as the classifier
  - given candidate email vector, find the KNN, then label email by the most
    common class among these neighbors
  - nearest by Euclidean distance
- vectorization: consider the top 100 features according to document frequency (DF)
   - DF range: 0.02 to 0.5
   - feature selection algorithm then applied to find the most powerful
     discriminatory terms among the 100

Four feature selection algorithms are discussed in the paper:

Hill climbing:
- local neighborhood search, stops if neighborhood does not contain an improving solution
- may settle at first local optimum

```python
def Y(features, selected):
    # accuracy evaluation
    pass

def hill_climbing(features)
	'''Hill climbing algorithm, to find a neighbor differ by k=2 features. Goes
	for 1000 steps
	'''
    N = len(features)
    selected = [bool(random.randint(0,1)) for _ in range(N)]
	for _ in range(1000):
	    flipped = random.choices(range(N), k=2)
	    neighbor = [not x if i in flipped else x for i, x in enumerate(selected)]
	    if Y(features, selected) < Y(features, neighbor):
		    selected = neighbor
	return selected
```

Simulated annealing:
- local search inspired by cooling processes of molten metals
- hill climbing with probabilistic acceptance of non-improving moves, with probability
  $$p(s_1, s_2) = \exp\left(-\frac{1}{T}\max(0, Y(s_1)-Y(s_2))\right)$$
  where $$s_1,s_2$$ are the neighbors, $$Y(s)$$ is the accuracy evaluation
  function, and $$T$$ the temperature (a control parameter) which decreases
  with iterations and converges to 0, e.g. geometric sequence
- the probabilistic acceptance favors small deteriorations of the objective function

```python
def simulated_annealing(features)
	'''SA algorithm, to find a neighbor differ by k=2 features. Goes
	for 1000 steps with temperature defined as recipocal of iteration
	count, i.e., harmonic sequence
	'''
    N = len(features)
    selected = [bool(random.randint(0,1)) for _ in range(N)]
	for inv_t in range(1000):
	    flipped = random.choices(range(N), k=2)
	    neighbor = [not x if i in flipped else x for i, x in enumerate(selected)]
		Y1 = Y(features, selected)
		Y2 = Y(features, neighbor)
        T = 1.0/inv_T
		threshold = math.exp(-max(0, Y1-Y2)/T)
		if random.random() <= threshold:
		    selected = neighbor
	return selected
```

Threshold accepting:
- a variation of simulated annealing such that the acceptance of neighbor
  that leads to deteriorated objective function is based on a deterministic
  threshold rather than probabilistic

```python
def threshold_accepting(features)
	'''TA algorithm, to find a neighbor differ by k=2 features. Goes
	for 1000 steps with acceptance threshold of 0.66 in values of Y(s)
	'''
    N = len(features)
    selected = [bool(random.randint(0,1)) for _ in range(N)]
	for _ in range(1000):
	    flipped = random.choices(range(N), k=2)
	    neighbor = [not x if i in flipped else x for i, x in enumerate(selected)]
		Y1 = Y(features, selected)
		Y2 = Y(features, neighbor)
		if Y1 - Y2 < 0.66:
		    selected = neighbor
	return selected
```

Linear discriminant analysis (LDA):
- similar to PCA, to determine the set of the most discriminate projection axis
- assume feature samples from two different classes be
  $$x^{(1)} = \{x^{(1)}_1, x^{(1)}_2, \cdots, x^{(1)}_{L_1}\}$$ and
  $$x^{(2)} = \{x^{(2)}_1, x^{(2)}_2, \cdots, x^{(2)}_{L_2}\}$$ (each is a
  set of vectors), and $$x = x^{(1)}\cup x^{(2)}$$, the linear discriminant is

  $$\begin{align}
  \mathbf{w}^{\ast}
  &= \arg \max_\mathbf{w} J(\mathbf{w})
  = \frac{\mathbf{w}^T\mathbf{S}_B\mathbf{w}}{\mathbf{w}^T\mathbf{S}_W\mathbf{w}} \\
  \textrm{with}\quad
  \mathbf{S}_B &= (\mathbf{m}_1 - \mathbf{m}_2)(\mathbf{m}_1 - \mathbf{m}_2)^T \\
  \mathbf{S}_W &= \sum_{i=1,2}\sum_{\mathbf{x}\in x^{(i)}} (\mathbf{x}-\mathbf{m}_i)(\mathbf{x}-\mathbf{m}_i)^T
  \end{align}$$

  [LDA](https://en.wikipedia.org/wiki/Linear_discriminant_analysis) is to find
  the direction $$\mathbf{w}$$ which maximize the projected class means
  (numerator) while minimizing the class variance in the same direction
  (denominator)

The paper show by experiment that the performance is SA > TA > HC > LDA,
without rationale given.


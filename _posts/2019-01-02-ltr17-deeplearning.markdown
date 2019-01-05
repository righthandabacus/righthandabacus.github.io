---
layout: post
title: "Lin, Tegmark, & Rolnick (2017) Why does deep and cheap learning work so well?"
date: Wed, 02 Jan 2019 18:08:59 -0500
category: paper
bibtex:
  type: article
  title: "Why does deep and cheap learning work so well?"
  author: "Henry W. Lin and Max Tegmark and David Rolnick"
  journal: Journal of Statistical Physics
  month: September
  year: 2017
  volume: 168
  number: 6
  pages: 1223-1247
  url: https://arxiv.org/pdf/1608.08225.pdf
  url: https://link.springer.com/article/10.1007/s10955-017-1836-5
---

The paper points out in sec 1 that, in contrast to the good old-fashioned AI
(GOFAI) algorithms, artificial neural networks are understood only at a
heuristic level. The authors try to provide analytic insights on why deep
learning and general ANN works.

There are three kinds of machine learning applications:

- unsupervised learning: $$p(\mathbf{x})$$
- classification: $$p(y\mid\mathbf{x})$$
- generation or prediction: $$p(\mathbf{x}\mid y)$$

the above, we use $$\mathbf{x}$$ for data (e.g. the million greyscale pixels of
an image), $$y$$ for model parameters (e.g. the classification of an image) and
the problem is to find the probability distribution over a set of
possibilities. The quality of a neural network is to consider:

- expressibility: what function can a neural network express?
- efficiency: how many resources (neurons, parameters) does it require to approximate a given function?
- learnability: how rapidly can the neural network learn good parameters for approximating a function?

The paper points out that the set of possible functions is exponentially larger
than the set of functions can be represented by a neural network. Therefore, it
try to answer why a neural network can still approximate functions well. (For
example, there are $$256^{1000000}$$ possible images of 1 megapixel in
256-greyscale, but a neural network for image classification has merely
thousands or millions of parameters)

The key idea of the paper is to point out the combinatorial swindle nature of
neural networks, which for $$n$$ inputs taking $$v$$ values each, it cuts the
number of parameters from $$v^n$$ to $$O(vn)$$. Swindle works because the data
sets (problems) we are interested are drawn from an exponentially tiny faction
of all imaginable data sets.

The paper then move on to sec 2 to model a neural network. Firstly, the
probability distribution $$p(\mathbf{x}\mid y)$$ has many simplifying features
enabling accurate approximation, such as symmetry, locality, etc. Therefore the
harder case would be modeling $$p(y\mid\mathbf{x})$$, which can be expressed with
simpler function $$p(\mathbf{x}\mid y)$$ using Bayes' theorem:

$$p(y\mid\mathbf{x}) = \frac{p(\mathbf{x}\mid y)p(y)}{p(\mathbf{x})} = \frac{p(\mathbf{x}\mid y)p(y)}{\sum_{y'}p(\mathbf{x}\mid y')p(y')}$$

If we introduce the Hamiltonian $$H_y(\mathbf{x})$$ and self-information
$$\mu_y$$, we can rewrite $$p(y\mid\mathbf{x})$$ in Boltzmann form:

$$\begin{align}
H_y(\mathbf{x}) &= -\log p(\mathbf{x}\mid y) \\
\mu_y &= -\log p(y) \\
\Rightarrow\quad
p(\mathbf{x}\mid y) &= \exp(-H_y(\mathbf{x})) \\
p(y) &= \exp(-\mu_y) \\
p(y\mid\mathbf{x}) &= \frac{p(\mathbf{x}\mid y)p(y)}{\sum_{y'}p(\mathbf{x}\mid y')p(y')} \\
&= \frac{\exp(-H_y(\mathbf{x})-\mu_y)}{\sum_{y'}\exp(-H_{y'}(\mathbf{x})-\mu_{y'})} \\
&= \frac{1}{N(\mathbf{x})}e^{-[H_y(x)+\mu_x]} & \textrm{where }N(\mathbf{x}) = \sum_y e^{-[H_y(\mathbf{x})+\mu_y]}
\end{align}$$

Usually $$y$$ takes discrete values and we usually find $$p(\mathbf{x}\mid y)$$
over all values of $$y$$, so we can express $$p(\mathbf{x}\mid y), H_y(\mathbf{x}),
\mu_y$$ as vectors $$\mathbf{p},\mathbf{H}, \mathbf{\mu}$$ and write the above as

$$\mathbf{p}(\mathbf{x}) = \frac{1}{N(\mathbf{x})}e^{-[\mathbf{H}(\mathbf{x})+\mathbf{\mu}]}$$

The vector $$\mathbf{p}(\mathbf{x})$$ is approximated by a neural network. A
standard $$n$$-layer feedforward network implements a function of the form

$$\mathbf{f}(\mathbf{x}) = \mathbf{\sigma}_n\mathbf{A}_n \cdots \mathbf{\sigma}_2\mathbf{A}_2 \mathbf{\sigma}_1\mathbf{A}_1\mathbf{x} $$

Here the $$\mathbf{A}_i$$ is an affine transformation function of the form
$$\mathbf{A}_i\mathbf{x} = \mathbf{W}_i\mathbf{x} + \mathbf{b}_i$$ for some
matrices $$\mathbf{W}_i$$ and bias vector $$\mathbf{b}_i$$. And
$$\mathbf{\sigma}_i$$ is a nonlinear operator on vectors, which softmax
function often used:

$$\mathbf{\sigma}(\mathbf{x}) = \frac{e^{\mathbf{x}}}{\sum_i e^{y_i}}$$

Comparing this to the notation of $$\mathbf{p}(\mathbf{x})$$ above, we can see
that we can rewrite it as:

$$\mathbf{p}(\mathbf{x}) = \mathbf{\sigma}[-\mathbf{H}(\mathbf{x})-\mathbf{\mu}]$$

which means a neural network can *approximate this function with a softmax
output layer*, if we can compute $$\mathbf{H}(\mathbf{x})$$ in hidden
layers. The way to compute the Hamiltonians is as follows: First we expand the
Hamiltonian as power series (of infinite terms):

$$H_y(\mathbf{x}) = h + \sum_i h_ix_i + \sum_{i\le j}h_{ij}x_ix_j + \sum_{i\le j\le k}h_{ijk}x_ix_jx_k + \cdots$$

This is a multivariate polynomial. If vector $$\mathbf{x}$$ has $$n$$
components, there are $$\frac{(n+d)!}{n!d!}$$ terms of degree lower than or
equal to $$d$$ in the polynomial (to see this, consider the terms in polynomial
are combinations of $$n$$ terms of $$x_i$$ and $$d$$ count of 1).

If we introduce the popular choice of logistic sigmoid activation function
$$\sigma(x) = 1/(1+e^{-x})$$ to act elementwise on neuron output, we can see
that a two-layer neural network of the form
$$\mathbf{f}=\mathbf{A}_2\mathbf{\sigma}\mathbf{A}_1$$ can approximate
multiplication (paper fig 2):

$$\begin{align}
\sigma(u) &= \sigma(0) + \sigma'(0)u + \sigma''(0)\frac{u^2}{2} + O(u^3)\\
m(u,v) &= \frac{\sigma(u+v) + \sigma(-u-v) - \sigma(u-v) - \sigma(-u+v)}{4\sigma''(0)} \\
&= uv [ 1+O(u^2+v^2) ] \\
&\approx uv
\end{align}$$

which we cal $$m(u,v)$$ the multiplication approximator and it is accurate when
$$u,v$$ are small in absolute values -- which can be achieved by scaling. Here
we established the first theorem in the paper, we can achieve arbitrarily
accurate multiplication using 4 neurons.

In the alternative case that $$\mathbf{x}$$ is binary (i.e.,
$$x_i\in\{0,1\}$$), we can make things simpler because we always have
$$x_i^2=x_i$$ and hence the power series of Hamiltonian has finite terms:

$$H_y(\mathbf{x}) = h + \sum_i h_ix_i + \sum_{i<j}h_{ij}x_ix_j + \sum_{i<j<k}h_{ijk}x_ix_jx_k + \cdots + h_{1\cdots n}x_1\cdots x_n$$

And the product of bits can be trivially determined from their sum, for
example, $$x_1 x_2 x_3=1$$ iff $$x_1+x_2+x_3=3$$. Therefore, with the logistic
sigmoid, we can write the product

$$\prod_{i\in K} x_i = \lim_{\beta\to\infty} \sigma\left(-\beta\left(k-\frac{1}{2}-\sum_{x\in K}x_i\right)\right)$$

for some set of subscript indices $$K$$. A moderately large $$\beta$$ in the
equation above is suffice to approximate the product, as the function
$$\sigma(x)$$ approaches 0 or 1 exponentially. Fig 2 of the paper also
shows a network with a single neuron to find the multiplication of bits.

At this point, we can see that, as long as we can express the Hamiltonian in
the form of a power series, we can approximate it in a neural network. However,
the Hamiltonian that we will approximate in machine learning problems are
usually simple, typically a polynomial of degree 2 to 4, and the number of
coefficients in $$O(n%4)$$, for the following reasons:

- Taylor's theorem suggests that we can discard the higher order terms (coefficient tends to zero in higher degrees)
- *Renomalization*: Higher order terms in the Hamiltonian are negligible if we only observe macroscopic variables
- many probability distributions can be accurately approximated by multivariate Gaussians, due to Central Limit Theorem:

  $$p(\mathbf{x}) = \exp\left(h+\sum_ih_ix_i-\sum_{ij}h_{ij}x_ix_j\right)$$

  so the Hamiltonian $$H=-\log p$$ is a quadratic polynomial.
 
These reasons are valid only if we observe the random variable directly,
instead of observing some generic functions of the random variables. But a
Hamiltonian in low-order polynomial does not necessarily mean a neural
network can reproduce it exactly due to other requirements such as accuracy,
stuck at local minimum, or quality of data.

Besides the order of the polynomial, the principle of locality, which means the
range of effect of a variable is only at its vicinity, also causes a lot of
coefficients in the Hamiltonian power series vanish and the resulting
polynomial has only $$O(n)$$ terms. Moreover, the nature of the problem which
usually is invariant under translation and rotation limits the number of
parameters further.

Discussion thus far is about neural networks in general and the further
question is why deep learning works well in some problems while shallow neural
networks do not. The reason this paper proposed is the hierarchical structure
of physical world, and the dynamic in physics is fundamentally Markovian: data
of hierarchy $$i$$ depends on data of hierarchy $$i-1$$ only and the
probability vector of data from each hierarchy is therefore in the form of

$$\mathbf{p}_n = \mathbf{M}_n\mathbf{M}_{n-1}\cdots\mathbf{M}_1\mathbf{p}_0$$

for some Markov transition matrix $$\mathbf{M}_k$$. A deep neural network
stacking a lot of simpler networks would then implement such *generative
process* efficiently. Using such deep neural network is to reverse the
hierarchical generative process to find the primitive input $$y$$ from observed
output $$\mathbf{x}$$.

Then we can proceed to describe these analytically, to show that deep neural
network is more efficient in certain cases:

First we consider $$p(y\mid x)$$, if $$p(y\mid x) = p(y\mid T(x))$$ then
$$T(x)$$ is a sufficient statistic. If $$T_{\ast}(x)$$ is a minimal sufficient
statistic, it means for all sufficient statistic $$T(x)$$ there is a function
$$f$$ such that $$T_{\ast}(x) = f(T(x))$$. The physical meaning of
$$T_{\ast}(x)$$ is an information distiller, which retains only the information
that is relevant and discarding all other. Consider the Markov property
$$P(y_i\mid y_{i+1}, y_{i+k}) = P(y_i\mid y_{i+1})$$, if we have $$T_i$$ as the
minimal sufficient statistic of $$P(y_i\mid y_n)$$, we have

$$\begin{align}
P(y_i\mid y_n)
&= \sum_{y_{i+1}} P(y_i\mid y_{i+1}, y_n)P(y_{i+1}\mid y_n) \\
&= \sum_{y_{i+1}} P(y_i\mid y_{i+1})P(y_{i+1}\mid T_{i+1}(y_n))
\end{align}$$

which means it depends on $$y$$ only through $$T_{i+1}(y_n)$$, i.e. $$T_{i+1}$$
is a sufficient statistic for $$P(y_i\mid y_n)$$, so we established theorem 2
that there exists a function $$f_i$$ such that $$T_i = f_i \circ T_{i+1}$$, and hence by induction,

$$\begin{align}
P(y_0\mid y_n) &= (f_0\circ f_1\circ\cdots\circ f_n)(y_n) \\
\therefore T_0 &= f_1\circ f_2\circ \cdots \circ T_{n-1}
\end{align}$$

The above expression states that, the structure of the inference problem (from
image $$\mathbf{x}$$ to classification $$y$$) reflects the structure of the
generative process (from classification $$y$$ to image $$\mathbf{x}$$).

The minimal sufficient statistics can also be put into information theory
context: The data processing inequality states that, for any function $$f$$
(the information distillation function) and random variables $$x,y$$, the
mutual information measure satisfies

$$I(x,y) \ge I(x, f(y))\quad\textrm{where }I(x,y)=\sum_{x,y}p(x,y)\log\frac{p(x,y)}{p(x)p(y)}$$

and a sufficient statistic is a function $$f(y)=T(y)$$ such that
$$I(x,y) = I(x, T(y))$$ which in other words, retained all information in
$$y$$. But if $$f(x)$$ is not strictly sufficient, it is still useful as long
as it retained most of the relevant information and make the Hamitonian
$$H_y(f(x))$$ easier to implement than $$H_y(x)$$

Finally, we are about to establish the no-flattening theorems, which states
that for some problems, deep neural network $$\mathbf{f}$$ is more efficient
than shallow one $$\mathbf{f}_{\epsilon}^{\ell}$$ (which only has only $$\ell$$
hidden layers and the error compare to $$\mathbf{f}$$ is bounded by
$$\epsilon$$) in terms of the number of parameters involved (i.e., the sum of
dimensions of its hidden layers, or number of neurons, $$N_n$$ and te number of
non-zero entries in weight matrices, or the number of synapses, $$N_s$$).

This result starts with defining the flattening cost functions:

$$\begin{align}
C_n(\mathbf{f},\ell,\epsilon) &= \min_{\mathbf{f}_{\epsilon}^{\ell}}\frac{N_n(\mathbf{f}_{\epsilon}^{\ell})}{N_n(\mathbf{f})} \\
C_s(\mathbf{f},\ell,\epsilon) &= \min_{\mathbf{f}_{\epsilon}^{\ell}}\frac{N_s(\mathbf{f}_{\epsilon}^{\ell})}{N_s(\mathbf{f})}
\end{align}$$

and if $$C_n>1$$ or $$C_s>1$$ for a problem, then flattening is costly and inefficient.

The paper qualitatively provide a few examples. Some numerical linear algebra
problems that allows divide-and-conquer approach (such as FFT) is in
$$O(n\log n)$$ complexity, which is suitable for deep neural network and it
will become $$O(n^2)$$ if hidden layers are eliminated to become a fully
flattened network. Hence $$C_s = O(n/\log n) \approx O(n)$$ in this example.

Another example is the matrix multiplication. For $$\mathbf{A}$$ of rank $$k$$,
$$\mathbf{B}$$ is $$k\times n$$ matrix and $$\mathbf{C}$$ is $$n\times k$$
matrix, then $$\mathbf{A} = \mathbf{B}\mathbf{C}$$ implemented in a flattened
neural network will have $$n^2$$ synapses in a network with $$\ell=0$$ layers
and $$2nk$$ synapses in a network with $$\ell=1$$ layers. So the flattening
cost is $$C_s = n/2k$$, which is greater than 1 for $$k<n/2$$.

Yet another example is the polynomial. It is vigorously proved in the appendix
of the paper that they are exponentially expensive to flatten. One result is
that, a monomial $$x_1x_2\cdots x_n$$ for $$n=2^k$$ needs $$4n$$ neurons to
evaluate if the multiplication are arranged in a $$k$$-layer binary tree, which
each two-element multiplication is performed by 4 neurons. But if this is to be
done by a flattened network, it will require $$2^n$$ neurons.

Summary of the paper:

1. Machine learning in practice inclined to certain class of problems that involves
   exceptionally simply probability distributions
2. Shallow network hinges on symmetry, locality, and polynomial log-probability
   in data from natural world, which has sparse low-order Hamiltonians
3. A neural network with a finite size and a generic smooth activation function
   can approximate a multivariate polynomial well
4. Success of deep learning depends on the hierarchical and compositional
   generative process in physics; such compositional functions can be efficiently
   implemented by a deep neural network
5. Those compositional functions generally not possible to retain the efficiency
   if the neural network is flattened 

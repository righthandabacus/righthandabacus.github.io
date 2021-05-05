---
layout: post
title: Threshold linear regression
date: Mon, 12 Apr 2021 14:41:53 -0400
tags: code math
---

Threshold regression means to do regression separately in different segments,
as separated by some threshold. Take linear models as example, we have a
response variable $$y$$ and predictors $$X$$, and additionally, we have a
discriminator $$q$$, which may be derived from $$X$$, and set of transition
functions $$g_i$$. The model is expressed as
$$
y = \sum_{i=1}^m (\mu_i + \beta_i^T x)g_i(q),
$$
of which, there would always be exactly one of the $$g_i(q)$$ set to 1 and all
other would be zero. In essense, if we know of the value of $$g_i(q)$$ apriori,
$$X$$ and $$y$$ are related with a linear model. For convenience of some
derivation, we may rewrite the above into

$$
y = \mu_1 + \beta_1^T x + \sum_{i=2}^m (\mu'_i + \beta'^T_i x)g'_i(q)
$$

so that $$\mu_k = \sum_{i=1}^k \mu'_i$$ and $$\beta_k = \sum_{i=1}^k \beta'_i$$
with $$\mu_1=\mu'_1$$ and $$\beta_1=\beta'_1$$ and $$g'_i(q)$$ defined
correspondingly. A simple case of transition functions $$g_i$$ would be an
indicator function. For example, we have a vector of thresholds $$(-\infty,
c_1, c_2, \cdots, c_{m-1}, \infty)$$ to partition $$\mathbb{R}$$ into $$m$$
segments, it can be that $$g_i(q)=\mathbb{I}\{q\in(c_{i-1},c_i]\}$$ and
therefore $$g'_i(q)=\mathbb{I}\{q>c_{i-1}\}$$.  Further detail can be seen
from, for example, González et al (2005)[^1].

Regression on a linear model can be obtained from OLS. But in case of threshold
regression, the introduction of unknown threshold variables
$$(c_1,\cdots,c_{m-1})$$ made the model regression difficult. Usually the thresholds
are found by exhaustive search on grid points[^2]. There is, however, an attempt to
solve for the threshold values together with the regression coefficients[^3].
Its model is as follows:

$$
\begin{aligned}
y &= \sum_{i=1}^m \beta_i^T x \mathbb{I}\{c_{i-1}<q\le c_i\} + \epsilon \\
&= \sum_{i=1}^m \beta'^T_i x \mathbb{I}\{c_{i-1}<q\} + \epsilon
\end{aligned}
$$

which $$m$$ is also a parameter to be found, and can be greater than the actual
number of segments in the model. In order to perform regression, we can
introduce the following loss metric[^3] to be minimized:

$$
L(\beta,c) = \frac{1}{n}\sum_{i=1}^n\Big(y_i - \sum_{j=1}^m \beta'^T_j x_i\mathbb{I}\{c_{i-1}<q_i\}\Big)^2 + \sum_{j=2}^m p_\lambda(\Vert \beta'_j\Vert_1)
$$

This is defined as the mean squared error plus some penalty metric. The penalty
function $$p_\lambda(\cdot)$$ of choice is the smoothly dlipped absolute
deviation (SCAD) function[^4]. It is defined, with parameters $$a$$ and
$$\lambda$$, as

$$
p_\lambda(\omega)=\begin{cases}
\lambda\omega & \omega\le\lambda \\
\dfrac{2a\lambda\omega-\omega^2-\lambda^2}{2(a-1)} & \lambda<\omega\le a\lambda \\
\dfrac{\lambda^2(a+1)}{2} & a\lambda<\omega
\end{cases}
$$

and its first-order derivative is correspondingly

$$
p'_\lambda(\omega)=\begin{cases}
\lambda & \omega\le\lambda \\
\dfrac{a\lambda-\omega}{a-1} & \lambda<\omega\le a\lambda\\
0 & a\lambda<\omega
\end{cases}
$$

Note that the parameter to the SCAD function is the L1-norm of $$\beta'_j$$,
which at the region of $$\Vert \beta'_j\Vert_1 \le \lambda$$, minimizing this
loss metric is same as performing lasso regression.

The benefit of using SCAD as penalty function can be found from Fan & Li
(2001)[^4]. Essentially, if this loss metric is minimized, some of the
$$\beta'_j$$ term may be forced to zero by such penalty to infer that the
corresponding segment can be omitted but merged with the previous segment.
However, this loss metric cannot be easily minimized as the transition function
$$\mathbb{I}\{c_{j-1}<q\}$$ is not always differentiable. Therefore, Jiang et
al (2016)[^3] suggested to replace it with a smoothed function. Namely, make
$$
\mathbb{I}\{c_{j-1}<q\} = \Phi\left(\frac{q-c_{j-1}}{h}\right)
$$
for some small value $$h$$. This is the standard normal CDF and runs from 0 to
1 over $$\mathbb{R}$$. With small $$h$$, this is approximately an indicator
function turning from 0 to 1 at $$c_{j-1}$$. Hence the smoothed loss metric is

$$
Q(\beta,c) = \frac{1}{n}\sum_{i=1}^n\Big(y_i - \sum_{j=1}^m \beta'^T_j x_i\Phi\left(\frac{q_i-c_{j-1}}{h}\right)\Big)^2 + \sum_{j=2}^m p_\lambda(\Vert \beta'_j\Vert_1)
$$

and $$\partial Q/\partial c_i$$ and $$\partial Q/\partial \beta'_i$$ are well
defined. We may therefore find the minimizer using Newton's method or otherwise
and the regression can be obtained.

In order to try out this idea, the following code is implemented:

```python
import tensorflow as tf
import tensorflow_probability as tfp

import numpy as np
import scipy as sp
from scipy.stats import norm

# Generate data points
n = 2000
in_theta = [-3, -2, 4, -1, -7, 4]
in_c = [-np.sqrt(3), np.sqrt(3)]
x = np.linspace(-6,6,n).reshape(-1,1)
y = np.where(x<in_c[0], in_theta[0]+in_theta[1]*x,
    np.where(x<in_c[1], in_theta[2]+in_theta[3]*x,
                        in_theta[4]+in_theta[5]*x)
    ) + np.random.rand(n).reshape(-1,1)
X = tf.constant(np.hstack([np.ones((n,1)), x]), dtype="float32")
y = tf.constant(y, dtype="float32")
in_theta = np.asarray(in_theta).reshape(-1,2)
in_beta = np.vstack([in_theta[0:1], np.diff(in_theta, axis=0)])

class ThresholdRegressionKeras(tf.keras.Model):
    def __init__(self, d, m, lamb, a, h, **kwargs):
        super().__init__(**kwargs)
        self.d = d
        self.m = m
        self.lamb = lamb
        self.a = a
        self.h = h
        self.c = tf.Variable(np.sort(np.random.randn(m-1)), dtype='float32', name="c")
        self.beta = tf.Variable(np.random.randn(d,m), dtype='float32', name="beta")
        self.normal = tfp.distributions.Normal(loc=0, scale=1)
    def call(self, x):
        """Compute the threshold regression response y using the last column of
        x as thresholding variable"""
        # thresholding variable as the last column of x
        q = x[:, -1:]
        # find the smoothed indicator function
        ind = self.normal.cdf((q-self.c)/self.h)
        # threshold linear model using the smoothed indicator function
        xbeta = x @ self.beta
        y = xbeta[:, 0] + tf.reduce_sum(xbeta[:, 1:] * ind, axis=1)
        return tf.reshape(y, (-1,1))

def loss(model):
    '''Penality loss function factory'''
    def loss_(y, y_hat):
        residual = y - y_hat
        mse = tf.reduce_mean(tf.square(y-y_hat))
        b_norm = tf.norm(model.beta[:, 1:], ord=1, axis=0)
        # SCAD penalty on L1-norm of beta
        penalty = tf.where(b_norm <= model.lamb,
                           model.lamb*b_norm,
                  tf.where(b_norm <= model.a*model.lamb,
                           (2*model.a*b_norm - tf.square(b_norm) - model.lamb**2)/(2*model.a-2),
                           (model.a+1)*model.lamb**2/2))
        return mse + tf.reduce_sum(penalty)
    return loss_

# find h
q = X[:,-1:]
n = len(q)
h = np.log(n)*q.numpy().std(ddof=1)/n

earlystop = tf.keras.callbacks.EarlyStopping(
    monitor='loss',
    min_delta=0,
    patience=100,
    verbose=1,
    mode='min',
    baseline=None,
    restore_best_weights=True
)

model = ThresholdRegressionKeras(d=2, m=3, lamb=1.0, a=3.7, h=h)
sgd = tf.keras.optimizers.SGD(learning_rate=0.05, momentum=0.0, nesterov=False, name="SGD")
loss_metric = loss(model)
model.compile(optimizer=sgd, loss=loss_metric)
model.fit(X, y, epochs=10000, batch_size=n, verbose=False, callbacks=[earlystop])
with np.printoptions(precision=4, suppress=True):
    for v in model.variables:
        print("Model {} (dtype {})".format(v.name, v.dtype.name))
        print(v.numpy())
    print("Loss: {}".format(loss_metric(y, model.predict(X)).numpy().squeeze()))
    print("MSE: {}".format(tf.keras.losses.MSE(tf.reshape(y, (-1,)), tf.reshape(model.predict(X), (-1))).numpy().squeeze()))
    print("h: {}".format(model.h))
    print("Expected beta:")
    print(in_beta.T)
    print("Expected c:")
    print(np.asarray(in_c))
```

We can make the same with SciPy and its optimize function but using Tensorflow
would be easier as the gradient vector can be inferred automatically (and comes
with the gradient descend algorithm). This model, however, does not perform
well. The issue is on the threshold values $$c_j$$ that, it may produce
$$c_{j-1} > c_j$$ and distorted the linear model. We may further add a penalty
factor to the loss function to correct this but after all, I suspect the loss
function is not concave with respect to the threshold variables $$c_j$$ ---
hence the global minimum is not always found.

To compare, we can build a grid-search model as follows, using scikit-learn for
its linear regression function:

```python
import json
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Generate data points
n = 2000
in_theta = [-3, -2, 4, -1, -7, 4]
in_c = [-np.sqrt(3), np.sqrt(3)]
x = np.linspace(-6,6,n).reshape(-1,1)
y = np.where(x<in_c[0], in_theta[0]+in_theta[1]*x,
    np.where(x<in_c[1], in_theta[2]+in_theta[3]*x,
                        in_theta[4]+in_theta[5]*x)
    ) + np.random.rand(n).reshape(-1,1)
in_theta = np.asarray(in_theta).reshape(-1,2)

# Threshold regression model
class ThresholdRegression:
    def __init__(self, thresholds):
        self.thresholds = thresholds
    def fit(self, X, y, q):
        self.regs = []
        thresh = np.append(-np.inf, np.append(self.thresholds, np.inf))
        for lthresh, uthresh in zip(thresh, self.thresholds):
            selector = (lthresh < q) & (q <= uthresh)
            if selector.any():
                self.regs.append(LinearRegression().fit(X[selector], y[selector]))
            else:
                # degenerated case of no data point
                self.regs.append(None)
        selector = (q >= self.thresholds[-1])
        self.regs.append(LinearRegression().fit(X[selector], y[selector]))
        return self
    def predict(self, X, q):
        thresh = np.append(-np.inf, np.append(self.thresholds, np.inf))
        y = np.zeros_like(q)
        for i in range(len(self.thresholds)+1):
            # segment i: thresh[i] < q < thresh[i+1]
            selector = (thresh[i] <= q) & (q < thresh[i+1])
            if selector.any():
                y = np.where((thresh[i] <= q) & (q < thresh[i+1]), self.regs[i].predict(X).reshape(y.shape), y)
        return y.reshape(-1,1)
    def mse(self, X, y, q):
        y_hat = self.predict(X, q)
        return ((y - y_hat)**2).mean()
    def score(self, X, y, q):
        y_hat = self.predict(X, q)
        u = ((y - y_hat)**2).sum()
        v = ((y - y.mean())**2).sum()
        return 1-(u/v)
    def get_params(self):
        params = {
            th: {'inter':reg.intercept_.squeeze().tolist(),
                 'coef':reg.coef_.squeeze().tolist()}
            for reg, th in zip(self.regs, self.thresholds) if reg
        }
        if self.regs[-1]:
            params[float('inf')] = {
                'inter':self.regs[-1].intercept_.squeeze().tolist(),
                'coef':self.regs[-1].coef_.squeeze().tolist(),
            }
        return params

def is_ascending(vec):
    "Check if the sequence `vec` is strictly ascending numbers"
    if len(vec) < 2:
        return True
    vec = np.asarray(vec).reshape(-1,)
    return (np.diff(vec) > 0).all()

# Enumerate thresholds in grid
m = 3  # num thresholds
lbound, ubound = -5.0, 5.0
grid_precision = 0.01
n_grid = int((ubound - lbound)/grid_precision) + 1
thresh_points = np.linspace(lbound, ubound, n_grid)
thresh_arrays = np.meshgrid(*([thresh_points]*(m-1)))
best_r_score = -np.inf
best_mse_score = np.inf
best_r_model = best_mse_model = None
for thresh_rows in zip(*thresh_arrays):
    for threshs in zip(*thresh_rows):
        if not is_ascending(threshs):
            continue # bad thresholds vector
        model = ThresholdRegression(threshs).fit(x,y,x[:,0])
        score = model.score(x,y,x[:,0])
        if score > best_r_score:
            best_r_score, best_r_model = score, model
        score = mean_squared_error(y, model.predict(x,x[:,0]))
        if score < best_mse_score:
            best_mse_score, best_mse_model = score, model
# Reference model
ref_model = ThresholdRegression(in_c).fit(x,y,x[:,0])

with np.printoptions(precision=4, suppress=True):
    print("Best R^2 model")
    print(json.dumps(best_r_model.get_params(), indent=4))
    print("Best R^2 score: {}".format(best_r_score))
    print("Best R^2 MSE: {}".format(mean_squared_error(y, best_r_model.predict(x,x[:,0]))))
    print("Best MSE model")
    print(json.dumps(best_mse_model.get_params(), indent=4))
    print("Best MSE R^2: {}".format(best_mse_model.score(x, y, x[:,0])))
    print("Best MSE score: {}".format(best_mse_score))
    print("Reference model")
    print(json.dumps(ref_model.get_params(), indent=4))
    print("Reference score: {}".format(ref_model.score(x,y,x[:,0])))
    print("Reference MSE: {}".format(mean_squared_error(y, ref_model.predict(x,x[:,0]))))
```

This code often gives good result, but surprisingly, not always reproduce the
parameters of the original model. Depends on the particular structure of the
data, sometimes a slightly different threshold partition and hence a slightly
different model would be found that produces a smaller MSE or higher $$R^2$$
score. But this is a very slow search trying out half a million combinations of
threshold partitions.  I can find several factors that impact the accuracy of
regression: The imbalanced number of samples in each segment, the magnitude of
the noise, and the similarity of linear models of neighbouring segments.

Introducing a test set or using k-fold validation may improve the search result
in both code. But this is sufficient to show that threshold regression, despite
a simple extension to linear model, can be a hard problem to solve.


[^1]: Andrés González, Timo Teräsvirta, and Dick van Dijk, "Panel Smooth Transition Regression Models". Research Paper 165, Quantitative Finance Research Centre, University of Technology Sydney. August 2005.
[^2]: Youyi Fong, Ying Huang, Peter B. Gilbert, and Sallie R. Permar, "chngpt: threshold regression model estimation and inference". BMC Bioinformatics 18:454, 2017.
[^3]: J. K. Jiang,  H. Z. Lin, L. Jiang, and Paul Siu Fai Yip, "Estimation of threshold values and regression parameters in threshold regression model" (in Chinese). Scientia Sinica Mathematica, 46(4):409--422, 2016.
[^4]: Jianqing Fan and Runze Li, "Variable Selection via Nonconcave Penalized Likelihood and its Oracle Properties". Journal of the American Statistical Association, 96(456):1348--1360, December 2001.

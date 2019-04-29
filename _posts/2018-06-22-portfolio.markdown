---
layout: post
title: Modern portfolio theory
date: Fri, 22 Jun 2018 16:27:03 -0400
published: true
tags: work math
---

Markowitz portfolio theory, CAPM, Arbitrary Pricing Theory

# Markowitz portfolio theory

Simple return: $$\dfrac{P_t - P_{t-1}}{P_{t-1}}$$

Continuous return: $$\log\dfrac{P_t}{P_{t-1}}$$

Markowitz (1952) is generically known as the mean-variance framework. With assumptions:

1. decision based on expected return and risk, as measured by the mean and variance of return
   - maximize mean return, minimize the standard deviation of the return
2. all investors have same time horizon
3. all investors agree on the means, variances, and correlations of returns on various investments
   - all information is freely and simultaneously available to all market participants
4. assets are arbitrarily fungible (mutually interchangable)

Portfolio with $$n$$ assets, each with return $$R_{t,i}$$ at time $$t$$, which the
mean and variance are $$\mu_{t,i},\sigma_{t,i}^2$$ respectively. Weight of asset $$i$$
in the portfolio is $$w_i\ge 0$$, and $$\sum_{i=1}^n w_i = 1$$. The return, mean, and variance
of the portfolio is denoted without subscript.

The mean of the portfolio is the linear combination:

$$ \mu = E\left[\sum_{i=1}^n w_iR_i\right]=\sum_{i=1}^n w_i\mu_i = \vec{w}'\vec{\mu} $$

the variance will be expressed as the weight vector $$\vec{w}$$ and the covariance
matrix $$\Sigma$$:

$$\begin{align}
\sigma^2(R)
&= E[(R-\mu)^2] \\
&= E\left[\left(\sum_{i=1}^n w_i(R_i-\mu_i)\right)^2\right] \\
&= E\left[\sum_{i=1}^n\sum_{j=1}^n w_iw_j(R_i-\mu_i)(R_j-\mu_j)\right] \\
&= \sum_{i=1}^n\sum_{j=1}^n w_iw_jE[(R_i-\mu_i)(R_j-\mu_j)] \\
&= \sum_{i=1}^n\sum_{j=1}^n w_iw_j\sigma_{ij} \\
&= \vec{w}'\Sigma\vec{w}
\end{align}$$

Note that if we define the correlation matrix $$P=(\rho_{ij})$$ with
$$\rho_{ij}=\dfrac{\sigma_{ij}}{\sigma_i\sigma_j}$$, and diagonal matrix of std dev $$S$$ to be
$$\sigma_i$$ on diagonal and zero off-diagonal. Then $$\Sigma = SPS$$ and
$$\sigma(R)=\sqrt{\vec{w}SPS\vec{w}}$$.

Diversification: If $$\rho_{ij}=\delta_{ij}$$, i.e. $$\sigma_{ij}=0$$ whenever
$$i\ne j$$, and with identical weight $$w_i=1/n$$, then $$\lim_{n\to\infty}\sigma^2(R)=0$$.
But if the assets are correlated, this variance approaches the average covariance over
all pairs of distinct asset as $$n\to\infty$$.

## Effect of correlation

Consider a simplified example of a portfolio with two assets, then

$$\begin{align}
w_1, w_2 &\ge 0 \\
w_1+w_2 &= 1 \\
\mu = E[R] &= w_1\mu_1 + w_2\mu_2 \\
\sigma^2(R)&= \begin{bmatrix}w_1 & w_2\end{bmatrix}
              \begin{bmatrix}\sigma_1 & 0 \\ 0 & \sigma_2\end{bmatrix}
              \begin{bmatrix}1 & \rho_{1,2} \\ \rho_{1,2} & 1\end{bmatrix}
              \begin{bmatrix}\sigma_1 & 0 \\ 0 & \sigma_2\end{bmatrix}
              \begin{bmatrix}w_1 \\ w_2\end{bmatrix} \\
&= w_1^2\sigma_1^2 + 2w_1w_2\rho\sigma_1\sigma_2 + w_2^2\sigma_2^2
\end{align}$$

The above describes $$(\mu,\sigma)$$ in parametric form of $$w_1,w_2$$.
We can optimize for minimum $$\sigma$$, for example, which the minimum is attained at

$$ w_1 = \frac{\sigma_2^2 - \rho\sigma_1\sigma_2}{\sigma_1^2+\sigma_2^2-2\rho\sigma_1\sigma_2} $$

If we take the correlation $$\rho_{1,2}$$ between the two asset to some extreme
values:

- $$\rho_{1,2}=0$$, the asset are uncorrelated, then $$\sigma^2(R)=w_1^2\sigma_1^2+w_2^2\sigma_2^2$$
- $$\rho_{1,2}=1$$, then $$\sigma^2(R)=(w_1\sigma_1+w_2\sigma_2)^2$$
- $$\rho_{1,2}=-1$$, then $$\sigma^2(R)=(w_1\sigma_1-w_2\sigma_2)^2$$

## Efficient frontier

In case we have more assets can potentially included in the portfolio, there is
certain combinations $$\vec{w}$$ such that they maximize the return $$\mu$$ for
the fixed $$\sigma$$. These set of portfolio is the efficient frontier.

The investor should have a utility function that depends on $$\mu,\sigma$$ and
he should choose the portfolio in the frontier that maximized the utility, i.e.:

$$\begin{gather}
\max\ U(\mu(\vec{w}),\sigma(\vec{w})) \\
\textrm{s.t. }\vec{w} = \arg\max \sigma(\vec{w}|\mu)
\end{gather}$$

If short sales are allowed, we require only $$\sum_{i=1}^n w_i = 1$$ but not
$$w_i\ge 0$$. Risk free lending/borrowing is modeled by an asset with
$$\sigma=0$$ and $$\mu=r$$ (constant), which lending/borrowing is long/short
the asset.

# CAPM

Markowitz theory needs $$2n+\binom{n}{2}$$ parameters. Sharpe (1964) provided a simplified model:

$$ R_i(t) = \alpha_i + \beta_i R(t) + e_i(t) $$

where $$i$$ is the index for a single security, $$R(t),R_i(t)$$ are the return
of the market and the security respectively at time $$t$$, $$e_i(t)$$ is noise
term --- a random variable of mean zero. The parameters $$\alpha_i,\beta_i$$
can be obtained by regression using EWMA of $$R(t)$$ across history:

$$ E[R_i(t)] = \alpha_i + \beta_i E[R(t)] $$

Also, we can have:

$$\begin{align}
E[(R_i - \bar{R}_i)^2] &= E[(\beta_i(R-\bar{R})+e_i)^2] \\
\textrm{i.e., } \sigma_i^2 &= \beta_i^2 \sigma^2(R) + \sigma^2(e_i) \\
E[(R_i - \bar{R}_i)(R_j - \bar{R}_j)] &= E[(\beta_i(R-\bar{R})+e_i)(\beta_j(R-\bar{R})+e_j)] \\
\textrm{i.e., } \sigma_{ij} &= \beta_i\beta_j \sigma^2(R)
\end{align}$$

Thus we have a simplier way to derive the covariance matrix of these $$n$$ assets:

$$\begin{align}
\Sigma &= \begin{bmatrix}
	\beta_1^2\sigma^2(R)+\sigma^2(e_1) & \beta_1\beta_2\sigma^2(R)          & \cdots & \beta_1\beta_n\sigma^2(R) \\
	\beta_2\beta_1\sigma^2(R)          & \beta_2^2\sigma^2(R)+\sigma^2(e_2) & \cdots & \beta_2\beta_n\sigma^2(R) \\
	\vdots                             & \vdots                             &        & \vdots \\
	\beta_n\beta_1\sigma^2(R)          & \beta_n\beta_2\sigma^2(R)          & \cdots & \beta_n^2\sigma^2(R)+\sigma^2(e_2)
\end{bmatrix} \\
&= \vec{\beta}\vec{\beta}'\sigma^2(R) + \mathrm{diag}(\sigma^2(e_1),\sigma^2(e_2),\cdots,\sigma^2(e_n)) \\
&= \vec{\beta}\vec{\beta}'\sigma^2(R) + \Sigma_e
\end{align}$$

If we consider a portfolio $$P$$ with weights $$w_1,w_2,\cdots,w_n$$ for the
assets, due to the linearity of expectation, we have

$$\begin{align}
\alpha_P  &= \sum_{i=1}^n w_i\alpha_1 \\
\beta_P   &= \sum_{i=1}^n w_i\beta_1 \\
E[R_P(t)] &= \alpha_P + \beta_PE[R(t)] \\
\sigma^2(P) &= \vec{w}'\Sigma\vec{w} = \vec{w}' (\vec{\beta}\vec{\beta}'\sigma^2(R) + \Sigma_e) \vec{w} \\
&= \left(\sum_{i=1}^n w_i\beta_i\right)^2 \sigma^2(R) + \sum_{i=1}^n w_i^2\sigma^2(e_i)
\end{align}$$

so if we have even weights $$w_i=1/n$$, then

$$\begin{align}
\sigma^2(P) &= \vec{\beta}^2\sigma^2(R) + \frac{1}{n}\vec{\sigma}^2(\vec{e}) \\
\lim_{n\to\infty} \sigma(P) &= \vec{\beta}\sigma(R)
\end{align}$$

This gives $$\sigma(e_i)$$ the non-market risk (aka unsystematic risk, unique
risk, or residual risk) and $$\beta_i\sigma(R)$$ is the market risk (aka
undiversifiable risk) of equity $$i$$. The coefficient $$\beta_i$$ is the
measure of the contribution of equity $$i$$ to the risk of the portfolio.

# APT (Arbitrary Pricing Theory)

Axioms:

- Investors are risk-averse and seek to maximise their wealth
- There is a risk-free rate for lending and borrowing
- No market friction, no arbitrage opportunities

The model describes the return $$R_i(t)$$ of asset $$i$$ at time $$t$$ in terms
of multiple indices:

$$ R_i(t) = a_i + \sum_{j=1}^J b_{ij}I_j(t) + e_i(t) $$

where $$I_j(t)$$ is the value of index $$j$$ at $$t$$, $$a_i$$ is the alpha of
asset $$i$$, and $$b_{ij}$$ is the beta (sensitivity) of asset $$i$$ to the index
$$j$$. The indices used can be unanticipated chyanges in inflation, bond
spreads, slope and level of interest rates, or industry production.

Considering a portfolio with weights,

$$\begin{align}
R_p &= \sum_{i=1}^n w_iR_i \\
    &= \sum_{i=1}^n w_i\left(a_i + \sum_{j=1}^J b_{ij}I_j + e_i\right) \\
    &= \sum_{i=1}^n w_ia_i + \sum_{i=1}^n\sum_{j=1}^J w_ib_{ij}I_j + \sum_{i=1}^n w_ie_i \\
	&= a_P + \sum_{j=1}^J b_{Pj}I_j + e_P \\
E[R_p] &= a_P + \sum_{j=1}^J b_{Pj}E[I_j]
\end{align}$$

where $$a_P,b_{Pj},e_P$$ are weighted sum of $$a_i,b_{ij},e_i$$ respectively.

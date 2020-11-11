---
layout: post
title: "Chatterjee (2015) Modeling credit risk"
date: Tue, 10 Nov 2020 20:08:40 -0500
category: paper
bibtex:
   type: techreport
   author: "Somnath Chatterjee"
   title: "Modeling credit risk"
   institution: "Bank of England"
   address: "London"
   howpublished: "Centre for Central Banking Studies Handbooks"
   url: www.bankofengland.co.uk/education/ccbs/handbooks_lectures.htm
   year: 2015
---

This is a concise paper describing the Vasicek model. It serves the purpose of
describing how banks should evaluate risk on loan assets, and provides a
connection between the Black-Scholes option pricing model to pricing bonds or
loans.

It starts with the geometric Brownian motion model, which we can describe the
price movement of a stock with the following:

$$
\begin{align}
dS_t &= \mu S_t dt + \sigma S_t dW_t \\
S_t &= S_0 \exp((\mu-\frac12\sigma^2)t + \sigma W_t) \\
&= S_0 \exp((\mu-\frac12\sigma^2)t + \sigma \sqrt{t} Z)
\end{align}
$$

The Vasicek model describes the total asset value of an obligor as gBM as well,
but the Brownian motion driving the model is a combination between two other
Brownian motion:

$$X_t = S\sqrt{\rho} + Z_i\sqrt{1-\rho}$$

which $$S$$ is the systematic risk and $$Z_i$$ is the idiosyncratic risk, which
only the former is shared across the same industry or market while the latter
is specific to the obligor. The default probability of two obligors are related
by the systematic risk, precisely, the covariance of their $$X_t$$ is $$\rho$$.

The convention of notation makes $$X_t\sim N(0,1)$$ and the asset price
movement is described by

$$ A_t = A_0 \exp((\mu-\frac12\sigma^2)t + \sigma \sqrt{t} X_t) $$

we assume the obligor defaults when its asset falls below some threshold $$B$$
at time $$T$$, i.e.

$$\Pr[A_T < B] = \Pr[X_T < c] = \Phi(c) = p^*$$

which $$c$$ is derived from the gBM $$A_t$$ and threshold $$B$$, and $$p^*$$ is
the unconditional probability of default, a.k.a. through-the-cycle average
loss. The quantity $$c<0$$ is the distance of current asset value from the
default threshold, in terms of standard deviations. For a pool of loans, the
correlation of defaults is due to the correlation between assets $$A_i$$ and
$$A_j$$, i.e. the systematic factor $$S$$.  If all obligors are correlated to
each other in the same factor $$\rho$$, we call the pool equi-correlated.

Besides the unconditional probability of default, we can also have the
probability of default conditional on $$S$$ if we fixed $$p^*$$:

$$
\begin{align}
\Pr[A_i(T)<B_i] &= \Pr[X_i<c_i]=\Pr[X_i<\Phi^{-1}(p^*)] \\
&= \Pr[S\sqrt{\rho}+Z_i\sqrt{1-\rho}<\Phi^{-1}(p^*)]\\
&=\Pr[Z_i<\frac{c_i-S\sqrt{\rho}}{\sqrt{1-\rho}}] \\
&=\Phi\left(\frac{\Phi^{-1}(p^*)-S\sqrt\rho}{\sqrt{1-\rho}}\right)
\end{align}
$$

This is the loss subject to credit condition $$S$$, and denoted by $$p(S)$$, which we can further find its pdf as:

$$
\begin{align}
\Pr[p(S)<x] &=\Pr[S>p^{-1}(x)] =\Phi(-p^{-1}(x))\\
&=\Phi\left(\frac{\sqrt{1-\rho}\Phi^{-1}(x)-\Phi^{-1}(p^*)}{\sqrt{\rho}}\right)
\end{align}
$$

Because of this, the KMV model suggested the relationship between a company's
liability (debts) and the equity (stock value). It considers that if the face
value of the debt is $$D_t$$ to repaid at time $$t$$, and the asset is modeled
with $$A_t$$, then:

- Equity can be considered as the call option on the asset $$A_t$$ with strike at $$D_t$$
- Debt can be considered as the risk-free debt of the same amount and shorting a put option on $$A_t$$ with strike $$D_t$$

so according to the Black-Schole formula, we can derive the value of equity
$$E$$ and debt $$B$$:

$$
\begin{align}
E &=A_t\Phi(d_1)-D_te^{-rt}\Phi(d_1-\sigma_A t) \\
B &= De^{-rt}-[De^{-rt}\Phi(-d_1-\sigma_A t)-A_0\Phi(-d_1)] \\
&= De^{-rt}-P_t
\end{align}
$$

which the volatility of equity and asset are related as
$$\sigma_E E = \frac{\partial E}{\partial A}\sigma_A A$$.

If we assume all those debts are bonds, then the bond's yield to maturity is

$$y = \frac{\ln(D_t/B)}{t}$$

and the difference $$y-r$$ is the credit spread to compensate for the default risk.

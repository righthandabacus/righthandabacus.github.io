---
layout: post
title: "Das & Stein (2011) Differences in Tranching Methods"
date: 2018-12-31
category: paper
bibtex:
  type: unpublished
  title: "Differences in Tranching Methods: Some Results and Implications"
  author: "Ashish Das and Roger M. Stein"
  institution: "Moody's Research Labs"
  month: Jan
  year: 2011
  url: https://www.moodys.com/sites/products/ProductAttachments/Differences%20in%20Tranching%20Methods%20-%20Some%20Results%20and%20Implications.pdf
---

There are two methods the rating agencies used to evaluate credit worthiness.
S&P and Moody's in particular, uses the probability of default (PD) and the
expected loss (EL) respectively. This paper compares these two methods in the
context of CDO.

While default is not a legal nor contractual term in CDO context, in contrast
to loans, the term is used to denote the economic loss of a tranche. A tranche
in CDO is subject to credit enhancement (CE), a.k.a. subordination. This paper
compares whether using PD or EL will be more conservative, i.e., results in a
higher level of CE. A tranche in this paper is defined by the attachment and
detachment points ($$A,D$$) to collateral losses ($$L$$), all in the range of 0
to 100%.

The attachment $$A$$, detachment $$D$$, collateral loss distribution function
$$f(L)$$, and the tranche's PD or EL are related. Usually a tranche is designed
to target for a specific PD or specific EL, with some predefined collateral
loss distribution assumed. Sec 3 describes their relationship:

For PD-based tranching,

$$PD = P(L > A) = \int_A^1 f(L)dL = 1 - F(A)$$

and usually select $$A$$ such that $$PD \le PD_T$$, which $$PD_T$$ is the
target PD for the tranche. The integral find the probability that the collateral
loss is above the attachment point, which the tranche receives at least some
loss. The CE of the tranche is equal to the collateral portfolio credit VaR
with $$\alpha = PD_T$$. (consider that VaR of $$X$$ at confidence level
$$\alpha$$ is $$\mathrm{VaR}_{\alpha}(X) = \inf\{x: F_X(x)>\alpha\}$$, so the
credit enhancement is the $$(1-PD_T)$$-th quantile of the loss distribution
function)

It is obvious that the PD-based tranching is oblivious (insensitive) to the
tranche thickness $$D-A$$. It is also insensitive to the severity of loss.

On the other hand, in case of collateral loss $$L\in[A,D]$$, the percentage
loss of a tranche is

$$L_T = \min\left(1, \max(0, \frac{L-A}{D-A}) \right)$$

i.e., linear to the loss $$L$$. With this, the EL-based tranching will have

$$EL = \int_0^1 L_T f(L) dL = \int_0^1 \min\left(1, \max(0, \frac{L-A}{D-A}) \right) f(L) dL$$

Here we see that EL depends on the thickness $$D-A$$ -- with the same
attachment point $$A$$, the thinner the tranche, the higher the EL.

The paper shows the mechanics to define tranches in a CDO. Assume the target
expected loss for the most senior tranche is $$EL_{Sr}$$, we have its CE as

$$CE_{Sr} = \min\left\{A : EL_{Sr}\ge\int_0^1 \min\left(1, \max(0, \frac{L-A}{1-A}) \right) f(L) dL \right\} $$

Here we substituted $$D=1$$ for the detachment point of the most senior tranche
is by definition at 100% level.  After we solve for such lowest value of $$A$$
numerically, we can proceed to the senior-most mezzanine tranche (with
$$EL_M$$ as EL and detachment point as $$CE_{Sr}$$):

$$CE_M = \min\left\{A : EL_M\ge\int_0^1 \min\left(1, \max(0, \frac{L-A}{CE_{Sr}-A}) \right) f(L) dL \right\} $$

and we can compute the other tranches down the waterfall iteratively. But note
that, the EL of tranches in the CDO cannot be arbitrary because the detachment
point of each tranche is determined by the more senior tranches. The following
derivation is provided in sec 6, which says the achievable target EL is
lower-bounded by $$1-F(D)$$:

$$\begin{align}
EL &= \int_A^D \frac{L-A}{D-A} f(L) dL + (1-F(D))
&ge 1-F(D)
\end{align}$$

There is no way to convert a PD-based tranching into EL-based tranching, even
if we assume collateral LGD (loss given default, i.e. severity) is always 1. To
see this, we note that tranche PD determined by $$A$$ solely but only EL will
take into account the level of $$D$$.

The key point of the paper is in sec 4. The authors argue that regardless of the
collateral loss distribution, tranche EL can be increased by decreasing the
attachment or detachment point. The reasoning is as follows. Consider a fixed
$$D$$ and the tranche attachment point *lowered* from $$A'$$ to $$A$$. The
tranche EL at $$A'$$ is computed using the above-mentioned integral

$$\begin{align}
EL_{A',D} &= \int_0^1 \min\left(1, \max(0, \frac{L-A'}{D-A'}) \right) f(L) dL \\
&= \int_0^A 0 f(L) dL + \int_A^{A'} 0 f(L)dL + \int_{A'}^D \frac{L-A'}{D-A'} f(L) dL + \int_D^1 1 f(L)dL \\
&= \int_{A'}^D \frac{L-A'}{D-A'}) f(L) dL + (1-F(D))
\end{align}$$

and the tranche EL at $$A$$ is similarly,

$$\begin{align}
EL_{A,D} &= \int_0^1 \min\left(1, \max(0, \frac{L-A}{D-A}) \right) f(L) dL \\
&= \int_0^A 0 f(L) dL + \int_A^{A'} \frac{L-A}{D-A} f(L)dL + \int_{A'}^D \frac{L-A}{D-A} f(L) dL + \int_D^1 1 f(L)dL \\
&= \int_A^{A'} \frac{L-A}{D-A} f(L)dL + \int_{A'}^D \frac{L-A}{D-A} f(L) dL + (1-F(D))
\end{align}$$

Since

$$\frac{L-A}{D-A} = 1 - \frac{D-L}{D-A}$$

so $$\frac{L-A}{D-A}$$ decreases as $$A$$ increases, i.e.

$$\begin{align}
EL_{A,D}
&= \int_A^{A'} \frac{L-A}{D-A} f(L)dL + \int_{A'}^D \frac{L-A}{D-A} f(L) dL + (1-F(D)) \\
&\ge \int_A^{A'} \frac{L-A}{D-A} f(L)dL + \int_{A'}^D \frac{L-A'}{D-A'} f(L) dL + (1-F(D)) \\
&\ge \int_{A'}^D \frac{L-A'}{D-A'} f(L) dL + (1-F(D)) = EL_{A',D}
\end{align}$$

The physical meaning of this is that given the same collateral loss $$L$$, the
lower the attachment point the more the tranche loss will be.

Similarly, we can also show that decreasing $$D$$ while $$A$$ fixed will have
tranche EL increased. This is intuitive from the fact that lowering the
detachment point, the tranche loss for the same level of collateral loss $$L$$
is higher.

As EL will be lower when $$A$$ increased but higher when $$A$$ decrease, it is
not clear qualitatively the relationship of a thinner tranche to the tranche
EL. In other words, *a thicker tranche is not necessarily less risky*.

Then the paper proceed to discuss the tranche's expected loss given default
(LGD). It is known that for collateral loss $$L\in[A,D]$$, the percentage loss
to the tranche is $$\frac{L-A}{D-A}$$. So the LGD is given by the
probability-weighted tranche loss over the probability of loss beyond the
attachment point (which is also the tranche PD), i.e.,

$$LGD = \frac{\int_A^D\frac{L-A}{D-A}f(L)dL + \int_D^1 f(L)dL}{\int_A^1 f(L)dL}$$

In the case of the most senior tranche

$$LGD = \frac{\int_A^1\frac{L-A}{1-A}f(L)dL}{\int_A^1 f(L)dL}$$

Now we assume $$f(L) \ge f(A)$$ for the tail of $$L > A$$ (which is reasonable
for most senior tranche as $$A$$ will not be close to 0). So the LGD of the
most senior tranche is bounded by

$$\begin{align}
LGD &= \frac{\int_A^1\frac{L-A}{1-A}f(L)dL}{\int_A^1 f(L)dL} \\
&\ge \frac{\int_A^1\frac{L-A}{1-A}f(A)dL}{\int_A^1 f(A)dL} \\
&= \frac{\int_A^1\frac{L-A}{1-A}dL}{\int_A^1 dL} \\
&= \frac{1}{(1-A)^2}\int_A^1\frac{L-A}dL \\
&= \frac{1}{2}
\end{align}$$

but in practice the LGD is much smaller than 0.5 due to the tail of the
distribution decreases and is convex. (Higher the convexity at beyond $$A$$,
the lower the LGD) The paper than uses a hypothetical loss distribution modeled
as a $$t$$-copula with 4 d.o.f., assume collateral assets with PD of 0.5%, 1%,
and 2%, correlation assumptions of $$\rho=$$ 5%, 15%, 25%, and a target tranche
EL of 0.05% for the most senior tranche. It turns out the LGD is well below
50%. The resulting attachment point and the tranche LGD are positively
correlated, for the lower the loss given default the more tolerance to default
at a given constant target EL.

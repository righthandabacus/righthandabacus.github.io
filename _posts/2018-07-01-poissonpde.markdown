---
layout: post
title: Stochastic PDE for Poisson jump processes
date: Sun, 01 Jul 2018 22:31:19 -0400
tags: math
---

I studied this 10+ years ago. But I just saw a very concise derivation from [stack exchange](https://quant.stackexchange.com/questions/18589/itos-formula-for-jump-process/18592#18592)

Let $$N_t$$ be a Poisson counter process, with parameter $$\lambda>0$$. Let
there be another stochastic process $$X_t = f(t,N_t)$$. Then, by Taylor series
expansion,

$$\begin{align}
dX_t &= \frac{\partial X_t}{\partial t}dt + \frac{\partial X_t}{\partial N_t}dN_t \\
     &\quad  + \frac{1}{2!}\left(
            \frac{\partial^2 X_t}{\partial N_t^2}(dN_t)^2
            + 2\frac{\partial^2 X_t}{\partial N_t\partial t}dN_tdt
            + \frac{\partial^2 X_t}{\partial t^2}(dt)^2
        \right) \\
     &\quad + \frac{1}{3!}\left(
            \frac{\partial^3 X_t}{\partial N_t^3}(dN_t)^3
            + 3\frac{\partial^3 X_t}{\partial N_t^2\partial t}(dN_t)^2dt
            + 3\frac{\partial^3 X_t}{\partial N_t\partial t^2}dN_t(dt)^2
            + \frac{\partial^3 X_t}{\partial t^3}(dt)^3
        \right) + \cdots \\
     &= \frac{\partial X_t}{\partial t}dt + \left(\frac{\partial X_t}{\partial N_t} + \frac{1}{2!}\frac{\partial^2 X_t}{\partial N_t^2} + \frac{1}{3!}\frac{\partial^3 X_t}{\partial N_t^3} + \cdots \right)dN_t
\end{align}$$

The second equality is due to the fact that $$dN_tdt = 0$$, and $$dN_t = (dN_t)^2 = (dN_t)^3 = \cdots = (dN_t)^n$$ since $$dN_t \in \{0, 1\}$$.

Therefore, we have

$$
dX_t = \frac{\partial X_t}{\partial t}dt + \left(\sum_{n=1}^{\infty}\frac{1}{n!}\frac{\partial^n X_t}{\partial N_t^n}\right)dN_t
$$

For example, if

$$ X_t = \left(\frac{\eta}{\lambda}\right)^{N_t} e^{(\lambda-\eta)t} $$

then

$$\begin{align}
\frac{\partial X_t}{\partial t} &= (\lambda-\eta)X_t \\
\frac{\partial^n X_t}{\partial N_t^n} &= \left[\log\left(\frac{\eta}{\lambda}\right)\right]^n X_t \\
\therefore\ dX_t &= \frac{\partial X_t}{\partial t}dt + \left(\sum_{n=1}^{\infty}\frac{1}{n!}\frac{\partial^n X_t}{\partial N_t^n}\right)dN_t \\
&= (\lambda-\eta)X_t dt + \left(\sum_{n=1}^{\infty}\frac{1}{n!} \left[\log\left(\frac{\eta}{\lambda}\right)\right]^n X_t \right) dN_t \\
&= (\lambda-\eta)X_t dt + \left(\exp\left(\log\left(\frac{\eta}{\lambda}\right)\right)-1\right) X_t dN_t \\
&= (\lambda-\eta)X_t dt + \left(\frac{\eta}{\lambda}-1\right) X_t dN_t
\end{align}$$

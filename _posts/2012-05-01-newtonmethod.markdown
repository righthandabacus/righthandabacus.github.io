---
layout: post
title: "Newton's method"
date: 2012-05-01 12:39:14 -0400
tags: math
---

To numerically find a root of $f(x)=0$, we may use the Newton's method. Assuming
the root is at the proximity of $x_k$, to find a better approximate $x_{k+1}$,
we consider the tangent line at $(x_k, f(x_k))$, which is provided by the
equation

$$
  y-f(x_k) = f'(x_k)(x-x_k).
$$

Thus the $x$-intercept of such tangent, $(x_k-\frac{f(x_k)}{f'(x_k)},0)$, reveals
an approximation closer than $x_k$, i.e. the formula for Newton's method is

$$
  x_{k+1} = x_k - \frac{f(x_k)}{f'(x_k)}.
$$

In case of a function $f: \mathbb{R}^n \mapsto \mathbb{R}$, we may find its zero
by the following. At an approximated zero $\vec{x}_k$,

$$
  y-f(\vec{x}_k) = \nabla f|_{\vec{x}_k}(\vec{x}-\vec{x}_k)^T
$$

is a hyperplane tangent to $f$ at $\vec{x}_k$, so the refinement would be

$$
  \vec{x}_{k+1} = \vec{x}_k - \frac{f(\vec{x}_k)}{|\nabla f(\vec{x}_k)|^2} \nabla f(\vec{x}_k).
$$

For a more general function, $f: \mathbb{R}^n \mapsto \mathbb{R}^m$, at an
approximated zero $\vec{x}_k$, the tangent hyperplane is

$$
  \vec{y} - f(\vec{x}_k) = \mathrm{J}f(\vec{x}_k) (\vec{x}-\vec{x}_k)^T,
$$

where $\mathrm{J}$ is the Jacobian operator, which if

$$
f(\vec{x}) = \begin{bmatrix}
    y_1(x_1,\cdots,x_n)  \\
    y_2(x_1,\cdots,x_n)  \\
    \vdots
    \\ y_m(x_1,\cdots,x_n)
\end{bmatrix}$$

then

$$
\mathrm{J}f = \begin{bmatrix}
    \frac{\partial y_1}{\partial x_1} & \frac{\partial y_1}{\partial x_2} & \cdots & \frac{\partial y_1}{\partial x_n}  \\
    \frac{\partial y_2}{\partial x_1} & \frac{\partial y_2}{\partial x_2} & \cdots & \frac{\partial y_2}{\partial x_n}  \\
    \vdots & \vdots & & \vdots  \\
    \frac{\partial y_m}{\partial x_1} & \frac{\partial y_m}{\partial x_2} & \cdots & \frac{\partial y_m}{\partial x_n}
\end{bmatrix}.$$

So the formula would be

$$
\vec{x}_{k+1} = \vec{x}_k - [\mathrm{J}f(\vec{x}_k)]^{-1}[f(\vec{x}_k)]^T
$$

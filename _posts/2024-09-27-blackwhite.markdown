---
layout: post
title: Black and White
date: 2024-09-27
tags: math
category: blog
---

There is no black and white. Human perceived black for no visible light, and
some composition of light wavelengths is perceived as white. To measure the
grayscale, we want to quantify what is black and what is white.

One way to quantify whiteness is to consider the "brightness" as used by paper
industry: Consider a scale of 0 to 100, the percentage of light reflected is
the whiteness. The best off-the-shelf paper available seems to be 98 bright.
Next to that, there are 97 to 92 bright. It is not whiteness, which is the hue
of color. Measuring brightness would be more objective.

Not sure how this brightness should be measured. But I found the standard ISO
11475:2017 titled "Paper and board — Determination of CIE whiteness, D65/10
degrees (outdoor daylight)". That should be related.

So how about black? There's a NATO report [STO-MP-SET-277](https://www.sto.nato.int/publications/STO%20Meeting%20Proceedings/STO-MP-SET-277/MP-SET-277-12.pdf)
titled "Analyzing Reflectance Data for Various Black Paints and Coatings" by
Mimi Huynh. This report is more than just visible light. But from figure 12,
if we focus on the 400nm to 700nm range, the best reflection of black paint is around 3%.

However, percentage of reflection and human perception are not the same. There's
[Weber-Fechner law](https://en.wikipedia.org/wiki/Weber%E2%80%93Fechner_law)
from psychophysics, which says human sensation increases in log scale of energy.
Now, let's say the best paper white and best black paint are two extremes. If
the light source emits light at $X$ candela, the best white reflects $0.98X$
and the best black reflect $0.03X$, the middle would be the *geometric mean* of
them (i.e., arithmetic mean after logarithm, due to the log scale required),
or 17% reflective.

That's the "18% gray" rule for what is the perceptually uniform middle gray,
which is the point of measurement for light metering in photography. For
brightness from 0 to 100% with 18% as the middle ground, we found that there
are only 5 stops possible.

Let's consider what is a perceptually uniform gray scale. The CIELAB color
space defined the luminance as an absolute measure in cd/m² (linear). The
"L*a*b*" counterpart space is the nonlinear conversion to reflect the human
vision perceived response. If color, we have to consider the luma (i.e., Y
in the YUV color space). That is more like a weighted sum of R, G, and B
intensity (after gamma correction if considering CRT monitor) reflecting the
[luminous efficiency](https://en.wikipedia.org/wiki/Luminous_efficiency_function)
such as blue is darker than green.

To calculate the perceived lightness from RGB values ($V\in[0,1]$), firstly, we
*linearize* the value:

$$
V' = \begin{cases}
V/12.92 & \text{if } V \le 0.04045 \\
(\frac{V+0.055}{1.055})^{2.4} & \text{if } V > 0.04045
\end{cases}
$$

Then the luma ($Y\in[0,1]$, or "tone level") is a weighted sum:

$$
Y = 0.2126 R' + 0.7152 G' + 0.0722 B'
$$

and the luma is converted into perceived lightness ($L^\ast\in[0,100]$) in a cube-root relationship:

$$
L^\ast = \begin{cases}
903.3 Y & \text{if } Y \le 0.008856 \\
116 Y^{1/3} - 16 & \text{if } Y > 0.008856
\end{cases}
$$

Perceived lightness is in cube root of light intensity, not logarithmic, due to
modern revision. But in these formulae, you get $L^\ast=50 \to Y=0.18$. Since we
know grayscale has RGB value all equal, hence the $Y$ formula above can be
skipped as the sum of all coefficients is 1, and make $Y=V'$.

Reversely, from $L^\ast$, we can compute the gamma-corrected RGB value (i.e.,
sRGB) of a shade of gray:

$$
\begin{aligned}
    Y &= \begin{cases}
        \dfrac{L^\ast}{903.3} & \text{if }L^\ast \le 8 \\
        \big(\dfrac{L^\ast+16}{116}\big)^3 & \text{if }L^\ast > 8
        \end{cases} \\
    V &= \begin{cases}
        12.92 Y & \text{if }Y\le 0.003131 \\
        1.055 Y^{5/12} - 0.055 & \text{if } Y > 0.003131
        \end{cases}
\end{aligned}
$$


See also:

- https://stackoverflow.com/questions/596216/
- https://xtremestuff.net/where-in-the-world-is-middle-gray/

---
layout: post
title: Creating a command line program automatically from a Python function
date: Sat, 08 Oct 2022 13:31:28 -0400
tags: code
category: blog
highlightjs:
  - python
---

Undeniably Python is quite idiot-friendly language that you don't need too much
learning to get something done. A lot of code out there to copy and the concise
language helped too. While writing a Python function is easy, it seems quite a
lot of boilerplates needed additionally to make a command line tool. But it
turns out, with a good coding style, something can be done.

Python is object-oriented language. Everything in Python is an object,
including functions. Since long time ago, we can extract the details of a
function including its name, the arguments, and which argument has default
value assigned.

In the newer Python, we can add type hints to variables, as well as arguments
to functions. While it is a hint, which does not affect the language execution,
we can sort-of declaring what type we expect for an argument of a function.

Also there are docstrings. It is the string put immediately inside the function
which for consumption of documentation generation tools. I prefer to use the
"Google style" docstring for saving screen real estate. The "numpy style"
docstring, however, is more verbose and more robust.

Assume we have a function declared with type hints, and carefully crafted
docstring, like the following:

```python
def quadratic(a: float, b: float, c: float, x: float=None, give_roots: bool=False):
    """Evaluating a quadratic polynomial and find its root
    Parameters
    ----------
    a : float
        Coefficient of x^2 term
    b : float
        Coefficient of x term
    c : float
        Coefficient of constant term
    x : float, optional
        Value that the quadratic polynomial should evaluate on
    give_roots : bool, optional
        If True, roots of the polynomial will be returned as well
    Returns
    -------
    A dictionary containing the value of the polynomial evaluated (if provided)
    as well as the roots
    """
    ret = {}
    # evaluate polynomial
    if x:
        ret["value"] = ((a*x) + b)*x + c
    # find roots
    if give_roots:
        det = b*b - 4*a*c
        first = -b / (2*a)
        if det < 0:
            second = cmath.sqrt(det) / (2*a)
        else:
            second = math.sqrt(det) / (2*a)
        ret["roots"] = [first + second, first - second]
    return ret
```

And we want to be able to make a script that can be called using:

    python quad.py -a 3 -b -2 -c 5 -x 0 --give_roots true

This is possible to be done automatically. First, based on the function, we can
read what are the arguments using `func.__code__.co_varnames` and whether the
argument has any default assigned using `func.__defaults__`. Also we can read
the type hints using `typing.get_type_hints(func)`. The docstring is a bit
difficult because you need to parse its content. If its is nicely written
following some style, the `docstring_parser` module is available for help.

Now what's left is how we can build the `add_argument()` calls from an
`ArgumentParser` object. Roughly it is about these:

- the short form argument, e.g., `-g`
- the long form argument, e.g., `--get_roots`
- the help text for the argument
- any defaults for it
- is it a required argument
- the type of the argument (so argparse will automatically give us the correct type instead of strings)

We can make the long form the same as the functions argument: Usually the long
form accepts wider range of identifiers such as containing a dash but any
Python identifier can be an argument well. The short form can be derived from
the long one.

The help text is not part of the function but we can infer that from docstring.
The `docstring_parser` module will produce an object for the docstring parsed,
and we can check if the variable is mentioned in it.

Telling if the argument has default need to be inferred from
`func.__defaults__` if it is a non-keyword-only arguments or
`func.__kwdefaults__` for keyword-only arguments. If a function takes an
argument and no default is assigned, it is a required argument.

The tricky part is the types of the arguments. Normally, the type hint will
give you the type (e.g., when we specifies `a: float`). But when we have
`x: float=None`, we are not going to see `float` as the type for `x` but
`typing.Optional[float]` instead. We need to extract `float` from such, which
we can get it from `typing.get_args()`.

The full code is as in gist, below. Note that it does not catch all corner
cases, such as more complex type hints (e.g., list of floats).

<script src="https://gist.github.com/righthandabacus/2e548e65f479b819603163856468c9d5.js"></script>

---
layout: post
title: Keyword-only arguments, positional-only arguments, and Python decorators
date: Tue, 27 Sep 2022 09:14:58 -0400
tags: code
category: blog
highlightjs:
  - python
---

In Python 3, we can define a function with keyword-only arguments and in Python
3.8, we can further define with positional-only arguments. The syntax is easy, as follows:

Syntax:

```python
def func(x, /, y, z=0, *, a, b):
    ...
```

which, the `*` signifies whatever afterwards are specified as keywords only.
And the `/` signifies whatever before are positional only. This means the
function defined above cannot be invoked with:

```python
func(x=3)
func(3, 4, 5, 6, 7)
```

for the first violated the requirement that argument `x` must be positional
only and the second violated that `a` and `b` must be keyword only. These
works, however:

```python
func(3, 4, 5, a=6, b=7)
func(3, 4, z=5, a=6, b=7)
```

because argument `z` (also `y`) can be specified either as positional or
keyword argument

In this definition, `z` has a default value. We can legally define a function
without default values for `a` and `b` because they are keyword only.
Otherwise, we must specify their default value for them if we provided one for
`z` since they *can be positional*.

While we do not provide a default for the two keyword-only arguments, they are
not required. That is, we cannot invoke with

```python
func(3, 4, a=5)
```

but must provide *both* as

```python
func(3, 4, a=5, b=6)
```

Why we need them? One example is to make decorators. Consider a decorator that
can be used differently, such as `functools.lru_cache`:

```python
@lru_cache
def my_function():
   ...

@lru_cache(maxsize=32):
def my_other_function():
   ...
```

This decorator can be invoked using `@lru_cache` as well as
`@lru_cache(maxsize=32)`. Recall that a decorator in Python simply means

```python
def my_function():
   ...
def my_other_function():
    ...

my_function = lru_cache(my_function)
my_other_function = lru_cache(maxsize=32)(my_other_function)
```

So in the first example, `lru_cache` is a function that takes another function
as argument, and in the second example, `lru_cache(maxsize=32)` returns a
function that does the same. If we look at the code, it is indeed implemented
this way:

```python
def lru_cache(maxsize=128, typed=False):
    if isinstance(maxsize, int):
        # Negative maxsize is treated as 0
        if maxsize < 0:
            maxsize = 0
    elif callable(maxsize) and isinstance(typed, bool):
        # The user_function was passed in directly via the maxsize argument
        user_function, maxsize = maxsize, 128
        wrapper = _lru_cache_wrapper(user_function, maxsize, typed, _CacheInfo)
        wrapper.cache_parameters = lambda : {'maxsize': maxsize, 'typed': typed}
        return update_wrapper(wrapper, user_function)
    elif maxsize is not None:
        raise TypeError(
            'Expected first argument to be an integer, a callable, or None')

    def decorating_function(user_function):
        wrapper = _lru_cache_wrapper(user_function, maxsize, typed, _CacheInfo)
        wrapper.cache_parameters = lambda : {'maxsize': maxsize, 'typed': typed}
        return update_wrapper(wrapper, user_function)

    return decorating_function
```

So some `isinstance()` and `callable()` is used to see if `lru_cache` is used
as a decorator or it is used as `lru_cache(maxsize=32)`.

We can indeed write in this way (which we assume we forbid the usage of
`lru_cache(32)` as decorator):

```python
def lru_cache(func=None, /, *, maxsize=128, typed=False):
    def decorating_function(user_function):
        wrapper = _lru_cache_wrapper(user_function, maxsize, typed, _CacheInfo)
        wrapper.cache_parameters = lambda : {'maxsize': maxsize, 'typed': typed}
        return update_wrapper(wrapper, user_function)

    if func is None:
        return decorating_function
    else:
        return decorating_function(func)
```

The way it works is as follows: If `@lru_cache` is used as decorator, it will be
invoked with the function to decorate as positional argument. But if
`@lru_cache(maxsize=32)` is used as decorator, we assume a decorator should
return as a result of `lru_cache(maxsize=32)`.

---
layout: post
title: Python docstring coding standards
date: Sun, 18 Feb 2018 13:37:13 -0500
tags: code
---

Everyone comes up with their own coding standard. Hence we have standards
proliferate, <https://xkcd.com/927/>:

![](https://imgs.xkcd.com/comics/standards.png)

In terms of Python docstrings, we have

- Google style
- NumPy style
- Epytext format (javadoc-like style)
- ReST

Below are some code examples:

## Epytext

```python
def myfunction(x, y):
	"""
	Javadoc style

	@param x: this is x
	@param y: this is y
	@return: this function returns zero
	@raise keyError: blah
	"""
	return 0
```

## ReStructuredText

```python
def myfunction(x, y):
	"""
	ReStructuredText as used by Sphinx

	:param x: this is x
	:param y: this is y
	:returns: this function returns zero
	:raises keyError: blah
	"""
	return 0
```

Sphinx use RST for documentation by default. And it allows more elaborate docs in the doc string, for example the below:

```python
def myfunction(x,y):
    """
    This is a short description

    **Section title**
    more description here

    :param x: this is x

    :Example:

    >>> import sys
    >>> a = myfunction(1,2)

    .. seealso:: :class:`MyClassOverThere`
    .. todo:: something more to do
    """
    return 0
```

## Google coding standard

```python
def myfunction(x, y):
	"""
	This is how Google do it

	Args:
		x: This is x
		y (int): This is y, with type specified

	Returns:
		int: This function returns zero

        Always, returns zero

	Raises:
		KeyError: blah
	"""
	return 0
```

## NumPy style

```python
def myfunction(x, y, z=None):
	"""
	NumPy format docstring is very verbose

	Parameters
	----------
	x: array_like
		this is `x`
	y:
		the is the second param
	z: {None, 1, 2, 3}, optional
		this is `z`, default None

	Returns
	-------
	integer
		always zero

	Raises
	------
	KeyError
		Blah
	NotImplementedError
		Blah Blah
	"""
	return 0
```

# Reference

- [What is the standard Python docstring format?](https://stackoverflow.com/questions/3898572/what-is-the-standard-python-docstring-format)
- [Napoleon - Marching toward legible docstrings](http://sphinxcontrib-napoleon.readthedocs.io/en/latest/index.html)
   - [example code for Google style](http://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html)
   - [example code for NumPy style](http://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_numpy.html)
- [Sphinx and RST syntax guide: Example on how to document your Python docstrings](https://thomas-cokelaer.info/tutorials/sphinx/docstring_python.html)

---
layout: post
title: A use case of metaclass
date: Wed, 26 Aug 2020 15:25:50 -0400
tags: code
---

There are many algorithms out there that uses some "magic constants". An
example is the S-box in most of the block ciphers, which we cannot possibly
avoid from the implementation. In some languages, we can hide them in a
namespace. For instance in JavaScript, we can put everything inside a pair of
braces, which not only confine the scope of the identifiers defined, but also
presenting their logical groupping. Python, however, do not have such option
unless we put it in a separate module.

One alternative in Python is `class`. We can put all such nuts and blots in a
class. But doing so means we have a different syntax, like this:

```
classobj = MyClass()
classobj.function(x)
```

Can I just call the class method without creating a class object? It turns out
not so intuitive. A python class has a special method `__call__` that can be
overridden such that a class object can be invoked just like a function:

```python
class CallMe:
    delimiter = "$"
	def __call__(self, message):
		print(self.delimiter + str(message) + self.delimiter)

x = CallMe()
x("foo")  # print $foo$
```

This is useful if the class is a factory that produces a *callable* that needs
to be initialized. If you have nothing to initialize, that line `x = CallMe()`
seems to be redundant. We cannot expect to do `CallMe("foo")` instead because
that looks really like invoking the constructor -- and a constructor means an
object is created.

This is how metaclass arises. If we want to modify the language behavior of a
class, we have to do that in its metaclass. In this case, we want to override
the constructor such that `CallMe("foo")` does not mean to create an object.
This is what to do:

```python
class Meta(type):
	def __call__(cls, message):
		print(cls.delimiter + str(message) + cls.delimiter)

class CallMe(metaclass=Meta):
	delimiter = "$"

CallMe("foo")  # print $foo$
```

Before Python 3, the way to specify metaclass is to define class variable
`__metaclass__ = Meta` inside the class `CallMe`. This usage is removed and we
have to specify as above. By specifing the `__call__` function in the
metaclass, the behavior of `CallMe("foo")` is modified. And as the `__call__`
function does not return an object of class `cls`, the class `CallMe` will not
be incarnate. Note that the first parameter of `__call__` is `cls` to signal
that functions in the metaclass is always dealing with the class type, not the
class object.

Indeed, we can avoid the metaclass:

```python
class CallMe:
	delimiter = "$"
	def __new__(self, message):
		print(self.delimiter + str(message) + self.delimiter)

CallMe("foo")  # print $foo$
```

This is another way to override the constructor in Python. Behavior will be the
same as the approach using metaclass. Actually for the expression
`CallMe("foo")`, the order of resolution will be the `__call__` of metaclass
take the highest precedence, then `__new__` of the calss, then `__init__` of
the class as a fallback.

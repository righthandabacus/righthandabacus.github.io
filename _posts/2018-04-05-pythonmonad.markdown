---
layout: post
title: Python monad code commentary
date: Thu, 05 Apr 2018 18:37:17 -0400
lastedit: Wed, 11 Apr 2018 11:48:51 -0400
tags: code
---

Wikipedia[^wiki] has a reference link to [an implementation of monad with
Python](http://www.valuedlessons.com/2008/01/monads-in-python-with-nice-syntax.html)
and its code is at the bottom.

As explained[^p06], monad is a triple of objects, (m, unit, bind), which m
is the "amplified type", unit and bind are to amplify type and functions.

This is an example of amplified type m:

```python
class Failable(Monad):
    def __init__(self, value, success):
        self.value   = value
        self.success = success
```

So the simple type is stored as `x.value` while the amplified type is `x`.

Given these, we can run the following (with `Nothing` and `Something` defined as
`Failable` with `self.success` to be `False` and `True` respectively):

```python
def mdiv(a, b):
    if b == 0:
        return Nothing
    else:
        return Something(a / b)

def with_maybe():
    return \
    mdiv(2.0, 2.0)    >> (lambda val1 :
        mdiv(3.0, 0.0)    >> (lambda val2 :
            mdiv(val1, val2)  >> (lambda val3 :
                Something(val3)
            )
        )
    )
```

The `with_maybe` above make use of closure in Python to compute $$\frac{2.0}{2.0} \left/ \frac{3.0}{0.0} \right.$$
and the operator `>>` is a short cut of bind:

```python
class Monad:
    def bind(self, func):
        raise NotImplementedError
    def __rshift__(self, bindee):
        return self.bind(bindee)
```

The function `with_maybe` returns an instance of `Failable` (which is `Nothing`
because of the division by zero in the computation).

The `>>` operator is functional but we have a procedural syntax even in
Haskell. The code below use `yield` to mean `bind` (bind a function result into
a variable) and `mreturn` to "unwrap" the complex type to simple type:

```python
def failable_monad_examle():
    def fdiv(a, b):
        if b == 0:
            return Failure("cannot divide by zero")
        else:
            return Success(a / b)

    @do(Failable)
    def with_failable(first_divisor):
        val1 = yield fdiv(2.0, first_divisor)
        val2 = yield fdiv(3.0, 1.0)
        val3 = yield fdiv(val1, val2)
        mreturn(val3)

    print with_failable(0.0)
    print with_failable(1.0)
```

The code looks clean but it is hard to understand how to works. All magic are
hidden inside the function decorator `do`, defined as follows:

```python
@decorator_with_args
def do(func, func_args, func_kargs, Monad):
    @handle_monadic_throws(Monad)
    def run_maybe_iterator():
        itr = func(*func_args, **func_kargs)

        if isinstance(itr, types.GeneratorType):
            @handle_monadic_throws(Monad)
            def send(val):
                try:
                    # here's the real magic
                    monad = itr.send(val)
                    return monad.bind(send)
                except StopIteration:
                    return Monad.unit(None)

            return send(None)
        else:
            #not really a generator
            if itr is None:
                return Monad.unit(None)
            else:
                return itr

    return run_maybe_iterator()
```

The key to the sequence of `yield` statement to work is the `bind` function. The
`send(val)` function defined a try-except clock that recursively call itself for
each subsequent `yield`. The final `mreturn()` raises the final value which is
handled by `handle_monadic_throws` decorator. Here it is worth to explain the
`yield` construct:

    x = yield y

This statement in a generator will generate value `y` and the instruction
pointer will stay here until either of the following occurs:

1. Another read from the generator, in this case `x` is assigned to `None`,
   proceed until next `yield`, and a new value is read
2. Invoke `send(z)` from the generator, which `x` is assigned to the send argument `z`,
   proceed until next `yield`, and the new value generated is returned

The former case is what we commonly see in Python code. The above is the latter
case, however. In each step, the monad (object of amplified type) is "bind" to the
function itself to read the generator again.

Now this is the last piece for the monad puzzle, the `bind` and `unit` calls:

```python
class Failable(Monad):
    def __init__(self, value, success):
        self.value   = value
        self.success = success
    def bind(self, bindee):
        if self.success:
            return bindee(self.value)
        else:
            return self
    @classmethod
    def unit(cls, val):
        return cls(val, True)
```

When this monad type object is bind to a function, it invoke the bindee only if
it is "success" type, otherwise return itself without calling the bindee.
Therefoe a sequence of bind becomes a chain-of-responsibility that is only success
if all steps are success.

The `unit` class method defined in the monad type is essential for the final
return. In case of `mreturn(x)`, this function converts simple value `x` into
a monad with value assigned to `x`.

# Full code

```python
import types

###### Base Monad and @do syntax#########

class Monad:
    def bind(self, func):
        raise NotImplementedError

    def __rshift__(self, bindee):
        return self.bind(bindee)

    def __add__(self, bindee_without_arg):
        return self.bind(lambda _ : bindee_without_arg())

def make_decorator(func, *dec_args):
    def decorator(undecorated):          # undecorated = decoratee for do()
        def decorated(*args, **kargs):
            return func(undecorated, args, kargs, *dec_args)

        decorated.__name__ = undecorated.__name__
        return decorated

    decorator.__name__ = func.__name__
    return decorator

def make_decorator_with_args(func):      # func = decorator `do`
    def decorator_with_args(*dec_args):  # dec_args = args to do() decorator, the monad type
        return make_decorator(func, *dec_args)
    return decorator_with_args

decorator           = make_decorator
decorator_with_args = make_decorator_with_args

@decorator_with_args
def do(func, func_args, func_kargs, Monad):
    @handle_monadic_throws(Monad)
    def run_maybe_iterator():
        itr = func(*func_args, **func_kargs) # decoratee expects to be GeneratorType

        if isinstance(itr, types.GeneratorType):
            @handle_monadic_throws(Monad)
            def send(val):
                try:
                    # here's the real magic
                    monad = itr.send(val)
                    return monad.bind(send)
                except StopIteration:
                    return Monad.unit(None)

            return send(None)
        else:
            #not really a generator
            if itr is None:
                return Monad.unit(None)
            else:
                return itr

    return run_maybe_iterator()

@decorator_with_args
def handle_monadic_throws(func, func_args, func_kargs, Monad):
    try:
        return func(*func_args, **func_kargs)
    except MonadReturn, ret:             # case of mreturn(x)
        return Monad.unit(ret.value)     # construct monad with value x
    except Done, done:
        assert isinstance(done.monad, Monad)
        return done.monad

class MonadReturn(Exception):
    def __init__(self, value):
        self.value = value
        Exception.__init__(self, value)

class Done(Exception):
    def __init__(self, monad):
        self.monad = monad
        Exception.__init__(self, monad)

def mreturn(val):
    raise MonadReturn(val)

def done(val):
    raise Done(val)

def fid(val):
    return val

##### Failable Monad ######

class Failable(Monad):
    def __init__(self, value, success):
        self.value   = value
        self.success = success

    def __repr__(self):
        if self.success:
            return "Success(%r)" % (self.value,)
        else:
            return "Failure(%r)" % (self.value,)

    def bind(self, bindee):
        if self.success:
            return bindee(self.value)
        else:
            return self

    @classmethod         # "constructor" for this monad
    def unit(cls, val):  # cls is class type instead of instance
        return cls(val, True)

class Success(Failable):
    def __init__(self, value):
        Failable.__init__(self, value, True)

class Failure(Failable):
    def __init__(self, value):
        Failable.__init__(self, value, False)

def failable_monad_examle():
    def fdiv(a, b):
        if b == 0:
            return Failure("cannot divide by zero")
        else:
            return Success(a / b)

    @do(Failable)
    def with_failable(first_divisor):
        val1 = yield fdiv(2.0, first_divisor)
        val2 = yield fdiv(3.0, 1.0)
        val3 = yield fdiv(val1, val2)
        mreturn(val3)

    print with_failable(0.0)      # Failure('cannot divide by zero')
    print with_failable(1.0)      # Success(0.6666)

###### StateChanger Monad #########

class StateChanger(Monad):
    def __init__(self, run):
        self.run = run

    def bind(self, bindee):
        run0 = self.run

        def run1(state0):
            (result, state1) = run0(state0)
            return bindee(result).run(state1)

        return StateChanger(run1)

    @classmethod
    def unit(cls, val):
        return cls(lambda state : (val, state))

def get_state(view = fid):
    return change_state(fid, view)

def change_state(changer, view = fid):
    def make_new_state(old_state):
        new_state    = changer(old_state)
        viewed_state = view(old_state)
        return (viewed_state, new_state)
    return StateChanger(make_new_state)


def state_changer_monad_example():
    @do(StateChanger)
    def dict_state_copy(key1, key2):
        val = yield dict_state_get(key1)
        yield dict_state_set(key2, val)
        mreturn(val)

    @do(StateChanger)
    def dict_state_get(key, default = None):
        dct = yield get_state()
        val = dct.get(key, default)
        mreturn(val)

    @do(StateChanger)
    def dict_state_set(key, val):
        def dict_set(dct, key, val):
            dct[key] = val
            return dct

        new_state = yield change_state(lambda dct: dict_set(dct, key, val))
        mreturn(val)

    @do(StateChanger)
    def with_dict_state():
        val2 = yield dict_state_set("a", 2)
        yield dict_state_copy("a", "b")
        state = yield get_state()
        mreturn(val2)

    print with_dict_state().run({}) # (2, {"a" : 2, "b" : 2})

###### Continuation Monad #########

class ContinuationMonad(Monad):
    def __init__(self, run):
        self.run = run

    def __call__(self, cont = fid):
        return self.run(cont)

    def bind(self, bindee):
        return ContinuationMonad(lambda cont : self.run(lambda val : bindee(val).run(cont)))

    @classmethod
    def unit(cls, val):
        return cls(lambda cont : cont(val))

    @classmethod
    def zero(cls):
        return cls(lambda cont : None)

def callcc(usecc):
    return ContinuationMonad(lambda cont : usecc(lambda val : ContinuationMonad(lambda _ : cont(val))).run(cont))

def continuation_monad_example():
    from collections import deque

    class Mailbox:
        def __init__(self):
            self.messages = deque()
            self.handlers = deque()

        def send(self, message):
            if self.handlers:
                handler = self.handlers.popleft()
                handler(message)()
            else:
                self.messages.append(message)

        def receive(self):
            return callcc(self.react)

        @do(ContinuationMonad)
        def react(self, handler):
            if self.messages:
                message = self.messages.popleft()
                yield handler(message)
            else:
                self.handlers.append(handler)
                done(ContinuationMonad.zero())

    @do(ContinuationMonad)
    def insert(mb, values):
        for val in values:
            mb.send(val)

    @do(ContinuationMonad)
    def multiply(mbin, mbout, factor):
        while True:
            val = (yield mbin.receive())
            mbout.send(val * factor)

    @do(ContinuationMonad)
    def print_all(mb):
        while True:
            print (yield mb.receive())

    original   = Mailbox()
    multiplied = Mailbox()

    print_all(multiplied)()
    multiply(original, multiplied, 2)()
    insert(original, [1, 2, 3])()

if __name__ == "__main__":
    failable_monad_examle()
    state_changer_monad_example()
    continuation_monad_example()
```

# Reference

[^wiki]: Wikipedia article on [Monad in functional programming](https://en.wikipedia.org/wiki/Monad_(functional_programming))

[^p06]: Dan Piponi (2006). "[You Could Have Invented Monads! (And Maybe You Already Have.)](http://blog.sigfpe.com/2006/08/you-could-have-invented-monads-and.html)". A Neighborhood of Infinity

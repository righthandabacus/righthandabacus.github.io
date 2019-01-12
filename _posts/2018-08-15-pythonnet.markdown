---
layout: post
title: "Using pythonnet to interface CSharp library"
date: Wed Aug 15, 2018 10:30 AM
tags: code
---

Recently I encountered the issue of having to call some functions from an
third-party library, which is available as .NET DLL only (is it called
"assembly" in .NET jargon?). It doesn't worth the effort to learn and write
code in C# just to be able to use the library. So ways to access the .NET DLL
in Python has been investigated.

It turns out, there are [two ways to hook up .NET and Python](https://stackoverflow.com/questions/34662448):

1. use IronPython, a Python environment implemented in .NET
2. use [pythonnet](https://pythonnet.github.io/), the Python for .NET module in CPython

Because of compatibility to other Python libraries, it should be better to use
the latter. However, it seems to me that their syntax are the same and
therefore the code are interchangeable between the two.

## Bootstrap

Below is an [example code](http://pythonnet.sourceforge.net):

```python
# load Python.NET
import clr

# the CLR namespaces are now recognized as Python packages
from System import String
from System.Collections import *
from System import Environment
drives = Environment.GetLogicalDrives()

# Load an assembly by "AddReference"
clr.AddReference('System.Speech')
from System.Speech.Synthesis import SpeechSynthesizer

spk = SpeechSynthesizer()
spk.Speak('Hello world!')
```

In the above, the `AddReference()` call allows using C# modules (assemblies).
This is the way to do it because the .DLL file for .NET and that for
traditional Windows are different. In order to generate the "old style" DLL
that we can use `ctypes` to access, we have to compile the C# code using `csc`.
The `AddReference()` call will find the DLL in some locations. To hint where to
find the DLL, we can do either of the following:

```python
# method 1: use AddReferenceToFileAndPath()
import clr
clr.AddReferenceToFileAndPath(r"C:\Folder\Subfolder\file.dll")

# method 2: append the path to sys.path
import clr
import sys
sys.path.append(r"C:\Folder\Subfolder")  # path of dll
clr.AddReference("file.dll") # the dll
```

Afterwards the .NET namespaces are available as Python modules.

However, I found that Python for .NET works under native Windows but not under
cygwin (which somewhat reasonable because under cygwin, it try to pretend as
much as a Linux). If we write a code that may be (accidentally) run under cygwin,
we can check against the case with:

```python
import platform
print(platform.python_implementation())
```

Also we can put path for DLL files in `sys.path` but mounted drive letter are
not supported (quite sure this is the issue with Python for .NET module, not
the python interpreter). In case a mounted drive letter is in the path, e.g.
`M:\Lib`, calling `clr.AddReference()` will cause the error of "Unable to find
assembly" even if some other path *after* the mounted drive letter provides the
correct DLL.

## Allowing `ctypes` to use the .NET code

The traditional `ctypes` module cannot load those DLL is because they are not
["COM visible"](https://stackoverflow.com/questions/25301889/how-can-i-use-a-c-sharp-dll-from-python-script)
and `ctype` cannot access a .NET assembly. The DLL file is COM visible only if
we set the following attribute in the C# code:

    [ComVisible(true)]

or better, we explicitly [expose the
function](https://stackoverflow.com/questions/7367976/calling-a-c-sharp-library-from-python).
Below is an example of how to do it (in C#):

```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using RGiesecke.DllExport;

class Test
{
    [DllExport("add", CallingConvention = CallingConvention.Cdecl)]
    public static int TestExport(int left, int right)
    {
        return left + right;
    }
}
```

The above code must set, in Visual Studio, the Settings → Build → Platform
Target to "x86" or "x64", but not "Any CPU". Then the generated DLL can be
called using `ctypes`:

```python
import ctypes
a = ctypes.cdll.LoadLibrary(source)
a.add(3, 5)
```

Of course, we cannot go with this approach if we do not have the source code.
But the following pages describe how to write a wrapper over C# DLL so that we
can use in Python:

- https://stackoverflow.com/a/42930903/9214517
- http://pragmateek.com/if-your-plumbing-doesnt-work-youre-just-not-using-enough-pipes/
- https://stackoverflow.com/questions/39456464/calling-c-sharp-from-python
- https://stackoverflow.com/questions/33285732/how-to-import-a-c-sharp-dll-to-python

In summary, C# code should have function marked `[DllExport]` or more verbose,

    [DllExport("add", CallingConvention = CallingConvention.Cdecl)]

to specify the exact name exposed as opposed to the function name. Then Python
can use `ctypes.WinDLL("path/to/myfile.dll")` to load the DLL

## Executing Python in C#

In the reverse direction, Python for .NET also allows C# code to access Python.
For example the following code is from [Python for .NET
readme](https://github.com/pythonnet/pythonnet/blob/master/README.md):

```csharp
static void Main(string[] args)
{
    using (Py.GIL())
    {
        dynamic np = Py.Import("numpy");
        Console.WriteLine(np.cos(np.pi * 2));

        dynamic sin = np.sin;
        Console.WriteLine(sin(5));

        double c = np.cos(5) + sin(5);
        Console.WriteLine(c);

        dynamic a = np.array(new List<float> { 1, 2, 3 });
        Console.WriteLine(a.dtype);

        dynamic b = np.array(new List<float> { 6, 5, 4 }, dtype: np.int32);
        Console.WriteLine(b.dtype);

        Console.WriteLine(a * b);
        Console.ReadKey();
    }
}
```

## Specific syntax

Python and C# are different languages. And .NET system do not have an exact
language mapping to Python syntax. Some nice thing in C#, such as iterable data
structures, are trivially mapped as iterables in Python, but others are not
directly mapped. Below I list out some aspects that we need a special syntax in
Python in order to access to the C# library:

### data type

There are different data type systems in .NET than in Python. Some data type
will be converted automatically when we use it, but some do not.  One example
is the `Decimal` type, which we have to do conversion manually:

```python
# Decimal to int:
n = Deciaml.ToInt32(decimal_obj)
assert isinstance(n, int)

# Convert to string:
s = decimal_obj.ToString()
assert isinstance(s, str)
```

Because `Decimal` is a class, reverse the above conversion is as simple as
`decimal_obj = Decimal(n)`.

### generics

C# has the ["generics" syntax](https://stackoverflow.com/questions/35494331/how-can-i-get-generics-to-work-in-python-net-with-cpython)
(mimicking C++ templates) and such code in C#:

```csharp
EventHandler<EventArgs>(); // C# code
```

becomes this in Python:

```python
EventHandler[EventArgs]() # Python code
```

### output variables

As an example, the `DateTime.TryParse()` function in C# has the following
signature:

```csharp
public static bool TryParse(
    string s,
    out DateTime result
)
```

which means the function will write something in `result` (must be a lvalue)
and at the same time return another value from the function.  Python does not
have the concept of output variable in the function. Therefore the above
function will be transformed to return a tuple of `(bool, DateTime)`:

```python
ok, parsed = DateTime.TryParse("2018-01-01 12:01:00", DateTime())
```

To match the function signature, we must provide a dummy `DateTime()` as the
second argument to the function `TryParse()`. But the `result` that supposed to
be written into now becomes the second element of the returning *tuple*. The
one provided as function argument, however, will never be used. The first
element of the tuple is always the original return value of the function
according to the C# function signature. Simply speaking, If the function has
$$n$$ output variables, the return will be a $$(n+1)$$-tuple. Python for .NET
requires the function prototype to match exactly so that in case of overloaded
function, it can ensure which one to invoke.

### delegates

According to [this SO question](https://stackoverflow.com/questions/22384783/how-to-pass-python-callback-to-c-sharp-function-call)
and [the doc](http://pythonnet.sourceforge.net/readme.html), the following is
the way to pass a python function to a C# function as "delegates":
 
```python
def my_handler(source, args):
    print 'my_handler called!'

# instantiate a delegate
d = AssemblyLoadEventHandler(my_handler)

# use it as an event handler
AppDomain.CurrentDomain.AssemblyLoad += d
```

Delegate is the C# way to call a function object.  C# Events, however, can do
implicit conversion on the delegates that are supposed to be event handlers:

```python
def handler(source, args):
    print 'my_handler called!'

# register event handler
object.SomeEvent += handler

# unregister event handler
object.SomeEvent -= handler

# fire the event
result = object.SomeEvent(...)
```

## References
- [Documentation](http://pythonnet.sourceforge.net/readme.html) in sourceforge or [a tutorial](http://pythonnet.github.io/) on Github
- https://stackoverflow.com/questions/4350191/c-sharp-scripting-python
- https://stackoverflow.com/questions/1200182/how-to-use-a-c-sharp-dll-in-ironpython
- https://stackoverflow.com/questions/14633695/how-to-install-python-for-net-on-windows
- Repository for Windows python binary: <https://www.lfd.uci.edu/~gohlke/pythonlibs/#pythonnet>
- https://stackoverflow.com/questions/34662448/net-c-interop-to-python
- [IronPython cookbook](http://www.ironpython.info/index.php/Speech_Synthesis)
- http://ironpython.net/
- https://notebooks.azure.com/api/user/denfromufa/library/pythonnet/html/Python.NET.ipynb

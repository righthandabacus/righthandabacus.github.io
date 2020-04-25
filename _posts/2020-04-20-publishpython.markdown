---
layout: post
title: Publishing a python package
date: 2020-04-20
lastedit: 2020-04-22
tags: code
---

When time has come, we may want to publish a python project as a package onto
PyPI. While a lot of my work were proprietary, there is finally a good project
that I can publish and this is what to do to put it up:

There is a site test.pypi.org for testing. We should use that and check before
we make the real one. First we have to register an account on test.pypi.org and
after that, generate a token so we can use the token for authentication instead
of username and password when we publish the package.

Once we did that we have to create `$HOME/.pypirc` with the following:

```
[distutils]
index-servers=
    pypi
    testpypi

[testpypi]
repository: https://test.pypi.org/legacy/
username: __token__
password: pypi-AgE.....

[pypi]
username: __token__
password: pypi-AgE...
```

If we use token, we have to say `username: __token__` and put the token as
password. Similarly for the real repository, pypi.org as the two sites do not
share the same authentication system. Hence we have the `.pypirc` as above.

The next step is to clean up the project. We can see how other projects doing
this. In general, we need:

- README
- LICENSE
- MANIFEST.in
- changelog
- requirements.txt
- setup.py

Not all of these are required but it is good to have. README and changelog is
quite free-style. I would prefer to do it in markdown. You may use [keep a
changelog](https://keepachangelog.com/en/0.3.0/) as a guideline. LICENSE should
be created and provided for you when you create your project on github, for
example. The file `requirements.txt` is quite standard in python projects and
helps `pip` to install dependent packages. An example of it is as follows:

```
lxml>=3.7.3
requests==2.20.0
```

Or you can skip the version and just put the name of the package there.
`MANIFEST.in` tells what to include in the python package. Usually it is a list
of non-code files. An example is as follows, as we are required to include the
license to fulfil the requirement of GPL:

```
include LICENSE
include README.md
```

The most complicated one is `setup.py`. I will keep my version as a template
and use it every time I make a new project. Here is how it looks like:

```python
import os.path
from setuptools import setup, find_packages

CWD = os.path.abspath(os.path.dirname(__file__))

# Get the long description from the README file
with open(os.path.join(CWD, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

import my_package

packages = find_packages(exclude=['contrib', 'docs', 'tests'])  # py module name
package_data = {}
requires = []
classifiers = [
	# See https://pypi.org/classifiers/
    "Development Status :: 3 - Alpha",
    "Programming Language :: Python :: 3",
]

setup(
    name = 'my_package',  # need a weird name to prevent conflict
    description = 'blah blah'
    version = my_package.__version__,
    author = my_package.__author__,
    author_email = my_package.__author_email__,
    long_description = long_description,
    long_description_content_type = 'text/markdown',

    # Look for package directories automatically
    packages = packages,
	py_modules = ['my_package'],
    package_data = package_data,

    # runtime dependencies
    install_requires = requires,
    url = "https://example.com",
    license = "GPL",
    classifiers = classifiers,
)
```

Most of the key things are included above. The keyword argument
`install_requires` to `setup()` should be a list of package names that we need
to install, `packages` argument should be list of all python files of this
project, to be discovered by the `find_packages()` call. But if the module is a
flat hierarchy (i.e., the code is not inside a directory of the same name but
as a `.py` file at the top level), we have to specify it as `py_modules`
argument, with list of names of the module (no `.py` extension). Of course, we
need the name, author, version, etc. to publish onto pypi.org and they are part
of the module-level constants defined in the code.

Once we finish with the `setup.py` we can test locally by running the following
command at the project's top directory:

    pip install .

Once this works, we can build the package. A compiled package may be more
complicated but the flow should be the same. First we build a source
distribution package (i.e., a tarball of all codes and files of the module) as
well as a wheel package of it (for convenience of the users only, not mandatory
to produce both):

    python setup.py sdist
	python setup.py bdist_wheel

Then a tarball and a `.whl` file will be created in `dist/` directory under the
project's root dir. We can further test it by pip install the corresponding
local file.

Then we register and upload the package: We first pip install the tool `twine`
and then upload everything in `dist`:

    twine upload -r testpypi dist/*

then wait a few minutes for pypi to upload the index and run the following to
ensure it works correctly:

    pip install --index-url https://test.pypi.org/simple/ your-package


If everything's fine, we can then repeat the above command on the real `pypi.org`:

    twine upload dist/*
    pip install your-package

We do not need additional arguments to the command as `pypi` is the default index to use

## Reference

https://packaging.python.org/guides/using-testpypi/

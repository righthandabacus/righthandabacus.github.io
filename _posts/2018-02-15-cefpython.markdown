---
layout: post
title: Reading DOM in CEFPython3
date: Thu, 15 Feb 2018 00:33:35 -0500
tags: code
---

Scrapy can't handle Javascript. Therefore I use PhantomJS in Selenium, via
GhostDriver, instead. But this is now marked as deprecated ([PhantomJS people
disagree](https://github.com/ariya/phantomjs/issues/15314)).  Even if it is not,
there are some weakness in Selenium+GhostDriver. Most noticeably is that I have
no way to tell if the browser is finish rendering the doc. This sometimes create
too much nuisance to *pretend* as a user on a web scrapping project. Therefore I
try out the official remote WebKit solution, namely, the Chromium Embedded
Framwork (CEF).

Google opened up the C++ CEF but it does not come with official Python bindings.
[CEFPython3](https://github.com/cztomczak/cefpython) is a third-party, a bit old
(Chromium v57 vs Chromium v64), but usable.

CEFPython can be installed with `pip install`. And what I am interested is to
retrieve the DOM upon a page is loaded. There is some conceptual thing to
remember:

- CEF is running in another process/thread, almost all function call to CEF are
  asynchronous
- Similar to Javascript, we should use *a lot* of callbacks to get the job done
- There is a function call `MessageLoop()` that must be run or otherwise the
  Chromium browser will be stuck. And this is *blocking* until the browser quits
- We can bind Python functions as Javascript functions
- We can also execute Javascript functions from Python, but such execution is
  asynchronous, thus always return nothing
- We can also pretend user interaction with browser, e.g. click, drag-and-drop

The following is an example, try to load Google and get its DOM:

```python
from __future__ import print_function
from cefpython3 import cefpython as cef
import sys
import threading

class LoadHandler(object):
    def OnLoadingStateChange(self, browser, is_loading, **kwargs):
        if not is_loading:
            print("loading completed")
            print(browser.GetUrl()) # should be redirected to https://www.google.com
            bindings = cef.JavascriptBindings(bindToFrames=False, bindToPopups=False)
            bindings.SetFunction("everything", everything)
            browser.SetJavascriptBindings(bindings)
            threading.Timer(0.1, runjs).start()
        else:
            print("loading not yet completed")

def everything(x):
    print(x)
	print(len(x))
	browser.CloseBrowser()

def runjs():
    print('*** run JS')
    JS_CODE = open('cefget.js').read()
    browser.ExecuteJavascript(JS_CODE)
	print('*** JS should call Python function in a while')

sys.excepthook = cef.ExceptHook # shutdown CEF processes on exception
cef.Initialize(settings={})
browser = cef.CreateBrowserSync()
browser.SetClientHandler(LoadHandler())
browser.LoadUrl('http://www.google.com')
cef.MessageLoop()
print('*** CEF shutdown')
cef.Shutdown()
```

The python code is very much like the tutorial in `cefpython`, except I call
explicitly the `browser.LoadUrl()` instead of passing a URL at
`cef.CreateBrowserSync()`. This `CreateBrowserSync()` is virtually the only way
you can create a browser in `cefpython` and the sync in the name means this is
a synchronous call -- browser is ready upon the function return.

Right after the browser is created, we invoke `cef.MessageLoop()` and this will
block the main thread until the browser terminates. All work are coded in
event-driven manner. Firstly, we hook up the load state such that, in
`LoadHandler.OnLoadingStateChange()`, when load is complete, we bind the Python
function `everything()` to Javascript and schedule run a piece of Javascript
code. The schedule run is necessary because Javascript binding is asynchronous.
You cannot have the bound function ready immediately. It turns out, even 0
second wait can do the job. You just need to make sure you do not execute
Javascript end to end, such as by making a thread using `threading.Timer()`.

The Javascript code is passed onto the browser as string. In the code, it will
invoke the bound `everything()` function, which in turn will trigger Python to
close the browser.

The following is the Javascript code to run, saved in a file `cefget.js`:

```javascript
(function(){
    function addpath(nodepath, element, xpath) {
        var rect = element.getBoundingClientRect();
        var offsetx = window.pageXOffset;
        var offsety = window.pageYOffset;
        var visible = (window.getComputedStyle(element, null).display != 'none')?1:0;
        var fgcolor = window.getComputedStyle(element, null).color;
        var bgcolor = window.getComputedStyle(element, null).getPropertyValue('background-color');
        var text = element.innerText;
        var html = element.outerHTML;
        nodepath.push([xpath, visible, rect.left+offsetx, rect.top+offsety, rect.width, rect.height, fgcolor, bgcolor, text, html]);
    };
    function pathwalker(nodepath, element, basepath) {
        var children = element.childNodes;
        var tagmap = {} // offset list for each children's tag
        for (var i=0; i<children.length; i++) {
            if (!children[i].tagName) continue;
            var tag = children[i].tagName.toLowerCase();
            if (tagmap[tag]) {
                tagmap[tag].push(i);
            } else {
                tagmap[tag] = [i];
            };
        };
        for (var i=0; i<children.length; i++) {
            if (!children[i].tagName) {
                continue // usually comment node
            };
            var tag = children[i].tagName.toLowerCase();
            /*
            if (tag.indexOf(':') >= 0) {
                continue // ignore everything with a prefix or namespace
            };
            */
            var xpath = basepath+'/'+tag;
            if (tagmap[tag].length > 1) {
                xpath = xpath + '[' + (tagmap[tag].indexOf(i)+1) + ']';
            };
            addpath(nodepath, children[i], xpath);
            pathwalker(nodepath, children[i], xpath);
        };
        return nodepath;
    };
    var nodepath = [];
    everything(pathwalker(nodepath, document, ''))
})();
```

It is a simple depth-first search DOM walker. It scan for each DOM element,
check its geometry and visibility, and extract its text and HTML code. Each
element will become an array (in `addpath()`) and the document will become a 2D
array of all its elements through the recursive call of `pathwalker()`. Finally
that 2D array is returned to the Python function `everything()`.

There is something to note: Simple Javascript objects can be passed back to
Python callback function. But *never pass back a DOM element*. Very likely it
will throw exceptions for the cause of too much recursive in converting JS
object into Python object. Besides that, CEF can exchange data between Python
and Javascript very fast. My impression is much faster than what you can get
with Selenium+GhostDriver.

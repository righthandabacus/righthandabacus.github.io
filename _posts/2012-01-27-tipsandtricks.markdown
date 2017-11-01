---
layout: post
title: "Tips and Tricks"
date: 2012-01-27 22:39:29 -0400
tags: code
---

## Script loading
Load a JavaScript by JavaScript:

```JavaScript
var jsObj = document.createElement("script");
jsObj.src = "/path/to/my.js";
document.body.appendChild(jsObj);
```

## Loading image and do relevant action

To load an image dynamically through JavaScript, we can always use the `createElement()`:
```JavaScript
var imgObj = document.createElement("img");
imgObj.src = "/path/to/image.png";
imgObj.onload = myfunc;
document.body.appendChild(imgObj);
```
The image starts download once the `src` attributed is assigned. It will call the `myfunc()` function when the image is downloaded completely.

## Functions

Two ways to assign functions as event handlers: Firstly, using unnamed function, such as
```JavaScript
object.onclick = function() {
    // code here ...
}
```

Another way is to use function object:
```JavaScript
object.onclick = new Function(
    // code here ...
);
```

The syntax of defining a function object is:

    Function([arg1, arg2, ...], code)

where the arguments `arg1`, `arg2`, etc and the `code` are all in string format. The arguments are string literals that going to be used in the code.

Using function object helps to build code dynamically.

## Event handler
In the JavaScript code,

    object.onclick = funcName;

The function `funcName()` is called when the event triggers. When it is called, it can access to an argument of event object (for details, see <http://www.w3schools.com/htmldom/dom_obj_event.asp>).

## Extra property

Extra property of an DOM object can be stored by putting another attribute into an element, for example:
```JavaScript
// Store
$('element').setAttribute("newAttribute","data");

// Retrieve
$('element').readAttribute("newAttribute"));
```
Of course, obscured name for the attribute is preferred to avoid conflict of existing attributes.

## CSS
JavaScript has different naming for the CSS attributes, especially when there is a dash in it.

For example, the CSS property `background` can be accessed with

    $('object').style.background

but in case the property `background-image` is

    $('object').style.backgroundImage

In general, remove all dashes and replace the multiple-word attribute with Java's naming convention.


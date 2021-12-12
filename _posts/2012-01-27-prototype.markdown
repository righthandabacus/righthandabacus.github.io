---
layout: post
title: "prototype.js"
date: 2012-01-27 22:39:29 -0500
tags: code
---

## Loading
Use: put the following line in somewhere of the HTML

```html
<script type="text/javascript" src="path/to/prototype.js"></script>
```

## Referring to objects
In HTML, we can identify objects by the `id` tag:

```html
<div id="whatever">
  blah blah
</div>
```

To refer to that particular piece of object in JavaScript, we uses

    document.getElementById("whatever")

and in prototype.js, we can use

    $("whatever")

and hence the following are equivalent:

    var foo = $("bar");
    var foo = getElementById("bar");

The `$()` syntax can be used as an array:

```javascript
var objs = $("objA", "objB");
for (i=0; i<objs.length; i++) {
    objs[i].innerHTML = blah blah blah;
}
```

Also, we can refer to an array of objects by HTML tag name (built-in) and by class name (provided by prototype.js), e.g.
```javascript
objs = document.getElementsByClassName("blah");
for (i=0; i<objs.length; i++) {
    objs[i].innerHTML = blah blah blah;
}

objs = document.getElementsByTagName("span");
for (i=0; i<objs.length; i++) {
    objs[i].innerHTML = blah blah blah;
}
```

## HTML form related
In case we have some value from a form, we can firstly, code the HTML as follows:

```html
<select name="foo" id="foo">
   <option value="one">un</option>
   <option value="two">deux</option>
   ...
</select>
```

then the following will get back the value:

    var foo = $F("foo");   // "one" or "two"

But it cannot pass on multiple IDs like `$()` does. To get everything from a form (with id), use the following syntax:

```javascript
var objs = Form.getElements("formId");
for (i=0; i<objs.length; i++) {
    // blah blah blah
};
```

Moreover, to help sending AJAX request, we can serialize a form as follows:

```javascript
  var string = Form.serialize("formId");
  // string = field1=value1&field2=value2&field3=...
```

Side note: If we need an array in the form (e.g. filling a table in HTML), we can use the following way:

```html
<input type="text" id="name[]" />
<input type="text" id="name[]" />
<input type="text" id="name[]" />
```

Then when passed to PHP, you use `$_POST['name']` will get an array of three elements.

## Coding help

Choose one from a list: We can avoid a set of if-then-else by using `Try.these`. This structure will return the *first one* that executed correctly. For example, the following means to get a suitable HTTP request object:
```javascript
var transportRequest = Try.these(
        function() {return new ActiveXObject('Msxml2.XMLHTTP')},
        function() {return new ActiveXObject('Microsoft.XMLHTTP')},
        function() {return new XMLHttpRequest()}
    );
```

## AJAX support
By example:
```javascript
var myAjax = new Ajax.Request(
    ajaxhandler.php,    // The URL
    { method: 'post',    // default if not specified
      parameters: 'field1=value1&field2=value2',    // Parameter (optional)
      onComplete: responsehandler    // Call this func on response (optional)
    }
);

function responsehandler(RequestObj) {
  // process RequestObj.responseText
}
```

The parameter to the request can be either as a hash:

    parameters: {field: value, field: value},

or as a string:

    parameters: 'field=value&field=value',

or directly generate a string from a form:

    parameters: $('formId').serialize(true),

Instead of `onComplete`, the `Ajax.Request` can also bear `onSuccess` and `onFailure` to distinguish HTML errors. Anonymous function can also be used there. Other `onXXX` callbacks are:
  * `onUninitialized`
  * `onLoading`
  * `onLoaded`
  * `onInteractive`
  * `onException`

## Periodic execution

We can periodically call a function:
```javascript
new PeriodicalExecuter( func , time );
    // func: A JavaScript function going to be called
    // time: Number of seconds between execution
```

See also `Ajax.PeriodicalUpdater` for AJAX-only use of periodic execution.

## Other
OOP: Create a class can be done by using the `Class` object, e.g.

    var myClass = Class.create();

and defining a class is done by using `prototype`, e.g.
```javascript
  myClass.prototype = {
       initialize : function(foo) {
           // constructor
           this.bar = foo;
       },
       ...
  }
```

Inheritance: Use the syntax: `Object.extend(destination, source)`, this will copy all the properties, fields, and methods from the source class to the destination class. For example, the following will set several default options and merge with the argument-provided options:
```javascript
setOptions: function(options) {
        this.options = {
            method:       'post',
            asynchronous: true,
            parameters:   ''
        }
        Object.extend(this.options, options || {});
    }
```

Function binding: Bind a function to an object using syntax `Function.prototype.bind(object)`. For example:
```javascript
var myObject = new Object();
myObject.message = "Hello!";
myObject.eventHandler = function() {
    alert(this.message);
}

$("mydiv").onmouseover = myObject.eventHandler.bind(myObject);
```
This will make sure the `this` pointer points to an object (`myObject`) currectly, instead of the object (`$("mydiv")`)who triggers it.

Event Listener binding: Similar to above, but allows an event object passed on to a function as argument. For example:
```javascript
myObject.eventHandler = function(event) {
    alert(event.srcElement.nodeName);
}

$("mydiv").onmouseover = myObject.eventHandler.bindAsEventListener(myObject);
```

Number conversion: `Number.prototype.toColorPart()` converts a number into its two digit hexadecimal equivalent (ie. 255 => FF, 10 => 0A, etc.).

`PeriodicalExecuter`: Used to call a specific function at regular intervals.


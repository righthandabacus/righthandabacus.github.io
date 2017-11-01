---
layout: post
title: "Javascript"
date: 2007-08-07 03:14:50 +0800
tags: code
---

As of Firefox. According to JavaScript 1.5 and Gecko DOM.

## ECMA Specification Part

Objects specified in ECMA spec (JS 1.5):
  * Array
  * Boolean
  * Date
  * Error
  * Function
  * java
  * JavaArray
  * JavaClass
  * JavaObject
  * JavaPackage
  * Math
  * netscape
  * Number
  * Object
  * Packages
  * RegExp
  * String
  * sun

Global Properties
  * Infinity
  * NaN
  * undefined 

Global Functions
  * Array
  * Boolean
  * Date
  * decodeURI
  * decodeURIComponent
  * encodeURI
  * encodeURIComponent
  * Error
  * eval
  * Function
  * isFinite
  * isNaN
  * Number
  * Object
  * parseFloat
  * parseInt
  * RegExp
  * String 

Functions
  * arguments 

Statements
  * block
  * break
  * const
  * continue
  * do...while
  * export
  * for
  * for...in
  * for each...in
  * function
  * if...else
  * import
  * label
  * return
  * switch
  * throw
  * try...catch
  * var
  * while
  * with 

Operators
  * Arithmetic Operators <nowiki>(+, -, *, /, %, ++, --, unary -)</nowiki>
  * Assignment Operators <nowiki>(=, +=, -=, *=, /=, >>=, <<=, >>>=, &=, |=, * ^=)</nowiki>
  * Bitwise Operators <nowiki>(&, |, ^, ~, <<, >>, >>>)</nowiki>
  * Comparison Operators <nowiki>(==, !=, ===, !==, >, >=, <, <=)</nowiki>
  * Logical Operators <nowiki>(&&, ||, !)</nowiki>
  * Special Operators:
    * Conditional Operator (condition ? ifTrue : ifFalse)
    * Comma Operator (,)
    * delete Operator
    * function Operator
    * in Operator
    * instanceof Operator
    * new Operator
    * this Operator
    * typeof Operator
    * void Operator 
  * String Operators (+ and += as concatenation operators)
  * Member Operators (obj.property and obj["property"])
  * Operator Precedence 

Comments
  * Code comments <nowiki>(// and /* */)</nowiki>

## DOM of Gecko Engine

Object types:
  * `document` -- such as a HTML document
  * `element` -- a node in the DOM
  * `nodeList` -- an array of elements
  * `attribute` -- of a node
  * `namedNodeMap` -- an associative array

## Syntax

Declaration and assignment:

```javascript
var intValue = 1;
var floatValue = 3.0;
var stringValue = "This is a string\n";
var sqString = 'This is also a string';

var emptyList = [];
var homogenousList = [1, 2, 3];
var heterogenousList = ["one", 2, 3.0];

var emptyMap = {};
var homogenousMap = {"one": 1, "two": 2, "three": 3};
var heterogenousMap = {"one": 1,
                       "two": "two",
                       "three": 3.0};

homogenousMap["one"] = 10;
homogenousMap.two = 20;
```

Removal:

```javascript
delete homogenousMap["one"];
delete homogenousMap.two;
```

Iteration

```javascript
for (var key in heterogenousMap) {
    window.alert("Heterogenous map property \""
                 + key
                 + "\" = "
                 + heterogenousMap[key]);
}
```

Functions

```javascript
var callable = function (message) { // <-- notice assignment
    window.alert("Callable called with message = "
                 + message);
}


function createClosure(initial) {
    var res = function () {
        initial = initial + 1;
        window.alert("Closure with modified state "
                     + initial);
    }
    return res;
}

function callCallable(f, x) {
    f(x);
}

function composeCallables(f, g, x) {
    f(g(x));
}
```

Objects

```javascript
function MyObject(name, value) {
    this.name = name;
    this.value = value;
}
```

Javascript supports prototype based object orientation. Not a class type but
an object constructor is created for new objects with particular properties.
In the example above the this keyword used to reference the ''current
instance'' of the object. The this object is essentially a property map with
members accessed (and initialized) in this example with the dot notation.

The object constructor, MyObject, is an object constructor not in how it's
defined, which looks like any other Javascript function, but in how it's
`invoked`.

    var my = new MyObject("foo", 5);

The new operator before the function invokes the function with a newly
construced object as this and returns that the initialized object.
Object Prototype

Part of what makes a language object oriented is that data not only has
properties but also `behaviors`. Also known as: member functions; methods;
and object messages. To implement a member function in Javascript one would be
tempted to write something like what's below based on the member
initialization exampled above.

```javascript
function BadObject(data) {
    this.data = data
    this.memberFunction = function () {
        // ...functions on data...
    }
}
```

While the code above will work without error, it does create a new closure for
each member function for each new instance of the object. What's really
required is a class level function that works on instance data. But remember,
Javascript objects aren't class based but prototype based. So how do we
implement "class" level member functions? (Skip to Implementation) Better yet,
how do we implement "class" level members functions in general?

Enter the prototype member.

The internal object member, prototype, has language defined significance in
that it is used for resolving property names if the property isn't found in
the current property map. It's considered internal because, while the
instance's prototype member is `inherited` from the ''constructor's''
prototype member, it cannot be accessed directly from the object instance
itself. The defined prototype member is a property map itself which holds
members for property name resolution. Consider the example below:

```javascript
 var parentPropertyMap = {"bar": "I'm the bar"};

 // Define the constructor with inheritable properties
 function ChildObject(foo) {
     this.foo = foo;
 }
 ChildObject.prototype = parentPropertyMap;

 childPropertyMap1 = new ChildObject("I'm the foo1");
 childPropertyMap2 = new ChildObject("I'm the foo2");

 // Prints "childPropertyMap1.foo = I'm the foo1"
 window.alert("childPropertyMap1.foo = " + childPropertyMap1.foo);

 // Prints "childPropertyMap2.foo = I'm the foo2"
 window.alert("childPropertyMap2.foo = " + childPropertyMap2.foo);

 // Prints "childPropertyMap1.bar = I'm the bar"
 window.alert("childPropertyMap1.bar = " + childPropertyMap1.bar);

 // Prints "childPropertyMap2.bar = I'm the bar"
 window.alert("childPropertyMap2.bar = " + childPropertyMap2.bar);
```

The member foo is an instance member added to the instance's property map
during construction:

```javascript
 function ChildObject(foo) {
     this.foo = foo;
 }
```

while bar is in the constructor's prototype:

```javascript
 var parentPropertyMap = {"bar": "I'm the bar"};
 ...
 ChildObject.prototype = parentPropertyMap;
```

which is `inherited` during the new operation:

```javascript
 childPropertyMap1 = new ChildObject("I'm the foo1");
 childPropertyMap2 = new ChildObject("I'm the foo2");
```

In other words, the member, bar, is shared across all instances of
ChildObject.

Therefore, by implementing the prototype member of the constructor function,
we can think of the constructor function itself as the "class" object.
Complete with static class functions:

```javascript
 function ClassObject() {}
 ClassObject.staticClassFunction = function(x) {
     return x * 2;
 }
```

static class variables:

```javascript
 function ClassObject() {}
 ClassObject.staticClassVariable = 5;
```

shared member variables:

```javascript
 function ClassObject() {}
 ClassObject.prototype.sharedMember = 5;
```

and of course, shared member functions:

```javascript
 function ClassObject(x) {
     this.x = x;
 }
 ClassObject.prototype.memberFunction = function(x) {
     return x * this.x;
 }
```

Member Function Implementation

```javascript
function Message(message) {
    this.message = message;
}

Message.prototype.show = function() {
    window.alert("Message.show() with message = "
                 + this.message);
}
```

(More on Classes and Objects)
Example Code

```javascript
////////////////////
// Basic Types
var intValue = 1;
var floatValue = 3.0;
var stringValue = "This is a string\n";

////////////////////
// Array
var emptyList = [];
var homogenousList = [1, 2, 3];
var heterogenousList = ["one", 2, 3.0];

////////////////////
// Property Map
var emptyMap = {};
var homogenousMap = {"one": 1, "two": 2, "three": 3};
var heterogenousMap = {"one": 1,
                       "two": "two",
                       "three": 3.0};

////////////////////
// Functions as values
var callable = function (message) { // <-- notice assignment
    window.alert("Callable called with message = "
                 + message);
}

function createClosure(initial) {
    var res = function () {
        initial = initial + 1;
        window.alert("Closure with modified state "
                     + initial);
    }
    return res;
}

////////////////////
// Functions as arguments
//
function callCallable(f, x) {
    f(x);
}

function composeCallables(f, g, x) {
    f(g(x));
}

////////////////////
// Objects
//
function MyObject(name, value) {
    this.name = name;
    this.value = value;
}

////////////////////
// Objects with Member Functions
//
function Message(message) {
    this.message = message;
}

Message.prototype.show = function() {
    window.alert("Message.show() with message = "
                 + this.message);
}

////////////////////
// Demo Utilities
//
function quote(message) {
    return "\"" + message + "\"";
}

////////////////////
// HTML Invoked demonstration
// 
//
function main() {
    window.alert("Integer = " + intValue);
    window.alert("Float = " + floatValue);
    window.alert("String = " + stringValue);

    for (var item in emptyList) {
        window.alert("Empty list item = " + item);
    }

    // Script style index iteration
    for (var i in homogenousList) {
        window.alert("Homogenous list item = "
                     + homogenousList[i]);
    }

    // C style index iteration
    for (var i=0; i < heterogenousList.length; ++i) {
        window.alert("Heterogenous list item = "
                     + heterogenousList[i]);
    }

    // Dot notation property access
    window.alert("Homogenous map property \"one\" "
                 + homogenousMap.one);
    // Subscript notation property access
    window.alert("Homogenous map property \"two\" "
                 + homogenousMap["two"]);

    for (var key in heterogenousMap) {
        window.alert("Heterogenous map property \""
                     + key
                     + "\" = "
                     + heterogenousMap[key]);
    }

    callable("(Function value invoked)");
    closure();
    closure();

    callCallable(closure);
    composeCallables(callable, quote, "My Message");

    var my = new MyObject("foo", 5);
    window.alert("MyObject my.name = " + my.name);
    window.alert("MyObject my[\"value\"] = " + my["value"]);

    var msg = new Message("bar");
    for (var key in Message.prototype) {
        window.alert("Message prototype member \""
                     + key
                     + "\" = "
                     + Message.prototype[key]);
    }

    window.alert("Message msg.message = " + msg.message);
    msg.show();
}
```


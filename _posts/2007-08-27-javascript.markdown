---
layout: post
title: "Broken Associative Array of JavaScript"
date: 2007-08-27 04:52:44 +0800
tags: code
---
The Associative Array (Perl's name: Hash) in JavaScript is broken. It is poorly
implemented for sake of backward compatibility. However, it can be a nightmare:

```javascript
var hash = new Array();
if (hash["clear"]==null) {
    hash["clear"]=1;
};
```

The above code will never set `hash["clear"]` to `1` because the key `clear` is
already defined. The reason is that, `clear()` is a function to array and `hash`
is an instance of array. So the key *is* mapped to a function!

To make it works, consider these variations:

```javascript
var hash = new Array();
if (hash["clear"]===undefined) {
    hash["clear"]=1;
};
```

```javascript
var hash = new Array();
if (hash["clear"]!="function" && hash["clear"]==null) {
    hash["clear"]=1;
};
```

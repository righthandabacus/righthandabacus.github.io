---
layout: post
title: "Java Strings"
date: 2006-08-15 12:50:49 +0800
tags: code
---
I learnt something about Java's string over [here](http://www.yoda.arachsys.com/java/strings.html).

Basically, it says,

```java
String data = "";
String s;
while ( (s=reader.readLine()) != null) {
    data += s + " ";
//  After compiler:
//    data = new StringBuffer(data).append(s).append(" ").toString();
};
System.out.println(data);
```

in other words, all `String` objects are not mutable --- you can only create a
new one with modified content and delete the old one. Hence string manipulation
involves a lot of malloc-free operations and consequently, inefficient. Thus the
better way to do is to use `StringBuffer` objects which is expandable
dynamically. For example:

```java
StringBuffer data = new StringBuffer();
String s;
while ( (s=reader.readLine()) != null) {
    data.append(s+" ");
}
System.out.println(data.toString());
```

Note, the problem of inefficiency happens only when you *manipulate* the string,
hence the concatenation in the `append()` function in the code above doesn't
matter.

---
layout: post
title: "Design Patterns: Elements of Reusable Object-Oriented Software"
subtitle: "Gang of Four / 1994"
date: 2010-04-05 17:20:47 -0400
category: book
bibtex:
  type: book
  title: "Design Patterns: Elements of Reusable Object-Oriented Software"
  author: Erich Gamma and Richard Helm and Ralph Johnson and John M. Vlissides
  publish: Addison-Wesley
  year: 1994
  isbn: 0201633612
---
This was my textbook of IEG3080 back in year 2001. Notes below are short summaries of different patterns (that I didn't use much, e.g. other than factories).

A reference card is found one the [web](http://www.mcdonaldland.info/files/designpatterns/designpatternscard.pdf)

# Creational patterns
Factory, Abstract factory, Builder, Prototype, Singleton

## Factory
Use the same interface in the abstract class to make an instance of an object but let the subclass to decide which object to create.

## Abstract Factory
An interface to a collection of factories so that the client can create a set of objects in a correct combination.

## Singleton
By exploiting a factory method, to ensure only one incarnation exists for a class.

## Builder
Use a function to build a complex structure of many objects. But instead of hard-coding the objects, this allows the flexibility for configuration, e.g. through parameters.

## Prototype
Implement a `clone()` operation in a class. When a new object is needed, invoke `clone()` instead of a factory so that part of an incarnated object can be cloned to the new object as well.

# Structural patterns
Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy
## Adapter
Conversion of interface from class A to class B. The adapter share the same interface with class B and when it is initialized, it is given an object of class A. Then the adapter is passed to a client that expects an incarnation of class B. When a class B function is invoked by the client, the adapter emulates class B behaviour by calling methods in class A.

## Facade
Use one interface to encapsulate the complex operation that involves a large number of objects so that the invoke of the operation could be simpler. Just like a "macro" in OOP. It also decouples the invoker from a large number of classes.

## Proxy
Proxy mimics the interface of another object. Proxy is passed to the client and it intercepts the interaction between the client and the underlying object. The use of proxy is to provide intercepting features such as remote connection caching or security check.

## Bridge
Similar to adapters, this purpose is to simplify class hierarchy. One abstract class (abstraction) is calling a virtual function of another abstract class (implementation), so that different clients (sub-class of the abstraction) can use different implementation transparently. The sub-class of implementation is a parameter in a bridge.

## Composite
Use the identical interface for components and composite components, where composite components are simply a set of components (implemented as a linked list or a tree). Then, the client has the same interface to control one component as to control a set of them.

## Decorator
Use in place of multiple inheritance and provides pluggable flexibility. A decorator and a real object both derived from the same abstract interface. The decorator has a pointer to an object of the interface class. When functions of the interface class is called through the decorator, the one in the object as referred by the pointer is invoked to do the real job but the decorator can modify the behaviour slightly.

## Flyweight
A pattern aimed to avoid a large number of similar objects. Example in the book is a typeset document. If we store each element as an object, we have to store a list of front, size, color, position, etc. Instead, we keep style in a tree and the characters in a list. And use the sequential index to fetch the style and compute positions.

# Comparing structural patterns
## Bridge vs Adapter
They are similar but usually adapter is used when in a later stage we found we need to have different interface to work together. But bridge is a feature foresee in the early phase of design.

## Facade vs Adapter
Facade is an adapter in certain sense. But usually adapter means to se the old interface but facade defines new interface.

## Composite vs Decorator
Both are using a recursive composition (i.e. within the class, there is a pointer to an object of the parent class where the real functions are implemented there)

## Decorator vs Proxy
Both forward requests. But proxy is not for recursive composition but for serving as a middleman because direct access is not convenient. Proxy controls access but does not provide the function whereas decorator provides supplementary functions.

# Behavioural patterns
Chain of responsibility, Command, Interpreter, Iterator, Mediator, Memento, Observer, State, Strategy, Template, Visitor
## Chain of responsibility
Use a list of objects to handle requests/events from a client. When the first object cannot handle the request, it passes it onto the second object, etc. Usually use with composite pattern to simplify the interface to clients.

## Command
Make request into an object so that an invoker does not call a function in the handler explicitly. The function to execute the command usually is implemented inside the command. Doing so makes the invoker see a unified interface to all commands. Example: A menu may create different commands but upon a click on the menu item, all command just need to run the execute() function.

## Template
Define a skeleton of algorithms at the base class and let the derived class define the fine steps so that slightly varied operation can share the same structure.

## Iterator
A general mechanism to access a collection sequentially without exposing the underlying structure

## Strategy
Define a set of interchangeable algorithms and encapsulate into the same interface.

## Observer
A subject maintains a call list and different observers subscribe to the list to get notified. Observers may unsubscribe later on. The subject-observer relationship is one-to-many. All observers share the same interface for the subject to notify.

## State
Make a state into an object and associate the state-specific operation into a state as functions. For example, receiving a SYN packet in TCP can give different reaction in different states. Thus make the SYN reaction as functions and encapsulate them into state objects. When a TCP connection changes state, the state object in the connection is replaced. This state transition may be invoked by the state object.

## Interpreter
Define a language for a problem and evaluate the language statement using a single function call, which in turn, evaluates each sub-statements. From client's point of view, the evaluate call is simple enough.

## Mediator
Like a facade, act as an middleman between a client and a set of other objects. But facade is a unidirectional protocol that makes request to the subsystem. Mediator, however, enables cooperative behaviour. Thus the protocol is multidirectional as each colleague can communicate via the mediator.

## Memento
Use an object to store the old state before making a state change so that a "rollback" is possible. The stored state are objects and thus we can keep them in a list as history and do a sequence of rollbacks. The object is called memento.

## Visitor
Separate an element's operation from a visitor's. For example, the elements are nuts and blots of a machine but a visitor wants to gather certain statistics of the elements. Those operation-unrelated functions can be kept outside the element, but implement an `accept()` function to take a visitor and invoke a function inside. Then we can implement different visitor to do different job without modifying the element's class.

# Principles
## Open-close Principle
An object should be designed such that it is open for extension but closed for modification.

## Hollywood Principle
High-level component calls lower-level components but not the way round. Low level components can only hook on a higher level. This is to unify the dependency to one direction and ease the understanding of a system.

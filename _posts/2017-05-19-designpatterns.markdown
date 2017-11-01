---
title: Design patterns revisited
date: 2017-05-19
tags: code reference
---

Patterns: [singleton](#singleton), [factory](#factory), [factory method](#factory-method),
[abstract factory](#abstract-factory), [builder](#builder), [prototype](#prototype),
[object pool](#object-pool), [adapter](#adapter), [bridge](#bridge), [composite](#composite),
[decorator](#decorator), [facade](#facade), [flyweight](#flyweight), [proxy](#proxy),
[chain of responsibility](#chain-of-responsibility), [command](#command),
[interpreter](#interpreter), [iterator](#iterator), [mediator](#mediator), [memento](#memento),
[observer](#observer), [state](#state), [strategy](#strategy),
[template method](#template-method), [state](#state), [strategy](#strategy),
[visitor](#visitor), [null object](#null-object);
and also [OO design principles](#oo-design-principles).

Singleton
=========

- Creational Pattern
- Ensure only one instance of a class is ever created
- Provide global point of access to the object

![](/img/patterns-singleton.png)

### Implementation
- Static member in the class
- Private constructor
- Static public method that returns a reference to the static member

```java
class Singleton {
	private static Singleton instance;
	private Singleton() {
		...
	}
	public static synchronized Singleton getInstance() {
		if (instance == null)
			instance = new Singleton();
		return instance;
	}
	...
	public void doSomething() {
		...
	}
};
Singleton.getInstance().doSomething();
```

### Issues
Lazy instantiation and double locking: Return instance if already created; lock
and check again and then create object otherwise:

```java
public static Singleton getInstance() {
	if (instance == null)	{
		synchronized(Singleton.class) {
			if (instance == null) {
				instance = new Singleton();
			}
		}
	};
	return instance;
}
```

Early instantiation: initialise the static member at load instead of time of first use:

```java
class Singleton {
	private static Singleton instance = new Singleton();
	...
};
```


Factory
=======

- Creational pattern (non-GoF)
- Creates object without exposing instantiation logic
- Refer to newly created object through a common interface

### Implementation
Noob implementation: parameterized factories to instantiate different objects:

```java
public class ProductFactory{
	public Product createProduct(String ProductID){
		if (id==ID1)
			return new OneProduct();
		if (id==ID2) return
			return new AnotherProduct();
		... // so on for the other Ids

		return null; //if the id doesn't have any of the expected values
	}
	...
}
```

Same interface for user of factory method, but have to modify the factory every
time a new concrete product class is added. To solve this issue, we can have
class registration:

```java
class ProductFactory {
	private HashMap m_RegisteredProducts = new HashMap();
	public void registerProduct (String productID, Class productClass) {
		m_RegisteredProducts.put(productID, productClass);
	}
	public Product createProduct(String productID) {
		Class productClass = (Class)m_RegisteredProducts.get(productID);
		Constructor productConstructor = 
			cClass.getDeclaredConstructor(new Class[] { String.class });
		return (Product)productConstructor.newInstance(new Object[] { });
	}
}
```

Then we can put registration into class:

```java
class OneProduct extends Product {
	static {
		Factory.instance().registerProduct("ID1",OneProduct.class);
	}
	...
}
```

Or outside class:

```java
public static void main(String args[]){
	Factory.instance().registerProduct("ID1", OneProduct.class);
}
```


Factory Method
==============

- Creational pattern
- Defines a common interface for creating objects, but let subclasses to decide which class to instantiate

![](/img/patterns-factory-method.png)

### Implementation
Concrete product classes all descendant from product class. Concrete
factory/creators all descendant from factory class. The base class provide
unique interface but defined in daughter classes.

```java
public interface Product { /* ... */ }

public abstract class Creator {
	public void anOperation() {
		Product product = factoryMethod();
	}
	protected abstract Product factoryMethod();
}

public class ConcreteProduct implements Product { /* ... */ }

public class ConcreteCreator extends Creator {
	protected Product factoryMethod() {
		return new ConcreteProduct();
	}
}

public class Client {
	public static void main( String arg[] ) {
		Creator creator = new ConcreteCreator();
		creator.anOperation();
	}
}
```

### Applicability
It is useful when a class can't anticipate the type of objects it is supposed
to create, but can delegate to subclasses. Example, "new document” button in
every application should call a factory to instantiate a new document but the
concrete type of document depends on the application. The factory method here
is the customization hook.


Abstract Factory
================
- Interface for creating a family of related objects without specifying their classes
- Abstract factory as base class of other concrete factory classes implementing
  the same methods; and abstract product as base class of other concrete product
  classes that the factory creates

![](/img/patterns-abstract-factory.png)

### Example
Define look-n-feel as abstract factory class that provide methods to produce
button and window objects. Concrete factory classes Windows and Motif produce
the button and window objects in Windows and Motif styles respectively. User of
the factory can switch look and feel without changing implementation by picking
up a different concrete factory object.


Builder
=======
- Creational pattern
- Use when the creation algorithm of a complex object is independent from parts
  that actually compose the object

![](/img/patterns-builder.png)

### Implementation
Builder is derived from an abstract builder and passed on to a director. The
director will use the builder interface to create a complex object (product).
Then a function outside of the director will retrieve the product created.

```java
//Abstract Builder
class abstract class TextConverter{
	abstract void convertCharacter(char c);
	abstract void convertParagraph();
}

// Product
class ASCIIText{
	public void append(char c){ /* - */ }
}

//Concrete Builder
class ASCIIConverter extends TextConverter {
	ASCIIText asciiTextObj;//resulting product

	/* converts a character and appends to the result */
	object void convertCharacter(char c) {
		char asciiChar = new Character(c).charValue();
		asciiTextObj.append(asciiChar);
	}
	void convertParagraph() { /* - */ }
	ASCIIText getResult() {
		return asciiTextObj;
	}
}

//This class abstracts the document object
class Document {
	static int value;
	char token;
	public char getNextToken() {
		//Get the next token
		return token;
	}
}

//Director
class RTFReader {
	private static final char EOF='0'; //Delimitor for End of File
	final char CHAR='c';
	final char PARA='p';
	char t;
	TextConverter builder;
	RTFReader(TextConverter obj) {
		builder=obj;
	}
	void parseRTF(Document doc) {
		while ((t=doc.getNextToken())!= EOF) {
			switch (t) {
				case CHAR: builder.convertCharacter(t);
				case PARA: builder.convertParagraph();
			}
		}
	}
}

//Client
public class Client {
	void createASCIIText(Document doc) {
		ASCIIConverter asciiBuilder = new ASCIIConverter();
		RTFReader rtfReader = new RTFReader(asciiBuilder);
		rtfReader.parseRTF(doc);
		ASCIIText asciiText = asciiBuilder.getResult();
	}
	public static void main(String args[]) {
		Client client=new Client();
		Document doc=new Document();
		client.createASCIIText(doc);
		system.out.println("This is an example of Builder Pattern");
	}
}
```

### Difference to Abstract Factory
Abstract factory: client uses factory method to create objects. Usually the object created is similar (e.g., from same base class)

Builder: builder class is instructed on how to create the object and then asked for it. Usually the object created has significant structural difference, and not with a common parent class


Prototype
=========

- Specifying the kind of objects to create using a prototypical instance
- Create new objects by copying prototype

![](/img/patterns-prototype.png)

### Implementation
A client use the prototype instance and clone method to create another instance. The prototype class provides interface to clone method and the concrete prototype implements the clone method for cloning itself:

```java
public interface Prototype {
	public abstract Object clone ( );
}

public class ConcretePrototype implements Prototype {
	public Object clone() {
		return super.clone();
	}
}

public class Client {
	public static void main( String arg[] ) {
		ConcretePrototype obj1 = new ConcretePrototype();
		ConcretePrototype obj2 = (ConcretePrototype)obj1.clone();
	}
}
```

### Applicability
Useful when classes to be instantiated are specified only at runtime and avoids the creation of a factory hierarchy.


Object Pool
===========
- Creational pattern (non-GoF)
- Offer mechanism to reuse objects that are expensive to create

### Implementation
ReusablePool object, usually implemented as a singleton, holds a pool of
objects and provide interfaces to manage them. Client calls the acquireReusable
method of ReusablePool  to get an object, and calls releaseReusable when
finished using the object.

From client's point of view, it cannot tell if ReusablePool is a factory class
that creates new object or returning a shared object from the resource pool.


Adapter
=======
- Structural pattern
- Convert the interface of a class into another interface the client expects

![](/img/patterns-adapter.png)

### Implementation
Usually as wrapper of objects to make incompatible type usable by client. The
wrapper provide the same interface of the target type, but invokes methods of
adaptee.

When the client invokes a method defined in the target type, the wrapper
invokes the adaptee's methods and translate the result.

### Issues
Adapter avoids modifying the adaptee classes. It can be implemented as
inheritance to target class, or multiple inheritance to both the target class
and adaptee class.


Bridge
======
- Structural pattern
- To decouple abstraction from implementation so that the two can vary independently
- C++ idiom: pimpl (pointer to implementation)

![](/img/patterns-bridge.png)

### Implementation
An abstraction class defines the interface and holds only a reference to the
implementation. A refined abstraction class derived from the abstraction class.

An implementor class defines the interface of the implementation. A concrete
implementor derived from the implementor class.

The actual implementation object is provided at runtime only, which makes the
implementation decoupled from the abstraction.

### Issues
Bridge pattern is often implemented using adapter pattern


Composite
=========
- Structural pattern
- Compose objects into tree structures to represent part-whole hierarchies
- Allow clients to treat compositions and individual objects uniformly

![](/img/patterns-composite.png)

### Implementation
Component base class as abstraction of both the leaves and composites. It
defines the interface.

Leaf class is derived from component base class but have no children.

Composite class is derived from component base class, and to store child
components. Composite implements methods defined in the base class by
delegating to child components; and additionally provide methods to add/remove
components


Decorator
=========
- Structural pattern
- Allow additional responsibilities added to an object dynamically without subclassing

![](/img/patterns-decorator.png)

### Implementation
A component base class, which the concrete component and the decorator are both
derived from it.

The decorator class holds a reference to a component object, and forward all
unmodified component methods to the component reference, while new methods may
be implemented or existing component methods may be overridden.

Then multiple decorators can be applied as a stack. This avoids subclassing all
the combinations.

### Issues
Decorator allows to add or alter behavior of an interface at runtime. Adapter
converts an interface to another. Facade provides a simpler interface than the
underlying one.


Facade
======
- Structural pattern
- Often used when a system is very complex or difficult to understand
- It hides the complexity and provides a simpler interface to the client

![](/img/patterns-facade.png)

### Example
```java
/* Complex parts */
class CPU {
	public void freeze() { ... }
	public void jump(long position) { ... }
	public void execute() { ... }
}

class HardDrive {
	public byte[] read(long lba, int size) { ... }
}

class Memory {
	public void load(long position, byte[] data) { ... }
}

/* Facade */
class ComputerFacade {
	private CPU processor;
	private Memory ram;
	private HardDrive hd;

	public ComputerFacade() {
		this.processor = new CPU();
		this.ram = new Memory();
		this.hd = new HardDrive();
	}

	public void start() {
		processor.freeze();
		ram.load(BOOT_ADDRESS, hd.read(BOOT_SECTOR, SECTOR_SIZE));
		processor.jump(BOOT_ADDRESS);
		processor.execute();
	}
}

/* Client */
class You {
	public static void main(String[] args) {
		ComputerFacade computer = new ComputerFacade();
		computer.start();
	}
}
```


Flyweight
=========
- Structural pattern
- Use sharing to support a large number of objects that have part of their internal state in common and other part can vary

![](/img/patterns-flyweight.png)

### Implementation
A flyweight object is usually immutable for safe sharing, or allow copy-on-write.

Sharing: Objects will hold a reference to flyweight object as internal attribute.


Proxy
=====
- Structural pattern
- Provide light version of heavy objects, which provides the same interface. The light version will instantiate costly objects only when they are really needed
- Proxy as placeholder for clients

![](/img/patterns-proxy.png)

### Implementation
Subject interface as base class for both the proxy and the real, costly subject.

The proxy maintains a reference to access the real subject, and control its access, creation, and destruction

Client handles the proxy in the same way it handles the real subject.

### Applicability
The proxy may act differently as the real subject depends on the role of it. It
can be used to delay the creation and initialization of expensive objects
(virtual proxy), provide a local representation of an object (remote proxy),
impose access control to real subjects (protection proxy), or simplify access
to objects, such as caching.


Chain of Responsibility
=======================
- Behavioral pattern
- Allows an object to send a command without knowing what object will receive and handle it
- Avoids attaching send of a request to its receiver, let other objects has the possibility to handle the request as well

![](/img/patterns-chainofresp.png)

### Implementation
A client sends command to handler. Handler instance has a linked list of
objects of handler type. If a concrete handler can handle the request then it
does so, otherwise it sends to its successor:

```java
public class Request {	
	private int m_value;
	public Request(int value) {
		m_value = value;
	}
	public int getValue() {
		return m_value;
	}
}

public abstract class Handler {
	protected Handler m_successor;
	public void setSuccessor(Handler successor) {
		m_successor = successor;
	}
	public abstract void handleRequest(Request request);
}

public class ConcreteHandlerOne extends Handler {
	public void handleRequest(Request request) {
		if (request.getValue() < 0) {
			//if request is eligible handle it
		} else {
			super.handleRequest(request);
		}
	}
 }

public class ConcreteHandlerTwo extends Handler {
	public void handleRequest(Request request) {
		if (request.getValue() > 0) {
			//if request is eligible handle it
		} else {
			super.handleRequest(request);
		}
	}
}

public class ConcreteHandlerThree extends Handler {
	public void handleRequest(Request request) {
		if (request.getValue() >= 0) {
			//if request is eligible handle it
		} else {
			super.handleRequest(request);
		}
	}
}

public class Main {
	public static void main(String[] args) {
		// Setup Chain of Responsibility
		Handler h1 = new ConcreteHandlerOne();
		Handler h2 = new ConcreteHandlerTwo();
		Handler h3 = new ConcreteHandlerThree();
		h1.setSuccessor(h2);
		h2.setSuccessor(h3);

		// Send requests to the chain
		h1.handleRequest(new Request(0));
	}
}
```


Command
=======
- Behavioral pattern
- Command = an object containing a request
- Allows commands saved in a queue

![](/img/patterns-command.png)

### Implementation
Command as abstract base class provides execute interface. Concrete command derived from command class, implements the execute method.
A client creates a concrete command object and sets the receiver, which knows how to perform operations. Invoker calls the execute method of the command object. Example:

```java
// command interface
public interface Order {
	public abstract void execute ( );
}

// Receiver class
class StockTrade {
	public void buy() { /* - */ }
	public void sell() { /* - */ }
}

// Invoker
class Agent {
	private m_ordersQueue = new ArrayList();
	public Agent() { /* - */ };
	void placeOrder(Order order) {
		ordersQueue.addLast(order);
		ordersQueue.getFirstAndRemove().execute();
	}	
}

// Concrete command
class BuyStockOrder implements Order {
	private StockTrade stock;
	public BuyStockOrder (StockTrade st) {
		stock = st;
	}
	public void execute( ) {
		stock.buy( );
	}
}

// Concrete command
class SellStockOrder implements Order { 
	private StockTrade stock;
	public SellStockOrder ( StockTrade st) {
		stock = st;
	}
	public void execute( ) {
		stock.sell( );
	}
}

// Client
public class Client {
	public static void main(String[] args) {
		StockTrade stock = new StockTrade();
		BuyStockOrder bsc = new BuyStockOrder (stock);
		SellStockOrder ssc = new SellStockOrder (stock);
		Agent agent = new Agent();

		agent.placeOrder(bsc); // Buy Shares
		agent.placeOrder(ssc); // Sell Shares
	}
}
```

### Applicability
- When the command added to queue and executes in different time
- If execute method memorize states: support undoable actions
- Allows high level operations at receiver defined as primitive operations in commands
- Decouples the object that invoke the action from the object that perform the
  action (producer-consumer design)

### Issues
Commands should help delegate request from clients to receiver. Something
should be sent from command to the receiver and the receiver should perform the
request.

Undo: either remember the state before each operation, or remember the command
and knows how to perform inverse operation. See memento pattern.

Command queue allows to run commands asynchronously in another thread.

Macro of commands: see composite pattern.


Interpreter
===========
- Behavioral pattern
- Use with a grammar, to interpret sentences in a language
- Decouples grammar from underlying expressions

![](/img/patterns-interpreter.png)

### Example
Each terminal or non-terminal symbol are represented by a class. Then use
composite pattern to represent a syntax tree and to evaluate the sentence.
There is a context object to maintain the states of input and output in the
midst of evaluation:

```java
public class Context {
	public String input;
	public int output;
}

public abstract class Expression {
	public void interpret(Context context) {
	  if (context.getInput().length() == 0) {
		return;
	  } else if (context.input.startsWith(nine())) {
		context.output = context.output + (9 * multiplier());
		context.input = context.input.substring(nine().length);
	  } else if (context.input.startsWith(four())) {
		context.output = context.output + (4 * multiplier());
		context.input = context.input.substring(four().length);
	  } else if (context.input.startsWith(five())) {
		context.output = context.output + (5 * multiplier());
		context.input = context.input.substring(five().length);
	  };
	  while (context.input.startsWith(one())) {
		context.output = context.output + (1 * multiplier());
		context.input = context.input.substring(one().length);
	  };
	};
	public abstract String one();
	public abstract String four();
	public abstract String five();
	public abstract String nine();
	public abstract int multiplier();
	
}

public class ThousandExpression  extends Expression{
	public String one() { return "M"; }
	public String four(){ return " "; }
	public String five(){ return " "; }
	public String nine(){ return " "; }
	public int multiplier() { return 1000; }
}
public class HundredExpression extends Expression{
	public String one() { return "C"; }
	public String four(){ return "CD"; }
	public String five(){ return "D"; }
	public String nine(){ return "CM"; }
	public int multiplier() { return 100; }
}
public class TenExpression extends Expression{
	public String one() { return "X"; }
	public String four(){ return "XL"; }
	public String five(){ return "L"; }
	public String nine(){ return "XC"; }
	public int multiplier() { return 10; }
}
public class OneExpression extends Expression{
	public String one() { return "I"; }
	public String four(){ return "IV"; }
	public String five(){ return "V"; }
	public String nine(){ return "IX"; }
	public int multiplier() { return 1; }
}

public class MainInterpreter {
	public static void main(String[] args) {
		String roman = "MCMXXVIII";
		Context context = new Context();
		Context.input = roman;

		ArrayList<Expression> parsetree = new ArrayList<Expression>();
		parsetree.add(new ThousandExpression());
		parsetree.add(new HundredExpression());
		parsetree.add(new TenExpression());
		parsetree.add(new OneExpression());

		for (Iterator it = parsetree.iterator(); it.hasNext();) {
			((Expression)it.next()).interpret(context);
		}
		System.out.println(roman + " = " + Integer.toString(context.output));
	}
}
```


Iterator
========
- Behavioral pattern
- Access elements of an aggregate object sequentially without exposing its underlying representation

![](/img/patterns-iterator.png)

### Implementation
Iterator object to an aggregate object implements next method to advance the
iterator, and with methods to tell if the iterator reaches the end of the
sequence.


Mediator
========
- Behavioral pattern
- Define an object that encapsulates how a set of objects interact, so that other objects should not aware of the existence of other objects
- Promotes loose coupling, allows changing object interactions independently and explicitly

![](/img/patterns-mediator.png)

### Example
Mediator should be used when a set of objects communicate in well-defined but
complex ways, with unstructured interdependencies. By decoupling objects from
their communication, it makes subclassing easier and the communication
customizable.

GUI example: Dialog window as mediator for various widgets. When a combobox
updates, the dialog helps to update a label, without letting the label aware of
the existence of the combobox.

### Issues
Mediator can implement observer pattern: mediator is an observer and
encapsulating objects are observable.

Because the mediator handles all logic between encapsulating objects, beware
that it becomes more and more complex.

### Related patterns
- Facade: Simplified mediator if the mediator is the only active class
- Adapter: Mediator pattern when the mediator changes the messages en route between sender and receiver
- Observer: Handles communication between observers and observables, where observables are easily added. Mediator pattern usually supposed to have the mediator inherited for extension.


Memento
=======
- Behavioral pattern
- To capture and restore internal state of an object without violating encapsulation

![](/img/patterns-memento.png)

### Implementation
Memento object stores internal state of originator object, which includes any
number of state variables. It provides get and set methods to update its state.

Originator creates a memento object capturing its internal state and uses the
memento object to restore its previous state.

Caretaker keeps the memento object but not operate on it. Memento is opaque to
the caretaker.

Caretaker operates on originator: It calls originator's createMemento() method
to keep the originator's state before other operation. If rollback is required,
it calls the originator's setMemento() method with the appropriate memento to
restore to a previous state.


Observer
========
- Behavioral pattern
- Defines one-to-many dependency between objects so that when one changes state, all its dependents are updated

![](/img/patterns-observer.png)

### Implementation
An observable class (a.k.a. subject) that defines operations for attaching and
detaching observers. A concrete observable maintains the state and notifies
observers upon state change.

Observers implements interface for observable objects to notify about change.

### Examples
The model-view-controller architectural pattern: The view represents the
observer and model is observable object.

### Issues
Observable should be in a consistent state before notifying observers. That is,
state update should be finish before notification. In case observable is a
derived class and state update is also done in the base class, we should have
the state update in separate function from notification:

```java
class Observable{
	...
	int state = 0;
	public void final updateState(int increment) {
		doUpdateState(increment);
		notifyObservers();
	}
	public void doUpdateState(int increment) {
		state = state + increment;		
	}
	...
}

class ConcreteObservable extends Observable{
	...
	int additionalState = 0;
	public doUpdateState(int increment){
		super.doUpdateState(increment);
		additionalState = additionalState + increment;
	}
	...
}
```


State
=====
- Behavioral pattern
- State machine: each individual state implemented as a derived class of the state pattern interface, and transitions implemented by invoking methods defined by the base class

![](/img/patterns-state.png)


Strategy
========
- Behavioral pattern
- Allow an algorithm's behavior to be selected at runtime
- Defines a family of algorithms (under a common interface) and makes algorithms interchangeable within that family

![](/img/patterns-strategy.png)

### Issues
Data to strategies: A context object should be used along strategy pattern to hold data. Input is provided by the context and output should be set by the strategy object into the context


Template method
===============
- Behavioral pattern
- define algorithm in base class using abstract operations for use in concrete classes, with some steps deferred to subclasses

![](/img/patterns-template-method.png)

### Implementation
Algorithm is defined in base class. Derived classes should not call base class
methods (the inverted control structure). Derived classes are to implement
"primitive methods” that the algorithm uses.

### Issues
Template method relies on inheritance of classes.

The template methods defined in the base class should never be overridden. Only
the "customization hooks” should be overridden in subclasses. This follows the
open-close principle (OCP)


Visitor
=======
- Behavioral pattern
- Represent an operation to be performed on elements of an object structure
- Allows to define new operation without changing the classes of elements on which it operates

![](/img/patterns-visitor.png)

### Implementation
Visitor base class declare visit operation interface for all types of visitable
classes. Concrete visitor implements all visit methods. Usually visit methods
are named the same for different visitable classes. It depends on polymorphism
to get the correct method to execute.

An aggregate object structure holds a number of visitable objects. Each
visitable class implements an accept method as the entry point for visitor
object.

### Issues
Visitor pattern is similar to iterator pattern. Iterator usually expect object
of the same type but visitor can be used on complex structures such as
hierarchical or composite. Consequently, visitor implementation will contain
sequence of if-blocks to check the object type.


Null object
===========
- Behavioral pattern (non-GoF)
- Provide an object as a surrogate for lack of object of a given type, or to represent "do nothing”

### Implementation
Implement the same interface as a real object (e.g., real operation) but
perform no-op in reality. Having a null object avoids the control flow to check
if real operation shall be done.

OO Design Principles
====================

### Single responsibility (SRP, of SOLID)
- A class should have only one reason to change
- If we have 2 reasons to change a class, we have to split the functionality into two classes

### Open-close (OCP, of SOLID)
- open for extension, closed for modification
- e.g., by passing a class object to another class object, function can change without the need to create derived class (see strategy pattern)
- If a new class is started by cut-and-paste of its parent class, likely a violation of OCP. Template pattern is the rescue.

### Liskov's substitution (LSP, of SOLID)
- If a class is derived from a base class, the reference to the base class can be replaced with a derived class without affecting the functionality
- i.e., derived class must be completely substitutable for their base class:
- If for each object o1 of type S there is an object o2 of type T such that for all programs P defined in terms of T, the behavior of P is unchanged when o1 is substituted for o2 then S is a subtype of T
- Polymorphism without any need to know the exact subtype of an object. If you need type checking and type conversion, likely a violation of LSP. Consider visitor pattern to handle the type-specific functions.

### Interface segregation (ISP, of SOLID)
- Clients should not be forced to implement interfaces they don't use
- "Fat interface” (a.k.a. "polluted interface”) should be avoided, i.e., user should not be forced to write dummy methods

### Dependency inversion (DSP, of SOLID)
- No high-level class (e.g., logic) should depend on low-level classes (e.g., mechanics). They should depend on abstractions
- Abstraction should not depend on details; details should depend on abstractions
- Provide flexibility to allow high-low level coupling changed

### Delegation, Aggregation, Composition
- Alternative to inheritance, about how to reuse code defined in another class
- Delegation: Loose connection to another class. Call other class' method when necessary. Method is executed in the context of encapsulating class. Encapsulating class may not even contain the instance of the other class
- Aggregation: Instances of other class created and managed elsewhere and passed on to the encapsulating class, often swappable.
- Composition: Tight connection. Instances of other classes created and managed inside encapsulating classes, and destroyed at destruction of the encapsulating class


# Reference

- <http://www.oodesign.com>
- <https://dzone.com/refcardz/design-patterns>
- <https://softwareengineering.stackexchange.com/questions/178488/lsp-vs-ocp-liskov-substitution-vs-open-close>

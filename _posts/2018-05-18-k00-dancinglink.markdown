---
layout: post
title: Knuth (2000) Dancing Link
date: Fri, 18 May 2018 17:21:53 -0400
tags: code
category: paper
bibtex:
  type: inproceedings 
  title: "Dancing Links"
  author: Donald E. Knuth
  year: 2000
  booktitle: {Millenial Perspectives in Computer Science: Proceedings of the 1999 Oxford-Microsoft Symposium in Honour of Sir Tony Hoare}
  pages: 187--214
  eprint: arXiv:cs/0011047
---

A paper that is easy to read, summarizing the development of algorithm to set
cover problem. The name "dancing link" is how Knuth called his implementation of
depth-first-search. Wikipedia has an article about that, named [Knuth's
Algorithm X](https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X).

Below is [my implementation](https://gist.github.com/righthandabacus/aae789127ff384e82a522a8cccad6277)
using modern C++, trying to mimick exactly the algorithm depicted in the paper
without obscure optimization:

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <memory> // for shared_ptr
#include <unordered_set>
#include <list>
#include <cassert>

using namespace std;

//////////////////////
// Input data

// const vector<const T> not allowed because vector will silently copy elements on expansion
const vector<vector<int> > PROBLEM_INPUT{
	{ 0, 0, 1, 0, 1, 1, 0 }
   ,{ 1, 0, 0, 1, 0, 0, 1 }
   ,{ 0, 1, 1, 0, 0, 1, 0 }
   ,{ 1, 0, 0, 1, 0, 0, 0 }
   ,{ 0, 1, 0, 0, 0, 0, 1 }
   ,{ 0, 0, 0, 1, 1, 0, 1 }
};

//////////////////////
// Node structure
class node;
typedef shared_ptr<node> nodeptr;
class node
{
	public:
		node(nodeptr _u=nullptr, nodeptr _d=nullptr, nodeptr _l=nullptr, nodeptr _r=nullptr);
		node(const string _name, nodeptr _u=nullptr, nodeptr _d=nullptr, nodeptr _l=nullptr, nodeptr _r=nullptr);
		const string name;  // set if columns
		int size;           // -1 if node, >=0 for columns
		nodeptr U;
		nodeptr D;
		nodeptr L;
		nodeptr R;
};
node::node(nodeptr _u, nodeptr _d, nodeptr _l, nodeptr _r) :
	size(-1), U(_u), D(_d), L(_l), R(_r)
{};

// _name is string value type, not reference, for the ctor will make a copy of
// it and C++11 will use std::move when possible
node::node(const string _name, nodeptr _u, nodeptr _d, nodeptr _l, nodeptr _r) :
	name(_name), size(0), U(_u), D(_d), L(_l), R(_r)
{};

//////////////////////
// build data structure from input data

// add a *brand new* node to left of a existing node in a circular list
auto addnode_left = [] (nodeptr old, nodeptr n) { n->L = old->L; n->R = old; old->L = old->L->R = n; };

nodeptr build(const vector<vector<int> > input)
{
	auto root = make_shared<node>(); // "root" node, points to columns
	root->L = root->R = root; // circular list of itself
	// set up the column linked list, from left to right
	for (unsigned i=0; i<input[0].size(); ++i) {
		// add node h to immediate left of root, circularly
		auto h = make_shared<node>(string(1,char('A'+i)));
		addnode_left(root, h);
		// up/down ptr point to column header itself
		h->D = h->U = h;
	};
	// set up data, one row at a time
	for (auto row : input) {
		auto h = root;
		nodeptr firstnode = nullptr;
		for (auto cell : row) {
			h = h->R; // h := scan each column header node from left to right
			if (cell == 1) {
				auto newnode = make_shared<node>(h->U, h); // input data is "1" at this position
				h->U = h->U->D = newnode; // header's up = bottom-most row with "1" so far
				h->size++;
				if (!firstnode) {
					firstnode = newnode->L = newnode->R = newnode; // first node in a row, make circular to itself
				} else {
					addnode_left(firstnode, newnode);   // add node to row's circular list
				};
			};
		};
	};
	return root;
};

// return the column header node of a particular cell
auto colnode = [] (nodeptr cell) -> nodeptr { while (cell->size == -1) cell = cell->U; return cell; };

void printgrid(nodeptr root)
{
	unordered_set<string> visited; // name of visited columns
	for (auto h=root->R; h!=root; h=h->R) { // h := scan each column from left onward
		for (auto n=h->D; n!=h; n=n->D) {   // n := row asserted in column h from topmost downward
			// find all asserted cell in this row
			string rowname = h->name;
			auto m = n->R;   // m := other asserted cells in same row as n
			for (; m!=n; m=m->R) { // scan each asserted position across the row
				// look for the column header to find the column name
				auto p = colnode(m);
				if (visited.find(p->name) != visited.end()) {
					// this row should have printed
					break;
				};
				rowname += p->name; // append column name to build the row name
			};
			if (m == n) { // loop end at m==n means never break
				cout << rowname << endl;
			};
		};
		visited.insert(h->name); // remember this column so not print rows asserted on this anymore
	};
};

//////////////////////
// solver, as in page 5 of the paper

// remove a node from circular list
auto remove_hori = [] (nodeptr n) { n->R->L = n->L; n->L->R = n->R; };
auto remove_vert = [] (nodeptr n) { n->D->U = n->U; n->U->D = n->D; };
// restore a node back to circular list
auto restore_hori = [] (nodeptr n) { n->R->L = n->L->R = n; };
auto restore_vert = [] (nodeptr n) { n->D->U = n->U->D = n; };

// cover a column, as in page 6
void cover(nodeptr column)
{
	assert(column->size >= 0); // make sure it is a column
	// remove column from header linked list
	remove_hori(column);
	// scan each node down the column for asserted rows
	for (auto row=column->D; row!=column; row=row->D) {
		for (auto cell=row->R; cell!=row; cell=cell->R) { // scan cell on each row from left
			// remove cell from column
			remove_vert(cell);
			// decrement size count
			colnode(cell)->size--;
			assert(colnode(cell)->size >= 0);
		};
	};
};

// uncover a column, as in page 6
void uncover(nodeptr column)
{
	assert(column->size >= 0); // make sure it is a column
	// scan each node up the column for asserted rows
	for (auto row=column->U; row!=column; row=row->U) {
		for (auto cell=row->L; cell!=row; cell=cell->L) { // scan cell on each row from right
			// increment size count
			assert(colnode(cell)->size >= 0);
			colnode(cell)->size++;
			// add cell back to column
			restore_vert(cell);
		};
	};
	// restore column to header linked list
	restore_hori(column);
};


#ifndef NDEBUG
// for LLDB: Explicitly instantiate any STL stuff you need in order to debug
// <http://lists.llvm.org/pipermail/lldb-dev/2017-February/011889.html>
template class std::vector<vector<int> >;
template class std::unordered_set<string>;
template class std::list<nodeptr>;
#endif

void solvegrid(nodeptr root, bool minbranching = false, list<nodeptr > solution = {})
{
	auto c = root->R; // first column in the list
	if (c == root) {
		// header list empty, print current solution
		for (auto row : solution) {
			string rowname = colnode(row)->name;
			for (auto cell=row->R; cell!=row; cell=cell->R) {
				rowname += colnode(cell)->name;
			};
			cout << rowname << endl;
		};
		return;
	};
	// optional: if min column approach, find the column with smallest size
	if (minbranching) {
		int minsize = c->size;
		for (auto cc=c->R; cc!=root; cc=cc->R) {
			if (cc->size < minsize) {
				minsize = cc->size;
				c = cc;
			};
		};
	};
	cover(c);
	for (auto r=c->D; r!=c; r=r->D) {
		solution.push_back(r);
		for (auto j=r->R; j!=r; j=j->R) {
			cover(colnode(j)); // remove colnode(j) from header list and conflicting rows from corresponding columns
		};
		// as c removed from root's linked list, recursion on this function will check another column
		solvegrid(root, minbranching, solution);
		// this step of recursion done, restore before moving on
		solution.pop_back();
		auto c = colnode(r);
		for (auto j=r->L; j!=r; j=j->L) {
			uncover(colnode(j));
		};
	};
	uncover(c);
};

int main()
{
	auto h = build(PROBLEM_INPUT);
	cout << "built problem with rows:" << endl;
	printgrid(h);
	cout << "solution (without min-branching):" << endl;
	solvegrid(h, false);
	cout << "solution (with min-branching):" << endl;
	solvegrid(h, true);
	return 0;
};
```

Something learned:

- LLDB (GDB is not available in default installation of macOS) has some hassle
  in dealing with STL-defined data types. To make it work, we have to explicitly
  instantiate them, see the code between `#ifndef NDEBUG` and `#endif`
- I tried to make an iterator type for the grid-style linked list, but it turns
  out to be a overkill for this situation
- Without using STL for the `node` class (can't find a good candidate to
  implement grid structure), it makes the code slightly messy, for example, I
  cannot use `accumulate()` or `reduce()` function without implementing an
  iterator
- The grid is built of `node` but there are two types of nodes: column headers
  that carries a column name and data nodes representing the asserted element in
  the input array. Modern C++ has `variant<>` or `any<>` but using them will
  require a `try`-`catch` structure, which `if`-`else` is neater in comparison

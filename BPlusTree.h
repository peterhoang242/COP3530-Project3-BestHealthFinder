#pragma once
#include <string>
#include <iostream>
#include <tuple>
#include <stack>
using namespace std;


struct Node {	// Used for leaf nodes of the B+ tree
	string name1;
	string name2;
	int data1;
	int data2;
	Node* next;
	int size;		// How many keys are currently in the node
	bool isLeaf;	// Is the node a leaf node?

	// Used if node isn't a leaf node
	Node* left;
	Node* middle;
	Node* right;
	Node();
	Node(string _name1, string _name2, int _data1, int _data2, Node* _next, int _size, bool _isLeaf, Node* _left, Node* _middle, Node* _right);
};

class BPlusTree {
	Node* head;
	Node* leafStart;	// Start of leaves linked list
	int size;

public:
	BPlusTree();
	tuple<bool, string, int> insertHelper(string name, int data, Node* root, Node* parent = nullptr);
	void insert(string _name, int _data);
	stack<pair<string, int>> descendingList();	// Returns a stack of string/int pairs (hospital name and data), top of the stack to the bottom is greatest to least
};


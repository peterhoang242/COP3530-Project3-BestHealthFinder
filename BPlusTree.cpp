#include "BPlusTree.h"
#include <string>
#include <iostream>
#include <tuple>
#include <stack>
using namespace std;
#pragma once


Node::Node() {
	name1 = "";
	name2 = "";
	data1 = 2147483647;
	data2 = 2147483647;
	next = nullptr;
	size = 0;
	isLeaf = true;
	left = nullptr;
	middle = nullptr;
	right = nullptr;
}

BPlusTree::BPlusTree() {
	head = nullptr;
	leafStart = nullptr;
	size = 0;
}

Node::Node(string _name1, string _name2, int _data1, int _data2, Node* _next, int _size, bool _isLeaf, Node* _left, Node* _middle, Node* _right) {
	name1 = _name1;
	name2 = _name2;
	data1 = _data1;
	data2 = _data2;
	next = _next;
	size = _size;
	isLeaf = _isLeaf;
	left = _left;
	middle = _middle;
	right = _right;
}


tuple<bool, string, int> BPlusTree::insertHelper(string name, int data, Node* root, Node* parent) {
	tuple<bool, string, int> output = make_tuple(false, name, data);	// Used for correcting overflows in the B+ Tree nodes: bool is whether there is an overflow (true if so), string and int is the information that is to be copied up
	bool isOverflow = false;

	if (root->isLeaf == false) {
		if (data < root->data1) {
			output = insertHelper(name, data, root->left, root);
			isOverflow = get<0>(output);
		}
		else if (data > root->data2) {
			output = insertHelper(name, data, root->right, root);
			isOverflow = get<0>(output);
		}
		else {
			output = insertHelper(name, data, root->middle, root);
			isOverflow = get<0>(output);
		}
	}
	else {
		if (root->size < 2) {
			if (root->size == 0) {
				root->name1 = name;
				root->data1 = data;
				root->size++;
			}
			else if (data < root->data1) {
				root->name2 = root->name1;
				root->data2 = root->data1;
				root->name1 = name;
				root->data1 = data;
				root->size++;
			}
			else if (data >= root->data1) {
				root->name2 = name;
				root->data2 = data;
				root->size++;
			}
		}
		else {
			isOverflow = true;
		}
	}

	if (isOverflow == true) {
		string outputName = get<1>(output);
		int outputData = get<2>(output);
		Node* newNode = new Node();

		if (root->isLeaf == true) {
			root->next = newNode;
		}

		if (data < root->data2) {
			newNode->name1 = outputName;
			newNode->data1 = outputData;
			newNode->size++;
			newNode->name2 = root->name2;
			newNode->data2 = root->data2;
			newNode->size++;
			root->name2 = "";
			root->data2 = 2147483647;
			root->size--;
		}
		else if (data >= root->data2) {
			newNode->name1 = root->name2;
			newNode->data1 = root->data2;
			newNode->size++;
			newNode->name2 = outputName;
			newNode->data2 = outputData;
			newNode->size++;
			root->name2 = "";
			root->data2 = 2147483647;
			root->size--;
		}

		if (outputName != name && outputData != data) {
			newNode->left = root->right;
			newNode->middle = newNode->left->next;
			root->right = nullptr;
		}


		// Copy smallest key from new second node to the parent
		if (parent == nullptr) {
			Node* newParent = new Node();
			newParent->isLeaf = false;
			newParent->left = root;
			newParent->middle = newNode;
			if (head == root) {
				head = newParent;
			}

			newParent->name1 = newNode->name1;
			newParent->data1 = newNode->data1;
			newParent->size++;

			isOverflow = false;
		}
		else {
			if (parent->middle == root) {
				parent->right = newNode;
			}

			if (parent->size < 2) {
				if (newNode->data1 < parent->data1) {
					parent->name2 = parent->name1;
					parent->data2 = parent->data1;
					parent->name1 = newNode->name1;
					parent->data1 = newNode->data1;
					parent->size++;
				}
				else if (newNode->data1 >= parent->data1) {
					parent->name2 = newNode->name1;
					parent->data2 = newNode->data1;
					parent->size++;
				}

				isOverflow = false;
			}
			else {
				get<1>(output) = newNode->name1;
				get<2>(output) = newNode->data1;
			}
		}

		if (root->isLeaf == false) {
			newNode->name1 = outputName;
			newNode->data1 = outputData;
			newNode->name2 = "";
			newNode->data2 = 2147483647;
			newNode->size--;
		}
	}

	get<0>(output) = isOverflow;
	return output;
}

void BPlusTree::insert(string _name, int _data) {
	if (size == 0) {
		Node* newNode = new Node();
		newNode->name1 = _name;
		newNode->data1 = _data;
		newNode->size++;
		head = newNode;
		leafStart = newNode;
		size++;
	}
	else {
		insertHelper(_name, _data, head);
	}
}

stack<pair<string, int>> BPlusTree::descendingList() { // Returns a stack of string/int pairs (hospital name and data), top of the stack to the bottom is greatest to least
	stack<pair<string, int>> dataList;
	Node* currentNode = leafStart;

	while (currentNode->next != nullptr) {
		dataList.push(make_pair(currentNode->name1, currentNode->data1));
		dataList.push(make_pair(currentNode->name2, currentNode->data2));
	}

	return dataList;
}	

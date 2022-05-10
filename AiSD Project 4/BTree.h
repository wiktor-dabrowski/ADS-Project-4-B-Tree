#pragma once
#include <iostream>
#include <sstream>
#include "node.h"

using namespace std;

class BTree
{
	node* root;
	int t;
public:
	BTree(int);
	~BTree();

	void insert(int);
	void search(int);
	void print();
	void load();
	void save();
	void remove(int);
	void calculate_cache(int);
	void calculate_alt_cache(int);
};


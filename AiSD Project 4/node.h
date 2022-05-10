#pragma once
#include <iostream>
#include <string>

using namespace std;

struct node
{
	int* keys;
	node** sons;
	int keys_nr = 0;
	bool is_leaf = true;
	int t;

	node(int);
	~node();
	void clear_tree();

	node* search(int);
	void insert(int);
	void split(int, node*);

	void print();
	void save_print();
	void load();

	void remove(int);
	int replace_prev(int);
	int replace_next(int);
	void remove_non_existing_in_node(int);

	bool cache_search(int, int*);
};
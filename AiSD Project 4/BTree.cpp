#include "BTree.h"

BTree::BTree(int _t) {
	t = _t;
	root = new node(t);
}
BTree::~BTree() {
	root->clear_tree();
	delete root;
}
void BTree::insert(int x) {
	if (root->keys_nr == 2 * t - 1) {
		node* new_root = new node(t);
		new_root->is_leaf = false;

		new_root->sons[0] = root;
		new_root->split(0, root);

		int index = 0;
		if (new_root->keys[0] < x)
			index++;
		new_root->sons[index]->insert(x);
		root = new_root;
	}
	else
		root->insert(x);
}
void BTree::search(int x) {
	if (root->search(x))
		cout << x << " +" << endl;
	else
		cout << x << " -" << endl;
}
void BTree::print() {
	root->print();
	cout << endl;
}
void BTree::load() {
	root->load();
}
void BTree::save() {
	cout << t << endl;
	root->save_print();
	cout << endl;
}
void BTree::remove(int x) {
	if (root->search(x)) {
		root->remove(x);

		if (root->keys_nr == 0) {
			node* tmp = root;
			if (root->is_leaf)
				root = new node(t);
			else
				root = root->sons[0];

			delete tmp;
		}
	}
}

void BTree::calculate_cache(int x) {
	int* cache_table = new int[x];
	for (int i = 0; i < x; i++)
		cache_table[i] = NULL;

	string line = "";
	getline(cin, line);
	istringstream iss(line);

	int n, read_nr;
	int cache_front = 0;
	int without_cache_read = 0;
	int with_cache_read = 0;
	while (iss >> n) {
		read_nr = 0;
		root->cache_search(n, &read_nr);
		without_cache_read += read_nr;
		bool found_in_cache = false;
		for (int i = 0; i < x; i++) {
			if (cache_table[i] == n) {
				found_in_cache = true;
				break;
			}
		}
		if (!found_in_cache) {
			with_cache_read += read_nr;
			cache_table[cache_front] = n;
			cache_front = (cache_front + 1) % x;
		}
	}

	cout << "NO CACHE: " << without_cache_read << " CACHE: " << with_cache_read << endl;

	delete[] cache_table;
}

void BTree::calculate_alt_cache(int x) {
	int* cache_table = new int[x];
	int* cache_table_nr = new int[x];
	for (int i = 0; i < x; i++) {
		cache_table[i] = NULL;
		cache_table_nr[i] = 0;
	}
		

	string line = "";
	getline(cin, line);
	istringstream iss(line);

	int n, read_nr;
	int without_cache_read = 0;
	int with_cache_read = 0;
	while (iss >> n) {
		read_nr = 0;
		root->cache_search(n, &read_nr);
		without_cache_read += read_nr;
		bool found_in_cache = false;
		for (int i = 0; i < x; i++) {
			if (cache_table[i] == n) {
				found_in_cache = true;
				cache_table_nr[i]++;
				break;
			}
		}
		if (!found_in_cache) {
			with_cache_read += read_nr;
			int min = cache_table_nr[0];
			int min_index = 0;
			for (int i = 1; i < x; i++) {
				if (cache_table_nr[i] == 0) {
					min_index = i;
					break;
				}
				if (cache_table_nr[i] < min) {
					min = cache_table_nr[i];
					min_index = i;
				}
			}
			cache_table[min_index] = n;
			cache_table_nr[min_index] = 1;
		}
	}

	cout << "NO CACHE: " << without_cache_read << " ALT CACHE: " << with_cache_read << endl;

	delete[] cache_table;
	delete[] cache_table_nr;
}
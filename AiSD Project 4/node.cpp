#include "node.h"

node::node(int _t) {
	t = _t;

	keys = new int[2 * t - 1];
	sons = new node * [2 * t];
}
node::~node() {
	delete[] keys;
	delete[] sons;
}
void node::clear_tree() {
	if (is_leaf)
		return;
	else {
		for (int i = 0; i < keys_nr + 1; i++) {
			sons[i]->clear_tree();
			delete sons[i];
		}
	}
}

node* node::search(int x) {
	int index = 0;
	for (int i = 0; i < keys_nr; i++) {
		if (keys[i] >= x)
			break;
		index++;
	}

	if (index < keys_nr && keys[index] == x)
		return this;
	else {
		if (is_leaf)
			return nullptr;
		else
			return sons[index]->search(x);
	}
}
void node::insert(int x) {
	int index = keys_nr - 1;

	if (is_leaf)
	{
		for (int i = index; i >= 0; i--) {
			if (keys[index] <= x)
				break;

			keys[index + 1] = keys[index];
			index--;
		}

		keys[index + 1] = x;
		keys_nr++;
	}
	else
	{
		for (int i = index; i >= 0; i--) {
			if (keys[index] <= x)
				break;
			index--;
		}

		if (sons[index + 1]->keys_nr == 2 * t - 1)
		{
			split(index + 1, sons[index + 1]);

			if (keys[index + 1] < x)
				index++;
		}
		sons[index + 1]->insert(x);
	}
}
void node::split(int index, node* to_split) {
	node* second = new node(t);
	second->is_leaf = to_split->is_leaf;
	second->keys_nr = t - 1;

	for (int i = 0; i < t - 1; i++)
		second->keys[i] = to_split->keys[i + t];

	if (!to_split->is_leaf)
	{
		for (int i = 0; i < t; i++)
			second->sons[i] = to_split->sons[i + t];
	}

	to_split->keys_nr = t - 1;

	for (int i = keys_nr; i >= index + 1; i--)
		sons[i + 1] = sons[i];

	sons[index + 1] = second;

	for (int i = keys_nr - 1; i >= index; i--)
		keys[i + 1] = keys[i];

	keys[index] = to_split->keys[t - 1];

	keys_nr++;
}

void node::print() {
	if (is_leaf) {
		for (int i = 0; i < keys_nr; i++)
			cout << keys[i] << " ";
	}
	else {
		sons[0]->print();
		for (int i = 0; i < keys_nr; i++) {
			cout << keys[i] << " ";
			sons[i + 1]->print();
		}
	}
}
void node::save_print() {
	cout << "( ";
	if (is_leaf) {
		for (int i = 0; i < keys_nr; i++)
			cout << keys[i] << " ";
	}
	else {
		sons[0]->save_print();
		for (int i = 0; i < keys_nr; i++) {
			cout << keys[i] << " ";
			sons[i + 1]->save_print();
		}
	}
	cout << ") ";
}
void node::load() {
	string word = "";
	while (word[0] != ')') {
		cin >> word;

		if (word[0] >= '0' && word[0] <= '9')
			keys[keys_nr++] = stoi(word);
		if (word[0] == '(') {
			sons[keys_nr] = new node(t);
			sons[keys_nr]->load();
			is_leaf = false;
		}
	}
}

void node::remove(int x) {
		int index = 0;
		while (index < keys_nr - 1 && keys[index] < x)
			index++;
		node* y = sons[index];
		node* z = sons[index + 1];

		if (is_leaf && keys[index] == x) {
			for (int i = index; i < keys_nr - 1; i++)
				keys[i] = keys[i + 1];
			keys_nr--;
		}
		else if (keys[index] == x) {
			if (y->keys_nr >= t) {
				keys[index] = y->replace_prev(x);
				y->remove(keys[index]);
			}
			else if (z->keys_nr >= t) {
				keys[index] = z->replace_next(x);
				z->remove(keys[index]);
			}	
			else {
				y->keys[y->keys_nr++] = x;
				for (int i = 0; i < z->keys_nr; i++) {
					y->sons[y->keys_nr] = z->sons[i];
					y->keys[y->keys_nr++] = z->keys[i];
				}
				y->sons[y->keys_nr] = z->sons[z->keys_nr];

				delete z;
				for (int i = index; i < keys_nr - 1; i++) {
					keys[i] = keys[i + 1];
					sons[i + 1] = sons[i + 2];
				}
				keys_nr--;

				y->remove(x);
			}
		}
		else if (!is_leaf) {
			if (keys[index] > x) {
				if (y->keys_nr == t - 1)
					remove_non_existing_in_node(index);

				y->remove(x);
			}
			else if (keys[index] < x) {
				if (z->keys_nr == t - 1)
					remove_non_existing_in_node(index + 1);

				z->remove(x);
			}
		}
}
int node::replace_prev(int x) {
	if (is_leaf) {
		int result = keys[keys_nr - 1];

		return result;
	}
	else
		return sons[keys_nr]->replace_prev(x);
}
int node::replace_next(int x) {
	if (is_leaf) {
		int result = keys[0];

		return result;
	}
	else
		return sons[0]->replace_next(x);
}
void node::remove_non_existing_in_node(int index) {
	node* v = nullptr;
	node* y = nullptr;
	node* z = nullptr;
	if (index - 1 >= 0)
		v = sons[index - 1];
	y = sons[index];
	if (index + 1 <= keys_nr)
		z = sons[index + 1];

	if (v && v->keys_nr >= t) {
		for (int i = y->keys_nr; i > 0; i--) {
			y->sons[i + 1] = y->sons[i];
			y->keys[i] = y->keys[i - 1];
		}
		y->sons[1] = y->sons[0];
		y->keys_nr++;

		y->keys[0] = keys[index - 1];
		y->sons[0] = v->sons[v->keys_nr];
		keys[index - 1] = v->keys[--v->keys_nr];
	}
	else if (z && z->keys_nr >= t) {
		y->keys[y->keys_nr++] = keys[index];
		y->sons[y->keys_nr] = z->sons[0];
		keys[index] = z->keys[0];

		for (int i = 1; i < z->keys_nr; i++) {
			z->sons[i - 1] = z->sons[i];
			z->keys[i - 1] = z->keys[i];
		}
		z->sons[z->keys_nr - 1] = z->sons[z->keys_nr];
		z->keys_nr--;
	}
	else {
		if (v) {
			for (int i = y->keys_nr - 1; i >= 0; i--) {
				y->sons[i + t] = y->sons[i];
				y->keys[i + t] = y->keys[i];
			}
			y->sons[y->keys_nr + t] = y->sons[y->keys_nr];

			for (int i = 0; i < v->keys_nr; i++) {
				y->sons[i] = v->sons[i];
				y->keys[i] = v->keys[i];
			}
			y->sons[v->keys_nr] = v->sons[v->keys_nr];
			y->keys[t - 1] = keys[index - 1];
			y->keys_nr = 2 * t - 1;

			delete v;
			for (int i = index - 1; i < keys_nr - 1; i++) {
				sons[i] = sons[i + 1];
				keys[i] = keys[i + 1];
			}
			sons[keys_nr - 1] = sons[keys_nr];
			keys_nr--;
		}
		else if (z) {
			y->keys[y->keys_nr++] = keys[index];
			for (int i = 0; i < t - 1; i++) {
				y->sons[y->keys_nr] = z->sons[i];
				y->keys[y->keys_nr++] = z->keys[i];
			}
			y->sons[y->keys_nr] = z->sons[z->keys_nr];

			delete z;
			for (int i = index; i < keys_nr - 1; i++) {
				keys[i] = keys[i + 1];
				sons[i + 1] = sons[i + 2];
				
			}
			keys_nr--;
		}
	}
}

bool node::cache_search(int x, int* read_nr) {
	(*read_nr)++;
	int index = 0;
	for (int i = 0; i < keys_nr; i++) {
		if (keys[i] >= x)
			break;
		index++;
	}

	if (keys[index] == x)
		return true;
	else {
		if (is_leaf)
			return false;
		else
			return sons[index]->cache_search(x, read_nr);
	}
}
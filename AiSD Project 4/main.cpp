#include <iostream>
#include <string>
#include "BTree.h"

using namespace std;

int main() {
	BTree* tree = nullptr;
	char command = '~';
	string to_ignore;
	int x;

	while (command != 'X') {
		cin >> command;

		switch (command) {
		case 'I':
			cin >> x;
			tree = new BTree(x);
			break;
		case 'A':
			cin >> x;
			tree->insert(x);
			break;
		case '?':
			cin >> x;
			tree->search(x);
			break;
		case 'P':
			tree->print();
			break;
		case 'L':
			cin >> x >> to_ignore;
			if (tree)
				delete tree;
			tree = new BTree(x);
			tree->load();
			break;
		case 'S':
			tree->save();
			break;
		case 'R':
			cin >> x;
			tree->remove(x);
			break;
		case '#':
			getline(cin, to_ignore);
			break;
		case 'C':
			cin >> x;
			tree->calculate_cache(x);
			break;
		case 'H':
			cin >> x;
			tree->calculate_alt_cache(x);
		}
	}
	if (tree)
		delete tree;

	return 0;
}
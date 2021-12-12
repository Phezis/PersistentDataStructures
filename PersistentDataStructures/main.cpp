#include "PersistentVector.h"
#include "PersistentList.h"

#include <iostream>
#include <list>

using namespace pds;
using namespace std;

struct S {
	S() = default;
	S(size_t a) : a(a), b(0) {}

	operator size_t() const { return a; }

	size_t a;
	size_t b;
};

int main() {
	list<int> l;
	vector<int> v(1, 1);
	cout << (*(v.crbegin()) == 1) << endl;
	cout << "A" << endl;
	return 0;
}

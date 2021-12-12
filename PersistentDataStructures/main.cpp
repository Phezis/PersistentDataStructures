#include "PersistentVector.h"
#include "PersistentList.h"

#include <iostream>

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
	
	vector<int> v(0, 0);
	std::cout << "A" << std::endl;
	return 0;
}

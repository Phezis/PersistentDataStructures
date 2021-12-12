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
	constexpr size_t size = (1 << 12); // + (1 << 8) + (1 << 6) + (1 << 2);
	PersistentVector<S> pvector;
	for (size_t k = 0; k < 1; ++k) {
		auto pv = pvector.push_back(0);
		for (size_t i = 1; i < size; ++i) {
			cout << "Iteration " << i << endl;
			pv = pv.push_back(i);
			for (size_t j = 0; j <= i; ++j) {
				if (pv[j] != j) {
					cout << pv[j] << " != " << j << endl;
				}
			}
		}
		for (size_t i = 0; i < size; ++i) {
			if (pv[i] != i) {
				cout << pv[i] << " != " << i << endl;
			}
		}
	}
	std::cout << "A" << std::endl;
	return 0;
}

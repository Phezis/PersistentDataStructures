#include "PersistentVector.h"
#include "PersistentList.h"

#include <iostream>

using namespace pds;
using namespace std;

int main() {
	PersistentVector<int> pvector;
	cout << pvector.size() << endl;
	return 0;
}

#include "ExtendedTest.h"
#include "ShortTest.h"
#include "FixedCapBiMap.h"

#include <iostream>
using namespace std;

int main() {
	testAll();
	testAllExtended();

	cout << "That's all!" << endl;
	return 0;
}

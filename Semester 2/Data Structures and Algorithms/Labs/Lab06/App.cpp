#include "include/ExtendedTest.h"
#include "include/ShortTest.h"
#include "include/SortedBag.h"
#include "include/SortedBagIterator.h"
#include <iostream>

using namespace std;

int main() {
  testAll();
  testAllExtended();

  cout << "Test over" << endl;
}

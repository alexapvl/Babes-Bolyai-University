#include "Bag.h"
#include "ExtendedTest.h"
#include "ShortTest.h"
#include <iostream>

using namespace std;

int main() {
  testAll();
  cout << "Short tests over" << endl;
  testAllExtended();
  cout << "All test over" << endl;
  return 0;
}
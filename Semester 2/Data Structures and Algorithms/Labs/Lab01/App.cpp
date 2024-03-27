#include <iostream>

#include "ExtendedTest.h"
#include "FixedCapBiMap.h"
#include "ShortTest.h"
using namespace std;

int main() {
  testAll();
  testAllExtended();

  cout << "That's all!" << endl;
  return 0;
}

#include <iostream>

#include "ExtendedTest.h"
#include "Matrix.h"
#include "ShortTest.h"

int main() {
  testAll();
  testAllExtended();
  testExtra();

  std::cout << "Test End" << std::endl;
}
#include <iostream>

#include "ExtendedTest.h"
#include "ShortTest.h"

int main() {
  testAll();
  testAllExtended();

  std::cout << "Finished SMM Tests!" << std::endl;
}

// do not use the iterator in the implementation
// the iterator is only for the ones who will use your implementation
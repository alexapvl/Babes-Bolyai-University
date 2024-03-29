#include <assert.h>

#include "Matrix.h"

void testAll() {
  Matrix m(4, 4);
  assert(m.nrLines() == 4);
  assert(m.nrColumns() == 4);
  m.modify(1, 1, 5);
  assert(m.element(1, 1) == 5);
  TElem old = m.modify(1, 1, 6);
  assert(m.element(1, 2) == NULL_TELEM);
  assert(old == 5);
}

void testExtra() {
  std::cout << "Test extra\n";
  Matrix m1(4, 5);
  try {
    m1.setMainDiagonal(5);
    assert(false);
  } catch (std::exception) {
    assert(true);
  }
  Matrix m2(4, 4);
  m2.setMainDiagonal(10);
  assert(m2.element(0, 0) == 10);
  assert(m2.element(1, 1) == 10);
  assert(m2.element(2, 2) == 10);
  assert(m2.element(3, 3) == 10);
}
#include "ShortTest.h"
#include "Bag.h"
#include "BagIterator.h"
#include <assert.h>

void testAll() {
  Bag b;
  b.add(5);
  b.add(1);
  b.add(10);
  b.add(10);
  b.remove(5);
  b.remove(10);
  b.remove(10);
  b.remove(1);
  assert(b.isEmpty() == true);
  assert(b.size() == 0);
  b.add(5);
  b.add(1);
  b.add(10);
  b.add(7);
  b.add(1);
  b.add(11);
  b.add(-3);
  assert(b.size() == 7);
  assert(b.search(10) == true);
  assert(b.search(16) == false);
  assert(b.nrOccurrences(1) == 2);
  assert(b.nrOccurrences(7) == 1);
  assert(b.remove(1) == true);
  assert(b.remove(6) == false);
  assert(b.size() == 6);
  assert(b.nrOccurrences(1) == 1);
  BagIterator it = b.iterator();
  it.first();
  while (it.valid()) {
    TElem e = it.getCurrent();
    it.next();
  }

  // test new function implementation for lab assignment
  Bag test;
  test.add(5);
  test.add(1);
  test.add(10);
  test.add(10);
  test.add(5);
  test.add(1);
  test.add(10);
  test.add(10);
  test.add(5);
  test.add(1);
  test.add(10);
  test.add(10);
  test.add(5);
  test.add(1);
  test.add(10);
  test.add(10);
  assert(test.removeOccurences(5, 5) == 4);
  assert(test.getIndex(5) == -1);
  assert(test.size() == 12);
}

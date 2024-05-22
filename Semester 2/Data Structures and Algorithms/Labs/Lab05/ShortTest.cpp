#include "ShortTest.h"
#include "Set.h"
#include "SetIterator.h"
#include <assert.h>

void testAll() {
  Set s;
  assert(s.isEmpty() == true);
  assert(s.size() == 0);
  assert(s.add(5) == true);
  assert(s.add(1) == true);
  assert(s.add(10) == true);
  assert(s.add(7) == true);
  assert(s.add(1) == false);
  assert(s.add(10) == false);
  assert(s.add(-3) == true);
  assert(s.size() == 5);
  assert(s.search(10) == true);
  assert(s.search(16) == false);
  assert(s.remove(1) == true);
  assert(s.remove(6) == false);
  assert(s.size() == 4);

  SetIterator it = s.iterator();
  it.first();
  int sum = 0;
  while (it.valid()) {
    TElem e = it.getCurrent();
    sum += e;
    it.next();
  }
  assert(sum == 19);

  testFilter();
}

void testFilter() {
  Set s;
  s.add(1);
  s.add(2);
  s.add(3);
  s.add(4);
  s.add(5);
  s.add(6);
  s.filter([](TElem e) { return e % 2 == 0; });
  assert(s.size() == 3);
}

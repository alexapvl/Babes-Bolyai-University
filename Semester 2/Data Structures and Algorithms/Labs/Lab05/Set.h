#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
// DO NOT INCLUDE SETITERATOR

// DO NOT CHANGE THIS PART
#define NULL_TELEM -111111
#define REMOVED_TELEM -111112
typedef int TElem;
typedef bool (*Condition)(TElem);
class SetIterator;

class Set {
  // DO NOT CHANGE THIS PART
  friend class SetIterator;

private:
  const int INITIAL_CAPACITY = 17;
  const double LOAD_FACTOR_THRESHOLD = 0.6;

  TElem* elements;
  int capacity;
  int length;

  int hash(TElem elem, int cap) const;
  int hash2(TElem elem, int cap) const;
  void resize();

public:
  // implicit constructor
  Set();

  // adds an element to the set
  // returns true if the element was added, false otherwise (if the element was
  // already in the set and it was not added)
  bool add(TElem e);

  // removes an element from the set
  // returns true if e was removed, false otherwise
  bool remove(TElem e);

  // checks whether an element belongs to the set or not
  bool search(TElem elem) const;

  // returns the number of elements;
  int size() const;

  // check whether the set is empty or not;
  bool isEmpty() const;

  void filter(Condition cond);

  // return an iterator for the set
  SetIterator iterator() const;

  int findNextPrime(int n) {
    if (n <= 1)
      return 2;
    if (n == 2)
      return n;
    if (n % 2 == 0)
      n++;
    while (true) {
      bool isPrime = true;
      int sqrt_n = std::sqrt(n);
      for (int i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) {
          isPrime = false;
          break;
        }
      }
      if (isPrime) {
        return n;
      }
      n += 2;
    }
  }

  // destructor
  ~Set();
};
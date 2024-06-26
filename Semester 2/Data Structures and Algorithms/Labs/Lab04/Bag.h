#pragma once
// DO NOT INCLUDE BAGITERATOR
//  ADT Bag - using a DLLA with (element, frequency) pairs.
#include <exception>
#include <iostream>

// DO NOT CHANGE THIS PART
#define NULL_TELEM -111111;
typedef int TElem;
typedef int TFrequency;
class BagIterator;

typedef std::pair<TElem, TFrequency> BagElement;
typedef struct {
  BagElement info;
  int next;
  int prev;
} DLAANode;

class Bag {

private:
  // TODO - Representation
  DLAANode* nodes;
  int capacity;
  int head;
  int tail;
  int firstEmpty;
  int length;

  void resize();
  int allocate();
  void deallocate(int position);

  // DO NOT CHANGE THIS PART
  friend class BagIterator;

public:
  int getIndex(TElem elem);
  // constructor
  Bag();

  // adds an element to the bag
  void add(TElem elem);

  void addFirst(TElem elem);

  // removes one occurence of an element from a bag
  // returns true if an element was removed, false otherwise (if e was not part of the bag)
  bool remove(TElem elem);

  // checks if an element appears is the bag
  bool search(TElem elem) const;

  // returns the number of occurrences for an element in the bag
  // will return the frequency of the element e in the bag
  int nrOccurrences(TElem elem) const;

  // returns the number of elements from the bag
  int size() const;

  // returns an iterator for this bag
  BagIterator iterator() const;

  // checks if the bag is empty
  bool isEmpty() const;

  // destructor
  ~Bag();

  // returns a string representation of the bag
  std::string to_string() const;

  int removeOccurences(int nr, TElem elem);
};
#pragma once
// DO NOT INCLUDE SMMITERATOR

// DO NOT CHANGE THIS PART
#include <utility>
#include <vector>
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TELEM pair<TKey, TValue>(-111111, -111111);
using namespace std;
class SMMIterator;
typedef bool (*Relation)(TKey, TKey);

typedef struct Node {
  TElem info;
  Node* prev;
  Node* next;
} Node;

class SortedMultiMap {
  friend class SMMIterator;

 private:
  Node* head;    // pointer to the first node
  Node* tail;    // pointer to the last node
  Relation rel;  // relation used to sort the SMM

 public:
  // constructor
  SortedMultiMap(Relation r);

  // adds a new key value pair to the sorted multi map
  void add(TKey c, TValue v);

  // adds a new key value pair to the beginning of the sorted multi map
  void addFirst(TKey c, TValue v);

  // adds a new key value pair to the end of the sorted multi map
  void addLast(TKey c, TValue v);

  // returns the values belonging to a given key
  vector<TValue> search(TKey c) const;

  // removes a key value pair from the sorted multimap
  // returns true if the pair was removed (it was part of the multimap), false
  // if nothing is removed
  bool remove(TKey c, TValue v);

  // returns the number of key-value pairs from the sorted multimap
  int size() const;

  // verifies if the sorted multi map is empty
  bool isEmpty() const;

  // returns an iterator for the sorted multimap. The iterator will returns the
  // pairs as required by the relation (given to the constructor)
  SMMIterator iterator() const;

  // destructor
  ~SortedMultiMap();

  // returns a string representation with [key, value] pairs(one on each line)
  // string toString() const;
};

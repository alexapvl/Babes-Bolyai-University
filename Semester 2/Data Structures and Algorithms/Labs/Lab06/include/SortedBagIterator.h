#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator {
  friend class SortedBag;

private:
  const SortedBag& bag;
  SortedBagIterator(const SortedBag& b);

  TNode* elements;
  int currentIndex;
  int currentFrequency;
  int numberOfElements;
  void DFS(TNode* node);

public:
  TComp getCurrent();
  bool valid();
  void next();
  void first();
};

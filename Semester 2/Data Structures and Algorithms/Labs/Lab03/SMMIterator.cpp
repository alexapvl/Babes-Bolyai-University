#include "SMMIterator.h"

#include "SortedMultiMap.h"

SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d) { first(); }

void SMMIterator::first() { this->currentNode = map.head; }

void SMMIterator::next() {
  if (!valid()) {
    throw std::exception();
  }
  this->currentNode = this->currentNode->next;
}

bool SMMIterator::valid() const {
  if (this->currentNode == nullptr) {
    return false;
  }
  return true;
}

TElem SMMIterator::getCurrent() const {
  if (!valid()) {
    throw std::exception();
  }
  return this->currentNode->info;
}

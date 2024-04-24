#include "BagIterator.h"
#include "Bag.h"
#include <exception>

using namespace std;

BagIterator::BagIterator(const Bag& c) : bag(c) {
  this->currentElement = this->bag.head;
}

void BagIterator::first() {
  this->currentElement = this->bag.head;
}

void BagIterator::next() {
  if (this->currentElement == -1) {
    throw exception();
  }
  this->currentElement = this->bag.nodes[this->currentElement].next;
}

bool BagIterator::valid() const {
  if (this->currentElement == -1)
    return false;
  else
    return true;
}

TElem BagIterator::getCurrent() const {
  if (this->currentElement == -1) {
    throw exception();
  }
  return this->currentElement;
}

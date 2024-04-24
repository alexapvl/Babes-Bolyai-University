#include "BagIterator.h"
#include "Bag.h"
#include <exception>

using namespace std;

BagIterator::BagIterator(const Bag& c) : bag(c) {
  this->first();
}

void BagIterator::first() {
  this->currentFrequency = 1;
  this->currentIndex = this->bag.head;
}

void BagIterator::next() {
  if (this->currentIndex == -1)
    throw exception();
  if (this->currentFrequency < this->bag.nodes[currentIndex].info.second)
    this->currentFrequency++;
  else {
    this->currentIndex = this->bag.nodes[this->currentIndex].next;
    this->currentFrequency = 1;
  }
}

bool BagIterator::valid() const {
  return this->currentIndex != -1;
}

TElem BagIterator::getCurrent() const {
  if (this->currentIndex == -1) {
    throw exception();
  }
  return this->bag.nodes[this->currentIndex].info.first;
}

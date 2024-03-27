#include "FixedCapBiMapIterator.h"

#include <exception>

#include "FixedCapBiMap.h"

FixedCapBiMapIterator::FixedCapBiMapIterator(const FixedCapBiMap& d) : map(d) {
  this->currentPosition = 0;
}

void FixedCapBiMapIterator::first() { this->currentPosition = 0; }

void FixedCapBiMapIterator::next() {
  if (this->currentPosition == this->map.mapSize) throw std::exception();
  this->currentPosition++;
}

TElem FixedCapBiMapIterator::getCurrent() {
  if (this->currentPosition == this->map.mapSize) throw std::exception();
  return this->map.elements[currentPosition];
}

bool FixedCapBiMapIterator::valid() const {
  return this->currentPosition < this->map.mapSize;
}

#include "SortedBag.h"
#include "SortedBagIterator.h"

SortedBag::SortedBag(Relation r) {
	//TODO - Implementation
}

void SortedBag::add(TComp e) {
	//TODO - Implementation
}


bool SortedBag::remove(TComp e) {
	//TODO - Implementation
	return false;
}


bool SortedBag::search(TComp elem) const {
	//TODO - Implementation
	return false;
}


int SortedBag::nrOccurrences(TComp elem) const {
	//TODO - Implementation
	return 0;
}



int SortedBag::size() const {
	//TODO - Implementation
	return 0;
}


bool SortedBag::isEmpty() const {
	//TODO - Implementation
	return false;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
	//TODO - Implementation
}

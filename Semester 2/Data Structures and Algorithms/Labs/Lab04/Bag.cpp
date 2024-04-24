#include "Bag.h"
#include "BagIterator.h"
using namespace std;

Bag::Bag() {
  this->capacity = 5;
  this->nodes = new DLAANode[this->capacity];
  for (int i = 0; i < 5 - 1; i++) {
    this->nodes[i].next = i + 1;
  }
  this->nodes[4].next = -1;
  this->head = -1;
  this->tail = -1;
  this->firstEmpty = 0;
  this->length = 0;
}

void Bag::add(TElem elem) {
  int index = this->getIndex(elem);
  if (index != -1) {
    this->nodes[index].info.second++;
  } else if (this->head == -1) { // if the bag is empty
    this->addFirst(elem);
  } else { // if the bag is empty, add the first element to it
    int newElemIndex = this->allocate();
    if (newElemIndex == -1) {
      this->resize();
      newElemIndex = this->allocate();
    }
    this->nodes[tail].next = newElemIndex;
    this->nodes[newElemIndex].prev = this->tail;
    this->tail = newElemIndex;
    this->nodes[this->tail].info.first = elem;
    this->nodes[this->tail].info.second = 1;
  }
  this->length++;
}

void Bag::addFirst(TElem elem) {
  // adds the element if the bag is empty, i.e. the head = -1 and tail = -1
  this->head = this->firstEmpty;
  this->tail = this->firstEmpty;
  this->firstEmpty = this->nodes[this->firstEmpty].next;
  this->nodes[this->head].prev = -1;
  this->nodes[this->tail].next = -1;
  this->nodes[head].info.first = elem;
  this->nodes[head].info.second = 1;
}

bool Bag::remove(TElem elem) {
  int indexOfElem = this->getIndex(elem);
  if (indexOfElem == -1) { // if the element is not in the bag
    return false;
  }
  this->nodes[indexOfElem].info.second--;          // decrement the frequency of the existing node
  if (this->nodes[indexOfElem].info.second == 0) { // if the frequency of the existing node is 0 we need to delete the element from the bag
    this->deallocate(indexOfElem);                 // deallocate the node if the frequency is zero
  }
  this->length--; // update the length of the bag
  return true;
}

bool Bag::search(TElem elem) const {
  int currentIndex = this->head;
  while (currentIndex != -1) {
    if (this->nodes[currentIndex].info.first == elem) {
      return true;
    }
    currentIndex = this->nodes[currentIndex].next;
  }
  return false;
}

int Bag::nrOccurrences(TElem elem) const {
  int currentIndex = this->head;
  while (currentIndex != -1) {
    if (this->nodes[currentIndex].info.first == elem) {
      return this->nodes[currentIndex].info.second;
    }
    currentIndex = this->nodes[currentIndex].next;
  }
  return 0;
}

int Bag::size() const {
  return this->length;
}

bool Bag::isEmpty() const {
  return this->length == 0;
}

BagIterator Bag::iterator() const {
  return BagIterator(*this);
}

Bag::~Bag() {
  delete[] this->nodes;
}

std::string Bag::to_string() const {
  std::string result = "";
  int current = this->head;
  while (current != -1) {
    result += "prev: " + std::to_string(this->nodes[current].prev) + " " + "next: " + std::to_string(this->nodes[current].next) + " info: " + std::to_string(this->nodes[current].info.first) + "(" + std::to_string(this->nodes[current].info.second) + ") \n";
    current = this->nodes[current].next;
  }
  return "head: " + std::to_string(this->head) + "\n" + result + "tail: " + std::to_string(this->tail) + "\n############################\n";
}
// Private methods

void Bag::resize() {
  DLAANode* newNodes = new DLAANode[this->capacity * 2]; // allocate a new array of nodes
  for (int i = 0; i < this->capacity; i++) {             // copy the old nodes into the new array
    newNodes[i] = this->nodes[i];
  }
  delete[] this->nodes;                                           // deallocate the old array
  this->nodes = newNodes;                                         // update the nodes pointer
  this->firstEmpty = this->capacity;                              // update the firstEmpty index
  for (int i = this->capacity; i < this->capacity * 2 - 1; i++) { // initialize the next of the new empty nodes
    this->nodes[i].next = i + 1;
  }
  this->nodes[this->capacity * 2 - 1].next = -1; // initialize the next of the last new empty node
  this->capacity *= 2;                           // update the capacity
}

int Bag::allocate() {
  int newElementIndex = this->firstEmpty;
  if (newElementIndex != -1) {                             // if there is an empty node available
    this->firstEmpty = this->nodes[this->firstEmpty].next; // update the firstEmpty index
    if (this->firstEmpty != -1) {                          // if there are more empty nodes
      this->nodes[this->firstEmpty].prev = -1;             // update the prev of the new firstEmpty
    }
    this->nodes[newElementIndex].next = -1; // initialize the next of the new node
    this->nodes[newElementIndex].prev = -1; // initialize the prev of the new node
  }
  return newElementIndex;
}

void Bag::deallocate(int position) {
  if (position == this->head && position != this->tail) { // if the element is the first in the bag
    this->head = this->nodes[position].next;
    this->nodes[this->head].prev = -1;
  } else if (position == this->tail && position != this->head) { // if the element is the last in the bag
    this->tail = this->nodes[position].prev;
    this->nodes[this->tail].next = -1;
  } else if (position == this->head && position == this->tail) {
    this->head = -1;
    this->tail = -1;
  } else { // if element is in between other elements in the bag
    this->nodes[this->nodes[position].prev].next = this->nodes[position].next;
    this->nodes[this->nodes[position].next].prev = this->nodes[position].prev;
  }
  this->nodes[position].next = this->firstEmpty;
  this->nodes[position].prev = -1;
  if (this->firstEmpty != -1) {                    // if there were other empty nodes before
    this->nodes[this->firstEmpty].prev = position; // update the prev of the new firstEmpty
  }
  this->firstEmpty = position; // update the firstEmpty index
}

void Bag::insert_position(TElem elem, int position) {
  int index = this->getIndex(elem); // search for the element in the bag
  if (index != -1) {                // if the element is already in the bag
    throw exception();
  }
  if (position < 0 || position >= this->capacity) { // if the position is invalid
    throw exception();
  }
  index = this->allocate();
  if (index == -1) {
    this->resize();
    index = this->allocate();
  }
  this->nodes[index].info.first = elem;
  this->nodes[index].info.second = 1;
  if (position == 0) {
    if (this->head == -1) { // if bag is empty
      this->head = index;
      this->tail = index;
    } else {
      this->nodes[this->head].prev = index;
      this->nodes[index].next = this->head;
      this->nodes[index].prev = -1;
      this->head = index;
    }
  } else {
    int currentNode = this->head;
    int currentPosition = 0;
    while (currentNode != -1 && currentPosition < position - 1) {
      currentNode = this->nodes[currentNode].next;
      currentPosition++;
    }
    if (currentNode == -1) { // should never be -1 if position is valid from the beginning
      int nextNode = this->nodes[currentNode].next;
      this->nodes[index].next = nextNode;
      this->nodes[index].prev = currentNode;
      this->nodes[currentNode].next = index;
      if (nextNode == -1) {
        this->tail = index;
      } else {
        this->nodes[nextNode].prev = index;
      }
    }
  }
  this->length++;
}

int Bag::getIndex(TElem elem) {
  int currentIndex = this->head;
  while (currentIndex != -1) {
    if (this->nodes[currentIndex].info.first == elem)
      break;
    currentIndex = this->nodes[currentIndex].next;
  }
  return currentIndex;
}

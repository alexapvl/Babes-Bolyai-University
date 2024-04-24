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
  } else if (this->head == -1 && this->tail == -1) // if the bag is empty
    this->addFirst(elem);                          // if the bag is empty, add the first element to it
  else {
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
  if (this->nodes[indexOfElem].info.second == 0) { // if the frequency of the existing node is 0
    if (indexOfElem == this->head) {               // if the node to be removed is the head
      this->head = this->nodes[indexOfElem].next;
      if (this->head != -1) { // if there are other nodes in the bag
        this->nodes[this->head].prev = -1;
      }
    } else if (indexOfElem == this->tail) { // if the node to be removed is the tail
      this->tail = this->nodes[indexOfElem].prev;
      this->nodes[this->tail].next = -1;
    } else { // if the node to be removed is in the middle of the bag
      this->nodes[this->nodes[indexOfElem].prev].next = this->nodes[indexOfElem].next;
      this->nodes[this->nodes[indexOfElem].next].prev = this->nodes[indexOfElem].prev;
    }
    this->deallocate(indexOfElem); // deallocate the node
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
  return result + "############################\n";
}

// Private methods

void Bag::resize() {
  cout << this->capacity << "resizing"
       << this->capacity * 2 << endl;
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
  this->nodes[position].next = this->firstEmpty;   // update the next of the node to be deallocated
  this->nodes[position].prev = -1;                 // update the prev of the node to be deallocated
  if (this->firstEmpty != -1) {                    // if there are other empty nodes
    this->nodes[this->firstEmpty].prev = position; // update the prev of the new firstEmpty
  }
  this->firstEmpty = position; // update the firstEmpty index
}

void Bag::insert_position(TElem elem, int position) {
  int index = this->getIndex(elem);   // search for the element in the bag
  if (index != -1) {                  // if the element is already in the bag
    this->nodes[index].info.second++; // increment the frequency of the existing node
    this->length++;
    return;
  }
  if (position < 0 || position >= this->capacity) { // if the position is invalid
    throw std::exception();                         // throw an exception
  }
  int newElemIndex = this->allocate(); // get the position at which the new node will be added
  if (newElemIndex == -1) {            // if there are no empty nodes available
    this->resize();                    // resize the array of nodes
    newElemIndex = this->allocate();   // get the position at which the new node will be added
  }
  this->nodes[newElemIndex].info.first = elem; // add the element to the new node
  this->nodes[newElemIndex].info.second = 1;   // initialize the frequency of the new node
  if (position == 0) {                         // update the head if the new node is added at the beginning
    if (this->head == -1) {                    // if the bag is empty
      this->head = newElemIndex;
      this->tail = newElemIndex;
    } else { // if the bag is not empty
      this->nodes[newElemIndex].next = this->head;
      this->nodes[this->head].prev = newElemIndex;
      this->head = newElemIndex;
    }
  } else {                                                        // update the nodes if the new node is added in the middle or at the end
    int currentNode = this->head;                                 // start from the head
    int currentPosition = 0;                                      // start from the first position
    while (currentNode != -1 || currentPosition < position - 1) { // move to the position
      currentNode = this->nodes[currentNode].next;
      currentPosition++;
    }
    if (currentNode != -1) {                        // this should always be true if the position is valid
      int nextNode = this->nodes[currentNode].next; // get the next node
      this->nodes[newElemIndex].next = nextNode;    // update the next of the new node
      this->nodes[newElemIndex].prev = currentNode; // update the prev of the new node
      this->nodes[currentNode].next = newElemIndex; // update the next of the current node
      if (nextNode == -1) {                         // update the tail if the new node is added at the end
        this->tail = newElemIndex;
      } else { // update the prev of the next node if the new node is added in the middle
        this->nodes[nextNode].prev = newElemIndex;
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

#include "Bag.h"
#include "BagIterator.h"
using namespace std;

Bag::Bag() {
  this->capacity = 5;
  this->nodes = new DLAANode[this->capacity];
  this->head = -1;
  this->tail = -1;
  this->firstEmpty = 0;
  this->length = 0;
}

void Bag::add(TElem elem) {
  int indexOfElem = this->search_index(elem);  // search for the element in the bag
  if (indexOfElem != -1) {                     // if the element is already in the bag
    this->nodes[indexOfElem].info.second++;    // increment the frequency of the existing node
  } else {                                     // if the element is not in the bag
    if (this->head == -1) {                    // if the bag is empty
      this->head = this->allocate();           // allocate memory for the new element
      this->tail = this->head;                 // update the tail
      this->nodes[head].info.first = elem;     // add the element to the new node
      this->nodes[head].info.second = 1;       // initialize the frequency of the new node
      this->nodes[head].next = -1;             // initialize the next of the new node
      this->nodes[head].prev = -1;             // initialize the prev of the new node
    } else {                                   // if the bag is not empty
      int newIndex = this->allocate();         // get the position at which the new node will be added
      this->nodes[newIndex].info.first = elem; // add the element to the new node
      this->nodes[newIndex].info.second = 1;   // initialize the frequency of the new node
      // update links
      this->nodes[tail].next = newIndex;
      this->nodes[newIndex].prev = tail;
      this->tail = newIndex;
    }
  }
  this->length++; // update the length of the bag
}

bool Bag::remove(TElem elem) {
  int indexOfElem = search_index(elem); // search for the element in the bag
  if (indexOfElem == -1) {              // if the element is not in the bag
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
  int current = this->head;
  while (current != -1) {
    if (this->nodes[current].info.first == elem) {
      return true;
    }
    current = this->nodes[current].next;
  }
  return false;
}

int Bag::nrOccurrences(TElem elem) const {
  int current = this->head;
  while (current != -1) {
    if (this->nodes[current].info.first == elem) {
      return this->nodes[current].info.second;
    }
    current = this->nodes[current].next;
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
  return result;
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
  this->nodes[position].next = this->firstEmpty;   // update the next of the node to be deallocated
  this->nodes[position].prev = -1;                 // update the prev of the node to be deallocated
  if (this->firstEmpty != -1) {                    // if there are other empty nodes
    this->nodes[this->firstEmpty].prev = position; // update the prev of the new firstEmpty
  }
  this->firstEmpty = position; // update the firstEmpty index
}

int Bag::search_index(TElem elem) const { // search for the element in the bag and return its index
  int current = this->head;               // start from the head
  while (current != -1) {                 // move to the next node until the end of the bag or until the element is found
    if (this->nodes[current].info.first == elem) {
      return current; // return the index of the node if the element is found
    }
    current = this->nodes[current].next; // move to the next node
  }
  std::cout << "OFFFFFFFFFF\n";
  return -1; // return -1 if the element is not found
}

void Bag::insert_position(TElem elem, int position) {
  int elemIndex = this->search_index(elem); // search for the element in the bag
  if (elemIndex != -1) {                    // if the element is already in the bag
    this->nodes[elemIndex].info.second++;   // increment the frequency of the existing node
    return;
  }
  if (position < 0 || position >= this->capacity) { // if the position is invalid
    throw std::runtime_error("Invalid position");   // throw an exception
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

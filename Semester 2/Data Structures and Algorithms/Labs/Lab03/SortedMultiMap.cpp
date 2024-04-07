#include "SortedMultiMap.h"

#include <exception>
#include <iostream>
#include <vector>

#include "SMMIterator.h"
using namespace std;

/*
BC: θ(1)
WC: θ(1)
AC: θ(1)
*/
SortedMultiMap::SortedMultiMap(Relation r) {
  this->head = nullptr;
  this->tail = nullptr;
  this->rel = r;
}

/*
BC: θ(1) - when the list is empty or the new node should be the first in
the list
WC: θ(size) - when the new node should be added at the end of the
list AC: O(size)
*/
void SortedMultiMap::add(TKey c, TValue v) {
  // if the list is empty, add the new node at the beginning
  // or if the new node should be the first in the list based on the relation
  if (this->isEmpty() || !this->rel(this->head->info.first, c)) {
    this->addFirst(c, v);
    return;
  }

  // using the iterator, we find the position where the new node should be
  // based on the relation of the smm
  SMMIterator smmit = this->iterator();
  while (smmit.valid() && this->rel(smmit.getCurrent().first, c)) {
    smmit.next();
  }

  // if the iterator is at the end, add the new node at the end
  if (!smmit.valid()) {
    this->addLast(c, v);
    return;
  }

  // now the iterator points to the element before which
  // we should insert the new node
  Node* newNode = new Node;     // allocate the memory for the new node
  newNode->info = TElem(c, v);  // set the info of the new node

  // update the links of the new node
  newNode->prev = smmit.currentNode->prev;
  newNode->next = smmit.currentNode;

  // update the links for the nodes to the left and the right
  smmit.currentNode->prev->next = newNode;
  smmit.currentNode->prev = newNode;
}

/*
BC: θ(1)
WC: θ(1)
AC: θ(1)
*/
void SortedMultiMap::addFirst(TKey c, TValue v) {
  Node* newNode = new Node;     // allocate the memory for the new node
  newNode->info = TElem(c, v);  // set the info of the new node

  // set the next of the new node to the current head
  newNode->next = this->head;

  // set the prev of the new node to nullptr because we insert at the beginning
  newNode->prev = nullptr;

  // if the list is empty, the new node is both the head and the tail
  if (this->head == nullptr) {
    this->head = newNode;
    this->tail = newNode;
  } else {
    // set the prev of the current head to the new node
    this->head->prev = newNode;

    // set the head to the new node
    this->head = newNode;
  }
}

/*
BC: θ(1)
WC: θ(1)
AC: θ(1)
*/
void SortedMultiMap::addLast(TKey c, TValue v) {
  Node* newNode = new Node;     // allocate the memory for the new node
  newNode->info = TElem(c, v);  // set the info of the new node

  // set the next of the new node to nullptr because we insert at the end
  newNode->next = nullptr;

  // set the prev of the new node to the current tail
  newNode->prev = this->tail;

  // if the list is empty, the new node is both the head and the tail
  if (this->head == nullptr) {
    this->head = newNode;
    this->tail = newNode;
  } else {
    // set the next of the current tail to the new node
    this->tail->next = newNode;

    // set the tail to the new node
    this->tail = newNode;
  }
}

/*
BC: θ(number of elements with key 'c') - when the key is the first one in
the SMM
WC: θ(size) - when the key is the last one in the SMM
AC: O(size)
*/
vector<TValue> SortedMultiMap::search(TKey c) const {
  vector<TValue> values;
  SMMIterator smmit = this->iterator();
  while (smmit.valid()) {
    if (smmit.getCurrent().first == c) {
      values.push_back(smmit.getCurrent().second);
    }
    smmit.next();
  }
  return values;
}

/*
BC: θ(1) - when the node to be removed is the head of the SMM
WC: θ(size) - when the node to be removed is the tail of the SMM
(practically it is θ(2*size) since we have to iterate through the SMM to
find the key(it is the last one))
AC: O(size * number of elements with key 'c')
*/
bool SortedMultiMap::remove(TKey c, TValue v) {
  SMMIterator smmit = this->iterator();
  while (smmit.valid()) {
    if (smmit.getCurrent().first == c && smmit.getCurrent().second == v) {
      // if the node to be removed is the head
      if (smmit.currentNode == this->head) {
        this->head = this->head->next;
        if (this->head != nullptr) {
          this->head->prev = nullptr;
        }
        delete smmit.currentNode;
        return true;
      }

      // if the node to be removed is the tail
      if (smmit.currentNode == this->tail) {
        this->tail = this->tail->prev;
        if (this->tail != nullptr) {
          this->tail->next = nullptr;
        }
        delete smmit.currentNode;
        return true;
      }

      // if the node to be removed is in the middle
      smmit.currentNode->prev->next = smmit.currentNode->next;
      smmit.currentNode->next->prev = smmit.currentNode->prev;
      delete smmit.currentNode;
      return true;
    }
    smmit.next();
  }
  return false;
}

/*
BC: θ(size)
WC: θ(size)
AC: θ(size)
*/
int SortedMultiMap::size() const {
  int size = 0;
  SMMIterator smmit = this->iterator();
  while (smmit.valid()) {
    size++;
    smmit.next();
  }
  return size;
}

/*
BC: θ(1)
WC: θ(1)
AC: θ(1)
*/
bool SortedMultiMap::isEmpty() const { return this->head == nullptr; }

/*
BC: θ(1)
WC: θ(1)
AC: θ(1)
*/
SMMIterator SortedMultiMap::iterator() const { return SMMIterator(*this); }

/*
BC: θ(size)
WC: θ(size)
AC: θ(size)
*/
SortedMultiMap::~SortedMultiMap() {
  Node* currentNode = this->head;
  while (currentNode != nullptr) {
    Node* nextNode = currentNode->next;
    delete currentNode;
    currentNode = nextNode;
  }
}

// string SortedMultiMap::toString() const {
//   string smmString = "";
//   SMMIterator smmit = this->iterator();
//   while (smmit.valid()) {
//     smmString += "[" + to_string(smmit.getCurrent().first) + ", " +
//                  to_string(smmit.getCurrent().second) + "]\n";
//     smmit.next();
//   }
//   return smmString;
// }

#include "../include/SortedBag.h"
#include "../include/SortedBagIterator.h"

SortedBag::SortedBag(Relation r) : r(r) {
  root = NULL;
  sizeOfBag = 0;
}

void SortedBag::add(TComp e) {
  /*
    based on the relation, if the relation is true, we go left, otherwise we go right
  */

  TNode* newNode = new TNode();
  newNode->data.first = e;
  newNode->data.second = 1;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->parent = NULL;
  TNode* current = root;
  if (current == NULL) {
    root = newNode;
    sizeOfBag = 1;
    return;
  }
  while (current != NULL) {
    if (current->data.first == e) {
      current->data.second++; // element already exists in the tree and the frequency is updated
      delete newNode;
      break;
    }
    if (r(e, current->data.first)) { // go left
      if (current->left == NULL) {   // we insert
        current->left = newNode;
        newNode->parent = current;
        break;
      } else { // we go there and continue
        current = current->left;
      }
    } else {                        // go right and do the same thing
      if (current->right == NULL) { // we insert
        current->right = newNode;
        newNode->parent = current;
        break;
      } else { // we go there and continue
        current = current->right;
      }
    }
  }
  sizeOfBag++;
}

bool SortedBag::remove(TComp e) {
  // TODO - Implementation
  // will need to redo links if the removed node has children(cases 0/1/2)
  // cases 0 and 1 are easy while in case 2 we will need to figure it out
  return false;
}

bool SortedBag::search(TComp elem) const {
  TNode* current = root;
  while (current != NULL) {
    if (current->data.first == elem) {
      return true;
      break;
    } else if (r(elem, current->data.first))
      current = current->left;
    else
      current = current->right;
  }
  return false;
}

int SortedBag::nrOccurrences(TComp elem) const {
  TNode* current = root;
  while (current != NULL) {
    if (current->data.first == elem) {
      return current->data.second;
      break;
    } else if (r(elem, current->data.first))
      current = current->left;
    else
      current = current->right;
  }
  return 0;
}

int SortedBag::size() const {
  return sizeOfBag;
}

bool SortedBag::isEmpty() const {
  return root == NULL;
}

SortedBagIterator SortedBag::iterator() const {
  return SortedBagIterator(*this);
}

SortedBag::~SortedBag() {
  // TODO - Implementation
}

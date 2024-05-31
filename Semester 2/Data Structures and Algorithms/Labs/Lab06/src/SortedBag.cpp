#include "../include/SortedBag.h"
#include "../include/SortedBagIterator.h"
#include <iostream>
using namespace std;

// h is the height of the binary search tree

SortedBag::SortedBag(Relation r) : r(r) {
  root = NULL;
  sizeOfBag = 0;
  distinctElements = 0;
}
// BC: theta(1) | WC: theta(h) | AC: O(h)
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
    distinctElements = 1;
    return;
  }
  while (current != NULL) {
    if (current->data.first == e) {
      current->data.second++; // element already exists in the tree and the frequency is updated
      delete newNode;
      sizeOfBag++;
      return;
    }
    if (r(e, current->data.first)) { // go left
      if (current->left == NULL) {   // we insert
        current->left = newNode;
        newNode->parent = current;
        sizeOfBag++;
        distinctElements++;
        return;
      } else { // we go there and continue
        current = current->left;
      }
    } else {                        // go right and do the same thing
      if (current->right == NULL) { // we insert
        current->right = newNode;
        newNode->parent = current;
        sizeOfBag++;
        distinctElements++;
        return;
      } else { // we go there and continue
        current = current->right;
      }
    }
  }
}

// BC: theta(1) | WC: theta(h) | AC: O(h)
bool SortedBag::remove(TComp e) {
  // TODO - Implementation
  // will need to redo links if the removed node has children(cases 0/1/2)
  // cases 0 and 1 are easy while in case 2 we will need to figure it out
  TNode* current = root;
  while (current != NULL) {
    if (current->data.first == e) {
      if (current->data.second > 1) {
        current->data.second--;
        sizeOfBag--;
        return true;
      } else {
        if (numberOfDescendants(current) == 0) {
          if (current->parent == NULL) {
            root = NULL;
            delete current;
            sizeOfBag = 0;
            distinctElements = 0;
            return true;
          }
          if (current->parent->left == current) {
            current->parent->left = NULL;
            delete current;
            sizeOfBag--;
            distinctElements--;
            return true;
          } else {
            current->parent->right = NULL;
            delete current;
            sizeOfBag--;
            distinctElements--;
            return true;
          }
        } else if (numberOfDescendants(current) == 1) { // case in which we have 1 child
          if (current->left != NULL) {                  // we have a left child
            if (current->parent == NULL) {              // if we are at the root
              root = current->left;
              current->left->parent = NULL;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
            if (current->parent->left == current) { // if we are a left child
              current->parent->left = current->left;
              current->left->parent = current->parent;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            } else { // if we are a right child
              current->parent->right = current->left;
              current->left->parent = current->parent;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
          } else {                         // we have a right child
            if (current->parent == NULL) { // if we are at the root
              root = current->right;
              current->right->parent = NULL;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
            if (current->parent->left == current) { // if we are a left child
              current->parent->left = current->right;
              current->right->parent = current->parent;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            } else {
              current->parent->right = current->right;
              current->right->parent = current->parent;
              delete current;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
          }
        } else if (numberOfDescendants(current) == 2) { // case in which we have 2 children
          TNode* successor = current->right;
          while (successor->left != NULL) {
            successor = successor->left;
          }
          current->data = successor->data;
          if (successor->parent->left == successor) {
            if (successor->right != NULL) {
              successor->parent->left = successor->right;
              successor->right->parent = successor->parent;
              delete successor;
              sizeOfBag--;
              distinctElements--;
              return true;
            } else {
              successor->parent->left = NULL;
              delete successor;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
          } else {
            if (successor->right != NULL) {
              successor->parent->right = successor->right;
              successor->right->parent = successor->parent;
              delete successor;
              sizeOfBag--;
              distinctElements--;
              return true;
            } else {
              successor->parent->right = NULL;
              delete successor;
              sizeOfBag--;
              distinctElements--;
              return true;
            }
          }
        } else {
          return false;
        }
      }

    } else if (r(e, current->data.first))
      current = current->left;
    else
      current = current->right;
  }
  return false; // element not found to remove
}

bool SortedBag::search(TComp elem) const {
  TNode* current = root;
  while (current != NULL) {
    if (current->data.first == elem) {
      return true;
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
  // do it without using a stack or a queue
  // we will use a stack
}

TNode* SortedBag::getNode(TComp elem) {
  TNode* current = root;
  while (current != NULL) {
    if (current->data.first == elem) {
      return current;
    } else if (r(elem, current->data.first))
      current = current->left;
    else
      current = current->right;
  }
  return NULL;
}

int SortedBag::distinctCount() const {
  return distinctElements;
}

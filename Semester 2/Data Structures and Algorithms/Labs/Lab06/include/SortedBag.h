#pragma once
#include <utility>
/*
ADT SortedBag - using BST with linked representation with dynamic allocation.
In the BST(unique element, frequency) pairs are stored
*/
// DO NOT INCLUDE SORTEDBAGITERATOR

// DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef bool (*Relation)(TComp, TComp);
#define NULL_TCOMP -11111;

class SortedBagIterator;

typedef std::pair<TComp, int> TPair;
struct TNode; // forward declaration
struct TNode {
  TPair data;
  TNode* left;
  TNode* right;
  TNode* parent;
};

class SortedBag {
  friend class SortedBagIterator;

private:
  TNode* root;
  Relation r;
  int sizeOfBag;

  int numberOfDescendants(TNode* node) {
    if (node->left == NULL && node->right == NULL)
      return 0;
    if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL))
      return 1;
    if (node->left != NULL && node->right != NULL)
      return 2;
  }

public:
  // constructor
  SortedBag(Relation r);

  // adds an element to the sorted bag
  void add(TComp e);

  // removes one occurence of an element from a sorted bag
  // returns true if an element was removed, false otherwise (if e was not part of the sorted bag)
  bool remove(TComp e);

  // checks if an element appearch is the sorted bag
  bool search(TComp e) const;

  // returns the number of occurrences for an element in the sorted bag
  int nrOccurrences(TComp e) const;

  // returns the number of elements from the sorted bag
  int size() const;

  // returns an iterator for this sorted bag
  SortedBagIterator iterator() const;

  // checks if the sorted bag is empty
  bool isEmpty() const;

  // destructor
  ~SortedBag();
};
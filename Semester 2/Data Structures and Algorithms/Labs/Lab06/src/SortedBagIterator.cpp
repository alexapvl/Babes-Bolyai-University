#include "../include/SortedBagIterator.h"
#include "../include/SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
  elements = new TNode[b.sizeOfBag];
  first();
  numberOfElements = 0;
  DFS(b.root);
}

TComp SortedBagIterator::getCurrent() {
  if (!valid())
    throw exception();
  return elements[currentIndex].data.first;
}

bool SortedBagIterator::valid() {
  return currentIndex < numberOfElements;
}

void SortedBagIterator::next() {
  if (!valid())
    throw exception();
  if (currentFrequency < elements[currentIndex].data.second) {
    currentFrequency++;
  } else {
    currentIndex++;
    currentFrequency = 1;
  }
}

void SortedBagIterator::first() {
  currentIndex = 0;
  currentFrequency = 1;
}

void SortedBagIterator::DFS(TNode* node) {
  if (node == NULL)
    return;
  DFS(node->left);
  elements[numberOfElements++] = *node;
  DFS(node->right);
}
#include "Matrix.h"

#include <exception>

Matrix::Matrix(int nrLines, int nrCols) {
  this->nrL = nrLines;
  this->nrC = nrCols;
  this->size = 0;
  this->capacity = 5;
  this->triplets = new Triple[this->capacity];
}

Matrix::~Matrix() { delete[] this->triplets; }

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
Matrix::Matrix(Matrix& m) {
  this->nrL = m.nrL;
  this->nrC = m.nrC;
  this->size = m.size;
  this->capacity = m.capacity;
  this->triplets = new Triple[this->capacity];
  for (int i = 0; i < this->size; ++i) this->triplets[i] = m.triplets[i];
}

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
Matrix& Matrix::operator=(Matrix& m) {
  if (this == &m) return *this;
  this->nrL = m.nrL;
  this->nrC = m.nrC;
  this->size = m.size;
  this->capacity = m.capacity;
  delete[] this->triplets;
  this->triplets = new Triple[this->capacity];
  for (int i = 0; i < this->size; ++i) this->triplets[i] = m.triplets[i];
  return *this;
}

// Best case: Theta(1) | Worst case: Theta(1) | Average case: Theta(1)
int Matrix::nrLines() const { return this->nrL; }

// Best case: Theta(1) | Worst case: Theta(1) | Average case: Theta(1)
int Matrix::nrColumns() const { return this->nrC; }

// Best case: Theta(1) | Worst case: Theta(size) | Average case: O(size)
TElem Matrix::element(int i, int j) const {
  if (i < 0 || i >= this->nrL || j < 0 || j >= this->nrC)
    throw std::exception();
  int index = this->tripleExists(i, j);
  if (index != -1) {
    return this->triplets[index].second;
  } else {
    return NULL_TELEM;
  }
}

// Best case: Theta(1) | Worst case: Theta(size) | Average case: O(size)
int Matrix::tripleExists(int i, int j) const {
  for (int index = 0; index < this->size; ++index) {
    if (this->triplets[index].first.first == i &&
        this->triplets[index].first.second == j)
      return index;
  }
  return -1;
}

// Best case: Theta(1) | Worst case: Theta(size) | Average case: O(size)
TElem Matrix::modify(int i, int j, TElem e) {
  if (i < 0 || i >= this->nrL || j < 0 || j >= this->nrC)
    throw std::exception();
  int index = this->tripleExists(i, j);
  TElem oldValue = NULL_TELEM;
  if (e == NULL_TELEM) {
    if (index != -1) {  // we want to modify an element to the value NULL_TELEM,
      // thus it will be removed since we have impelemnted a sparse matrix
      oldValue = this->triplets[index].second;
      this->removeAtIndex(index);
      return oldValue;
    } else {  // we want to add a NULL_TELEM in the Matrix
      return NULL_TELEM;
    }
  }
  if (index == -1) {  // triple is not in the matrix
    int indexToInsert = this->getIndexAtWhichItShouldInsert(i, j);
    Triple t = Triple(std::pair<int, int>(i, j), e);
    this->insertAtIndex(t, indexToInsert);
  } else {
    oldValue = this->triplets[index].second;
    this->triplets[index].second = e;
  }
  return oldValue;
}

// Best case: Theta(1) | Worst case: Theta(size) | Average case: O(size)
void Matrix::setMainDiagonal(TElem elem) {
  if (this->nrL != this->nrC) throw std::exception();
  for (int i = 0; i < this->nrL; i++) this->modify(i, i, elem);
}

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
void Matrix::resize() {
  this->capacity *= 2;
  Triple* newTriplets = new Triple[this->capacity];
  for (int i = 0; i < this->size; ++i) newTriplets[i] = this->triplets[i];
  delete[] this->triplets;
  this->triplets = newTriplets;
}

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
int Matrix::getIndexAtWhichItShouldInsert(int i, int j) {
  for (int index = 0; index < this->size; ++index) {
    if (this->triplets[index].first.first > i ||
        (this->triplets[index].first.first == i &&
         this->triplets[index].first.second > j))
      return index + 1;
  }
  return this->size;
}

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
void Matrix::insertAtIndex(Triple& t, int index) {
  if (this->size >= this->capacity) this->resize();
  for (int i = this->size; i > index; --i)
    this->triplets[i] = this->triplets[i - 1];
  this->triplets[index] = t;
  this->size++;
}

// Best case: Theta(size) | Worst case: Theta(size) | Average case: Theta(size)
void Matrix::removeAtIndex(int index) {
  for (int i = index; i < this->size - 1; ++i)
    this->triplets[i] = this->triplets[i + 1];
  this->size--;
}
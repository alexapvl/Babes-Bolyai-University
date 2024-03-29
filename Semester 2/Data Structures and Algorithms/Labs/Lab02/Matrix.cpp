#include "Matrix.h"

#include <exception>

Matrix::Matrix(int nrLines, int nrCols) {
  this->nrL = nrLines;
  this->nrC = nrCols;
  this->size = 0;
  this->capacity = this->nrL * this->nrC;
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

// Best case: Theta(1) | Worst case: Theta(1) | Average case: Theta(1)
TElem Matrix::modify(int i, int j, TElem e) {
  if (i < 0 || i >= this->nrL || j < 0 || j >= this->nrC)
    throw std::exception();
  int index = this->tripleExists(i, j);
  TElem oldValue = NULL_TELEM;
  if (index == -1) {  // triple is not in the matrix
    this->triplets[this->size].first.first = i;
    this->triplets[this->size].first.second = j;
    this->triplets[this->size].second = e;
    this->size++;
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

#pragma once
#include "movie.h"

template <typename T>
class DynamicVector {
 private:
  T* elements;
  int size, capacity;
  void resize(int factorOfResizing = 2);

 public:
  DynamicVector(int initialCapacity = 10);
  DynamicVector(const DynamicVector& parameterVector);
  ~DynamicVector();
  DynamicVector& operator=(const DynamicVector& assignmnetVector);
  void addElementInDynamicVector(const T& elementToBeAdded);
  void removeElementFromDynamicVector(int positionToBeRemoved);
  void updateElementInDynamicVector(int positionToBeUpdated,
                                    const T& elementToBeAdded);
  bool checkIfElementIsInDynamicVector(const T& searchedElement) const;
  int returnPosition(const T& elementToBeAdded) const;
  size_t getSize() const;
  T& operator[](int indexOfElement);
  T getElement(int positionToGetElementBy) const;

 public:
  class Iterator {
   private:
    T* pointerToVector;

   public:
    Iterator(T* pointerToVector = nullptr) : pointerToVector(pointerToVector) {}
    T& operator*() const { return *pointerToVector; }
    Iterator& operator++() {
      ++pointerToVector;
      return *this;
    }
    Iterator operator++(int) {
      Iterator copyOfCurrentIterator = *this;
      pointerToVector++;
      return copyOfCurrentIterator;
    }
    bool operator==(const Iterator& otherIterator) const {
      return pointerToVector == otherIterator.pointerToVector;
    }
    bool operator!=(const Iterator& otherIterator) const {
      return pointerToVector != otherIterator.pointerToVector;
    }
  };
  Iterator begin() const { return Iterator(this->elements); }
  Iterator end() const { return Iterator(this->elements + this->size); }
};

template <typename T>
DynamicVector<T>::DynamicVector(int initialCapacity) {
  this->size = 0;
  this->capacity = initialCapacity;
  this->elements = new T[initialCapacity];
}

template <typename T>
DynamicVector<T>::DynamicVector(const DynamicVector& parameterVector) {
  this->size = parameterVector.size;
  this->capacity = parameterVector.capacity;
  this->elements = new T[this->capacity];
  for (int index = 0; index < this->size; ++index)
    this->elements[index] = parameterVector.elements[index];
}

template <typename T>
DynamicVector<T>::~DynamicVector() {
  delete[] this->elements;
}

template <typename T>
DynamicVector<T>& DynamicVector<T>::operator=(
    const DynamicVector& assignmnetVector) {
  if (this == &assignmnetVector) return *this;
  this->size = assignmnetVector.size;
  this->capacity = assignmnetVector.capacity;
  delete[] this->elements;
  this->elements = new T[this->capacity];
  for (int index = 0; index < this->size; ++index)
    this->elements[index] = assignmnetVector.elements[index];
  return *this;
}

template <typename T>
void DynamicVector<T>::addElementInDynamicVector(
    const T& elementToBeAddedToTheVector) {
  if (this->size == this->capacity) this->resize();
  this->elements[this->size] = elementToBeAddedToTheVector;
  ++this->size;
}

template <typename T>
int DynamicVector<T>::returnPosition(
    const T& elementToBeAddedToTheVector) const {
  for (int index = 0; index < this->size; ++index)
    if (this->elements[index] == elementToBeAddedToTheVector) return index;
  return -1;
}

template <typename T>
void DynamicVector<T>::removeElementFromDynamicVector(int positionToBeRemoved) {
  if (positionToBeRemoved < 0 || positionToBeRemoved >= this->size)
    throw std::invalid_argument("Invalid position");
  for (int index = positionToBeRemoved; index < this->size - 1; ++index)
    this->elements[index] = this->elements[index + 1];
  --this->size;
}

template <typename T>
size_t DynamicVector<T>::getSize() const {
  return this->size;
}

template <typename T>
T& DynamicVector<T>::operator[](int indexOfElement) {
  return this->elements[indexOfElement];
}

template <typename T>
void DynamicVector<T>::resize(int factorOfResizing) {
  this->capacity *= factorOfResizing;
  T* newResizedVector = new T[this->capacity];
  for (int index = 0; index < this->size; ++index)
    newResizedVector[index] = this->elements[index];
  delete[] this->elements;
  this->elements = newResizedVector;
}

template <typename T>
void DynamicVector<T>::updateElementInDynamicVector(int positionToBeUpdated,
                                                    const T& updatedElement) {
  this->elements[positionToBeUpdated] = updatedElement;
}

template <typename T>
T DynamicVector<T>::getElement(int positionToGetElementBy) const {
  if (positionToGetElementBy < 0 || positionToGetElementBy >= this->size)
    throw std::invalid_argument("Invalid position");
  return this->elements[positionToGetElementBy];
}

template <typename T>
bool DynamicVector<T>::checkIfElementIsInDynamicVector(
    const T& searchedElement) const {
  for (int index = 0; index < this->size; ++index)
    if (this->elements[index] == searchedElement) return true;
  return false;
}

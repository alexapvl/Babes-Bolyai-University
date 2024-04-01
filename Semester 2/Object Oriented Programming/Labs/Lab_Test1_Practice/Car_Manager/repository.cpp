#include "repository.h"

Repository::Repository() { this->data = std::vector<Car>(); }

Repository::~Repository() {}

void Repository::addCar(Car car) { this->data.push_back(car); }

void Repository::removeCar(int index) {
  this->data.erase(this->data.begin() + index);
}

int Repository::getIndex(Car car) {
  for (int i = 0; i < this->data.size(); ++i) {
    if (car == this->data[i]) return i;
  }
  return -1;
}

std::vector<Car> Repository::getAll() const { return this->data; }
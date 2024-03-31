#include "../../include/domain/car.h"

Car::Car(const std::string& manufacturer, const std::string& model,
         const int year, const std::string& color)
    : manufacturer(manufacturer), model(model), year(year), color(color){};

Car::~Car() {}

Car::Car(const Car& car) {
  this->manufacturer = car.manufacturer;
  this->model = car.model;
  this->year = car.year;
  this->color = car.color;
}

bool Car::operator==(const Car& otherCar) const {
  return this->model == otherCar.model && this->year == otherCar.year;
}

std::string Car::getManufacturer() const { return this->manufacturer; }
std::string Car::getModel() const { return this->model; }
int Car::getYear() const { return this->year; }
std::string Car::getColor() const { return this->color; }

void Car::setManufacturer(const std::string& manufacturer) {
  this->manufacturer = manufacturer;
}
void Car::setModel(const std::string& model) { this->model = model; }
void Car::setYear(const int year) { this->year = year; }
void Car::setColor(const std::string& color) { this->color = color; }

std::string Car::toString() const {
  return this->getManufacturer() + " | " + this->getModel() + " | " +
         std::to_string(this->getYear()) + " | " + this->getColor();
}

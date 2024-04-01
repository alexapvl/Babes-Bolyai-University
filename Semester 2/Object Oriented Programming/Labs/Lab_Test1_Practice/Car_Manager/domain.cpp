#include "domain.h"

Car::Car(const std::string& manufacturer, const std::string& model, int year,
         const std::string& color)
    : manufacturer(manufacturer), model(model), year(year), color(color) {}

Car::~Car() {}

void Car::setManufacturer(const std::string& manufacturer) {
  this->manufacturer = manufacturer;
}

void Car::setModel(const std::string& model) { this->model = model; }

void Car::setYear(const int year) { this->year = year; }

void Car::setColor(const std::string color) { this->color = color; }

std::string Car::getManufacturer() { return this->manufacturer; }

std::string Car::getModel() { return this->model; }

int Car::getYear() { return this->year; }

std::string Car::getColor() { return this->color; }

std::string Car::toString() {
  return this->getManufacturer() + " | " + this->getModel() + " | " +
         std::to_string(this->getYear()) + " | " + this->getColor();
}

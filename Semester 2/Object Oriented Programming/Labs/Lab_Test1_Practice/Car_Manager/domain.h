#pragma once
#include <iostream>

class Car {
 private:
  std::string manufacturer, model;
  int year;
  std::string color;

 public:
  Car(const std::string& manufacturer, const std::string& model, int year,
      const std::string& color);
  ~Car();

  void setManufacturer(const std::string& manufacturer);
  void setModel(const std::string& model);
  void setYear(const int year);
  void setColor(const std::string color);

  std::string getManufacturer();
  std::string getModel();
  int getYear();
  std::string getColor();

  bool operator==(const Car& otherCar) const {
    return this->model == otherCar.model && this->year == otherCar.year;
  }

  std::string toString();
};

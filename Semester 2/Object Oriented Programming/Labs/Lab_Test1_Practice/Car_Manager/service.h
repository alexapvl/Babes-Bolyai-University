#pragma once
#include "domain.h"
#include "repository.h"

class Service {
 private:
  Repository repo;

 public:
  Service();
  ~Service();
  void addCar(const std::string manufacturer, const std::string model,
              const int year, const std::string color);
  void removeCar(const std::string manufacturer, const std::string model,
                 const int year, const std::string color);
  std::vector<Car> showAllCars();
  std::vector<Car> showVintage();
};

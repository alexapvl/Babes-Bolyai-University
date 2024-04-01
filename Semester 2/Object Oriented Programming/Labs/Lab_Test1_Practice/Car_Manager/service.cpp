#include "service.h"

#include <algorithm>

Service::Service() { this->repo = Repository(); }
Service::~Service() {}

void Service::addCar(const std::string manufacturer, const std::string model,
                     const int year, const std::string color) {
  /*
  Adds a car to the repository
  If the car already exists, a message will be displayed and the car will not be
  added
  */
  Car car = Car(manufacturer, model, year, color);
  int index = this->repo.getIndex(car);

  if (index == -1) {
    this->repo.addCar(car);
  } else {
    throw std::runtime_error("Car already exists");
  }
}

void Service::removeCar(const std::string manufacturer, const std::string model,
                        const int year, const std::string color) {
  Car car = Car(manufacturer, model, year, color);
  int index = this->repo.getIndex(car);

  if (index == -1) {
    throw std::runtime_error("Car does not exist");
  } else {
    this->repo.removeCar(index);
  }
}

std::vector<Car> Service::showAllCars() {
  std::vector<Car> sorted = this->repo.getAll();

  std::sort(sorted.begin(), sorted.end(), [](Car& car1, Car& car2) {
    return car1.getManufacturer() < car2.getManufacturer() ||
           (car1.getManufacturer() == car2.getManufacturer() &&
            car1.getModel() < car2.getModel());
  });

  return sorted;
}

std::vector<Car> Service::showVintage() {
  /*
  Creates a vector "sorted" with all vintage cars(older than 45 years old) and
  then sorts them based on color, in a lexicographic order
  The function returns the sorted vector of cars
  */
  std::vector<Car> sorted = std::vector<Car>();
  std::vector<Car> cars = this->repo.getAll();

  for (int i = 0; i < cars.size(); ++i) {
    if (cars[i].getYear() < 1979) {
      sorted.push_back(cars[i]);
    }
  }

  std::sort(sorted.begin(), sorted.end(), [](Car& car1, Car& car2) {
    return car1.getColor() < car2.getColor();
  });

  return sorted;
}

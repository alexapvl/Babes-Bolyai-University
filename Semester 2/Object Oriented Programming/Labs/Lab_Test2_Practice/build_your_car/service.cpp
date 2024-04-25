#include "service.h"
#include <fstream>
#include <iostream>
using namespace std;

vector<Car> Service::getCars() {
  return cars;
}

Car Service::addCar(string bodyStyle, string engineType, string fuelType, int autonomy) {
  Car car = Car(bodyStyle, createEngine(engineType, fuelType, autonomy));
  cars.push_back(car);
  return car;
}

vector<Car> Service::getCarsWithMaxPrice(double maxPrice) {
  vector<Car> result;
  for (auto car : cars) {
    if (car.computePrice() < maxPrice) {
      result.push_back(car);
    }
  }

  sort(result.begin(), result.end(), [](Car c1, Car c2) {
    return c1.computePrice() < c2.computePrice();
  });
  return result;
}

void Service::writeToFile(string fileName, vector<Car> cars) {
  std::ofstream fout(fileName);
  for (auto car : cars) {
    fout << car.toString();
  }
  fout.close();
}
#pragma once
#include "car.h"
#include "engine.h"
#include <vector>
using namespace std;

class Service {
private:
  vector<Car> cars;
  Engine* createEngine(string engineType, string fuelType, int autonomy) {
    if (engineType == "Electric") {
      return new ElectricEngine{autonomy};
    } else {
      return new TurboEngine{fuelType};
    }
  }

public:
  Service() {
    Car c1 = Car{"Sedan", new ElectricEngine(100)};
    Car c2 = Car{"Convertible", new ElectricEngine(100)};
    Car c3 = Car{"Sedan", new TurboEngine("diesel")};
    cars.push_back(c1);
    cars.push_back(c2);
    cars.push_back(c3);
  }
  ~Service() {
  }
  vector<Car> getCars();
  Car addCar(string bodyStyle, string engineType, string fuelType, int autonomy);
  vector<Car> getCarsWithMaxPrice(double maxPrice);
  void writeToFile(string fileName, vector<Car> cars);
};

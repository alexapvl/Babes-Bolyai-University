#pragma once
#include <assert.h>

#include <vector>

#include "service.h"

class Test {
 public:
  static void testAll() {
    Test::testAdd();
    Test::testVintage();
  }

  static void testAdd() {
    Service service = Service();
    service.addCar("Mercedes", "CLS", 2018, "grey");
    assert(size(service.showAllCars()) == 1);
    try {
      service.addCar("Mercedes", "CLS", 2018, "grey");
      assert(false);
    } catch (std::runtime_error& e) {
      assert(true);
    }
  }

  static void testVintage() {
    Service service = Service();
    service.addCar("Mercedes", "CLS", 2018, "grey");
    service.addCar("Ferrari", "Roma", 1975, "red");
    std::vector<Car> vintage = service.showVintage();
    assert(size(vintage) == 1);
  }
};

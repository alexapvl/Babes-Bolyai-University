#include "car.h"
using namespace std;

double Car::computePrice() {
  if (bodyStyle == "Sedan") {
    return 8000 + engine->getPrice();
  } else { // Convertible
    return 9000 + engine->getPrice();
  }
}

string Car::toString() {
  return "Body Style: " + bodyStyle + " | " + engine->toString() + " | Price: " + to_string(computePrice()) + "\n";
}

Engine* Car::getEngine() {
  return engine;
}
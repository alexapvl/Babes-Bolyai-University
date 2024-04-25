#include "engine.h"
using namespace std;

double ElectricEngine::getPrice() {
  return basePrice + 0.01 * double(autonomy);
}

string ElectricEngine::toString() {
  return "Electric Engine | Autonomy: " + to_string(autonomy);
}

double TurboEngine::getPrice() {
  if (fuelType == "gasoline")
    return basePrice + 0.01 * 100;
  else
    return basePrice + 0.01 * 150; // diesel
}

string TurboEngine::toString() {
  return "Turbo Engine | Fuel Type " + fuelType;
}
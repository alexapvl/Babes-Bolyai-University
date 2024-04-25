#pragma once
#include <string>
using namespace std;

class Engine {
protected:
  string fuelType;
  double basePrice;

public:
  Engine(string fuelType = "", double basePrice = 3000) : fuelType(fuelType), basePrice(basePrice){};
  virtual ~Engine(){};
  virtual double getPrice() { return basePrice; }
  virtual string toString() = 0;
};

class ElectricEngine : public Engine {
private:
  int autonomy;

public:
  ElectricEngine(int autonomy) : Engine{}, autonomy(autonomy){};
  ~ElectricEngine(){};
  double getPrice() override;
  string toString() override;
};

class TurboEngine : public Engine {
private:
public:
  TurboEngine(string fuelType) : Engine{fuelType} {};
  ~TurboEngine(){};
  double getPrice() override;
  string toString() override;
};
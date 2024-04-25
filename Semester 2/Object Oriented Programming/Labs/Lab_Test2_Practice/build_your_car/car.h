#pragma once
#include "engine.h"
using namespace std;

class Car {
private:
  string bodyStyle;
  Engine* engine;

public:
  Car(string bodyStyle, Engine* engine) : bodyStyle(bodyStyle), engine(engine){};
  ~Car() {}
  double computePrice();
  string toString();
  Engine* getEngine();
};
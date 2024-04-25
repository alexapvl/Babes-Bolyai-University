#pragma once
#include "service.h"
#include <string>
using namespace std;

class UI {
private:
  Service s;
  string fileName;

public:
  UI(Service s, string fileName) : s(s), fileName(fileName){};
  ~UI(){};
  void start();
  void printMenu();
  void addCar();
  void showCars();
  void saveToFile();
};
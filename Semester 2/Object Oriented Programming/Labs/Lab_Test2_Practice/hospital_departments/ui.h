#pragma once
#include "service.h"
using namespace std;

class UI {
private:
  Service service;
  string fileName;

public:
  UI(Service service, string fileName) : service(service), fileName(fileName){};
  ~UI(){};
  void start();
  void printMenu();
  void addDepartment();
  void showAllDepartments();
  void showAllEfficientDepartments();
  void writeToFile();
};
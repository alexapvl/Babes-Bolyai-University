#pragma once
#include <iostream>

#include "service.h"

class UI {
 private:
  Service service;

 public:
  UI();
  ~UI();
  void addFiveCars();
  void printMenu();
  void start();
};

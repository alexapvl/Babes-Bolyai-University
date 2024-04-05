#pragma once
#include "service.h"
#include "tests.h"

class UI {
 private:
  Service service;

 public:
  UI();
  ~UI();
  void printMenu();
  void start();
};
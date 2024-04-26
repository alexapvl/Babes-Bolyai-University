#pragma once
#include "service.h"
#include <string>
using namespace std;

class UI {
private:
  Service s;
  string filename;

public:
  UI(Service s, string filename) : s(s), filename(filename){};
  ~UI(){};
  void start();
};
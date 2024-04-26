#include "service.h"
#include <iostream>
using namespace std;

class UI {
private:
  Portfolio p;
  string filename;

public:
  UI(Portfolio p, string filename) : p(p), filename(filename){};
  ~UI(){};
  void start();
};
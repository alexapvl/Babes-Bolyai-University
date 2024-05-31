#pragma once
#include "domain.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Repository {
private:
  vector<Equation> data;
  void readFromFile() {
    ifstream file("../equations.txt");
    string line;
    while (getline(file, line)) {
      stringstream ss(line);
      string x1, x2, x3;
      getline(ss, x1, ',');
      getline(ss, x2, ',');
      getline(ss, x3);
      double a, b, c;
      a = atof(x1.c_str());
      b = atof(x2.c_str());
      c = atof(x3.c_str());
      data.push_back(Equation(a, b, c));
    }
  }

public:
  Repository() {
    readFromFile();
  }
  ~Repository() {}

  vector<Equation> getData() { return data; }

  void updatePosition(int p, double a, double b, double c) {
    data.at(p).setA(a);
    data.at(p).setB(b);
    data.at(p).setC(c);
  }

  Equation getPosition(int p) {
    return data.at(p);
  }
};
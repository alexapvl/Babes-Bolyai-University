#pragma once
#include <math.h>
#include <string>
#include <utility>

using namespace std;

class Equation {
private:
  double a, b, c;

public:
  Equation(double a = 0.0, double b = 0.0, double c = 0.0) : a(a), b(b), c(c) {}
  ~Equation() {}
  void setA(double x) { a = x; }
  void setB(double x) { b = x; }
  void setC(double x) { c = x; }

  double getA() { return a; }
  double getB() { return b; }
  double getC() { return c; }

  pair<string, string> computeSolutions() {
    double delta = b * b - 4 * a * c;
    string x1, x2; // solutions
    if (delta >= 0) {
      x1 = to_string((-b + sqrt(delta)) / 2 * a);
      x2 = to_string((-b - sqrt(delta)) / 2 * a);
    } else {
      x1 = to_string(-b / 2) + " + i*" + to_string(sqrt(abs(delta)) / 2);
      x2 = to_string(-b / 2) + " - i*" + to_string(sqrt(abs(delta)) / 2);
    }
    return make_pair(x1, x2);
  }

  bool hasRealSolutions() {
    double delta = b * b - 4 * a * c;
    if (delta >= 0) {
      return true;
    } else {
      return false;
    }
  }

  string toStr() {
    string x1 = "", x2 = "", x3 = "";
    if (a != 0.0) {
      x1 = to_string(a) + "*x^2";
    }
    if (b != 0.0) {
      if (c > 0)
        x2 = "+" + to_string(b) + "*x";
      else
        x2 = to_string(b) + "*x";
    }
    if (c != 0.0) {
      if (c > 0)
        x3 = "+" + to_string(c);
      else
        x3 = to_string(c);
    }
    return x1 + " " + x2 + " " + x3;
  }
};
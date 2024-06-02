#pragma once
#include <string>
using namespace std;

class Meal {
private:
  double start, end;
  string type, description;
  int calories;

public:
  Meal(double start, double end, string type, int calories, string description) : start(start), end(end), type(type), calories(calories), description(description) {}
  ~Meal() {}

  double getStart() { return start; }
  double getEnd() { return end; }
  string getType() { return type; }
  int getCalories() { return calories; }
  string getDescription() { return description; }

  string toStr() { return to_string(start) + " - " + to_string(end) + " | " + getType() + " | " + to_string(calories) + " | " + getDescription(); }
};
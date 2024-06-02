#pragma once
#include "domain.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Repo {
private:
  vector<Meal> meals;

public:
  Repo() {
    loadFile();
  }
  ~Repo() {}

  vector<Meal> mealsAscendingOrder() {
    vector<Meal> result = meals;
    sort(result.begin(), result.end(), [](Meal m1, Meal m2) { return m1.getStart() < m2.getStart(); });
    return result;
  }

  vector<Meal> mealsSecondList(string type, int start) {
    vector<Meal> result;
    for (auto meal : meals) {
      if (meal.getType() == type && meal.getStart() >= start)
        result.push_back(meal);
    }
    return result;
  }

  void loadFile() {
    ifstream file("../meals.txt");
    string line;
    while (getline(file, line)) {
      stringstream ss(line);
      string s, e, t, c, d;
      getline(ss, s, ';');
      getline(ss, e, ';');
      getline(ss, t, ';');
      getline(ss, c, ';');
      getline(ss, d);
      double start, end;
      int calories;
      start = stof(s);
      end = stof(e);
      calories = stoi(c);
      meals.push_back(Meal(start, end, t, calories, d));
    }
  }
};
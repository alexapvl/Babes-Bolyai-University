#pragma once
#include "repo.h"

class Service {
private:
  Repo repo;

public:
  Service(Repo repo) : repo(repo) {}
  ~Service() {}

  vector<Meal> getMealsAscendingOrder() {
    return repo.mealsAscendingOrder();
  }

  vector<Meal> mealsSecondList(string type, int start) {
    return repo.mealsSecondList(type, start);
  }
};
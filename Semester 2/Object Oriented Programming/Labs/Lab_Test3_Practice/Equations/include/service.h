#pragma once
#include "repo.h"

using namespace std;

class Service {
private:
  Repository repo;

public:
  Service(Repository repo) : repo(repo) {}
  ~Service() {}

  vector<Equation> getData() { return repo.getData(); }

  void updatePosition(int p, double a, double b, double c) {
    repo.updatePosition(p, a, b, c);
  }

  Equation getPosition(int p) {
    return repo.getPosition(p);
  }
};
#pragma once
#include "user.h"
#include <string>

using namespace std;

class Issue {
  // description, status(open or closed), reporter(name of tester), solver(name of programmer)
private:
  string description, status, reporter, solver;

public:
  Issue(string description, string status, string reporter, string solver) : description(description), status(status), reporter(reporter), solver(solver) {}
  ~Issue() {}

  string getDescription() { return description; }
  string getStatus() { return status; }
  string getReporter() { return reporter; }
  string getSolver() { return solver; }

  void solve(User* user) {
    status = "closed";
    solver = user->getName();
  }

  string toStr() { return description + ";" + status + ";" + reporter + ";" + solver; }
};
#pragma once
#include "repo.h"

class Service {
private:
  Repository* repo;

public:
  Service(Repository* repo) : repo(repo) {}
  ~Service() {
    repo->saveFile();
    delete repo;
  }

  void addUser(string name, string type) {
    User* user = new User(name, type);
    repo->addUser(user);
  }

  bool addIssue(User* u, string desc) {
    if (desc == "") {
      return false;
    }

    for (auto issue : repo->getIssues()) {
      if (issue->getDescription() == desc)
        return false;
    }

    if (u->getType() == "tester") {
      Issue* issue = new Issue(desc, "open", u->getName(), "-");
      repo->addIssue(issue);
      return true;
    }
    return false;
  }

  bool solveIssue(int index, User* user) {
    Issue* issue = repo->getIssue(index);
    if (issue->getStatus() == "open") {
      repo->solveIssue(index, user);
      return true;
    }
    return false;
  }
  vector<Issue*> getIssues() { return repo->getIssues(); }
  vector<User*> getUsers() { return repo->getUsers(); }
};
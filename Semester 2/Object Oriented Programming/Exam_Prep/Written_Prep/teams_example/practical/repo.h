#pragma once
#include "issue.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Repository {
private:
  vector<User*> users;
  vector<Issue*> issues;

public:
  Repository() { loadFile(); }
  ~Repository() { saveFile(); }

  void addUser(User* u) {
    users.push_back(u);
  }

  void addIssue(Issue* i) {
    issues.push_back(i);
  }

  void solveIssue(int index, User* user) {
    auto issue = issues.at(index);
    issue->solve(user);
  }

  Issue* getIssue(int index) {
    return issues.at(index);
  }

  void loadFile() {
    ifstream userFile("../users.txt");
    string line;
    while (getline(userFile, line)) {
      stringstream ss(line);
      string name, type;
      getline(ss, name, ';');
      getline(ss, type);
      User* user = new User(name, type);
      users.push_back(user);
    }
    userFile.close();

    ifstream issuesFile("../issues.txt");
    while (getline(issuesFile, line)) {
      stringstream ss(line);
      string desc, status, reporter, solver;
      getline(ss, desc, ';');
      getline(ss, status, ';');
      getline(ss, reporter, ';');
      getline(ss, solver);
      cout << desc << endl;
      Issue* issue = new Issue(desc, status, reporter, solver);
      issues.push_back(issue);
    }
    issuesFile.close();
    cout << "File loaded\n";
  }

  void saveFile() {
    ofstream userFile("../users.txt");
    for (auto u : users) {
      userFile << u->toStr() << endl;
    }
    userFile.close();

    ofstream issuesFile("../issues.txt");
    for (auto i : issues) {
      issuesFile << i->toStr() << endl;
    }
    issuesFile.close();
  }

  vector<Issue*> getIssues() { return issues; }
  vector<User*> getUsers() { return users; }
};
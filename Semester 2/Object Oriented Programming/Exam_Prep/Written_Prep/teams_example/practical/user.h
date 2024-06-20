#pragma once
#include <string>

using namespace std;

class User {
  // name(string), type(programmer or tester)
private:
  string name, type;

public:
  User(string name, string type) : name(name), type(type) {}
  ~User() {}
  string getName() { return name; }
  string getType() { return type; }
  bool operator==(User& other) { return this->name == other.name; }
  string toStr() { return name + ";" + type; }
};

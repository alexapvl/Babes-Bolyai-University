#pragma once
#include <iostream>

class Player {
 private:
  std::string name, nationality, team;
  int goals;

 public:
  Player(std::string name, std::string nationality, std::string team,
         int goals);
  ~Player();

  void setName(std::string name);
  void setNationality(std::string nationality);
  void setTeam(std::string team);
  void setGoals(int goals);

  std::string getName();
  std::string getNationality();
  std::string getTeam();
  int getGoals();

  bool operator==(Player other) { return this->getName() == other.getName(); }
  std::string toStr() {
    return this->getName() + " | " + this->getNationality() + " | " +
           this->getTeam() + " | " + std::to_string(this->getGoals());
  }
};
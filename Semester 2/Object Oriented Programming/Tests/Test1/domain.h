#pragma once
#include <iostream>

class Patient {
 private:
  std::string name;
  int age;
  bool infected;
  int room;

 public:
  Patient(std::string name, int age, bool infected, int room);
  ~Patient();

  void setName(std::string name);
  void setAge(int age);
  void setInfected(bool infected);
  void setRoom(int room);

  std::string getName();
  int getAge();
  bool getInfected();
  int getRoom();

  bool operator==(Patient other) { return this->getName() == other.getName(); }
  std::string toStr() {
    std::string boolean;
    if (this->getInfected()) {
      boolean = "true";
    } else {
      boolean = "false";
    }
    return this->getName() + " | " + std::to_string(this->getAge()) + " | " +
           boolean + " | " + std::to_string(this->getRoom());
  }
};
#pragma once
#include "repository.h"

class Service {
 private:
  Repo repo;

 public:
  Service();
  ~Service();

  void addPatient(std::string name, int age, bool infected, int room);
  std::vector<Patient> getPatients();
  void healPatient(std::string name);
  void updateInfection(int age);
};
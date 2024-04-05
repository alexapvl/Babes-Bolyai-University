#pragma once
#include <vector>

#include "domain.h"

class Repo {
 private:
  std::vector<Patient> data;

 public:
  Repo();
  ~Repo();

  void addPatient(Patient p);
  std::vector<Patient> getPatients();
  void healPatient(int index);
  void infectPatient(int index);
  void updateInfection(int age);
  std::vector<Patient> getInfected();
  int getIndex(Patient p);
};
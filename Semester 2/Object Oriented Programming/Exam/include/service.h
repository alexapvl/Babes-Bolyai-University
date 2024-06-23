#pragma once
#include "repository.h"
#include <vector>

using namespace std;

class Service {
private:
  Repository* repo;

public:
  Service(Repository* repo) : repo(repo) {}
  ~Service() {
    delete repo;
  }

  vector<Patient*> getPatientsWithSpecialisationAndUndiagnosed(string specialisation) {
    return repo->getPatientsWithSpecialisationAndUndiagnosed(specialisation);
  }

  vector<Patient*> getPatientWithDoctorName(string name) {
    return repo->getPatientWithDoctorName(name);
  }

  bool addPatient(Patient* p) {
    return repo->addPatient(p);
  }

  Date createDate(string date) {
    return repo->createDate(date);
  }

  vector<Doctor*> getDoctors() { return repo->getDoctors(); }

  string getDoctorNameBySpecialisation(string spec) { return repo->getDoctorNameBySpecialisation(spec); }

  void updatePatient(string name, string diagnosis, string specialisation, string doctor) {
    repo->updatePatient(name, diagnosis, specialisation, doctor);
  }

  string getDiagnosis(string name) {
    return repo->getDiagnosis(name);
  }
};
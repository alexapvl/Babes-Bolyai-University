#pragma once
#include "doctorWindow.h"
#include "service.h"
#include "subject.h"

class DoctorSession : public Subject {
private:
  Service* service;

public:
  DoctorSession(Service* service) : service(service) {}
  ~DoctorSession() {
    delete service;
  }

  bool addPatient(Patient* p) {
    return service->addPatient(p);
  }

  Date createDate(string date) {
    return service->createDate(date);
  }

  vector<Patient*> getPatientsWithSpecialisationAndUndiagnosed(string specialisation) {
    return service->getPatientsWithSpecialisationAndUndiagnosed(specialisation);
  }

  vector<Patient*> getPatientWithDoctorName(string name) {
    return service->getPatientWithDoctorName(name);
  }

  string getDoctorNameBySpecialisation(string spec) { return service->getDoctorNameBySpecialisation(spec); }

  void updatePatient(string name, string diagnosis, string specialisation, string doctor) {
    service->updatePatient(name, diagnosis, specialisation, doctor);
    notify();
  }
  string getDiagnosis(string name) { return service->getDiagnosis(name); }
};
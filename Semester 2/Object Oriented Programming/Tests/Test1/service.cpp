#include "service.h"

Service::Service() {
  this->repo = Repo();
  Patient p1 = Patient("Jessica_Thompson", 42, false, 3);
  Patient p2 = Patient("Lidia_Aspen", 30, true, 3);
  Patient p3 = Patient("Scott_Smith", 86, false, 2);
  Patient p4 = Patient("Zeno_Hardy", 37, true, 8);
  Patient p5 = Patient("Andrew_Scott", 62, false, 2);

  this->repo.addPatient(p1);
  this->repo.addPatient(p2);
  this->repo.addPatient(p3);
  this->repo.addPatient(p4);
  this->repo.addPatient(p5);
}
Service::~Service() {}

void Service::addPatient(std::string name, int age, bool infected, int room) {
  /*
  param: name(string), age(int), infected(bool), room(int)
  Adds a patient
  If the patient with the given name already exists, display an error message in
  the ui and does not add it
  */
  Patient p = Patient(name, age, infected, room);
  int index = this->repo.getIndex(p);
  if (index != -1) {
    throw std::runtime_error("Patient already exists");
  }
  this->repo.addPatient(p);
}
std::vector<Patient> Service::getPatients() { return this->repo.getPatients(); }
void Service::healPatient(std::string name) {
  /*
  param: name(string)
  Heals a patient with the given name
  If the patient does not exist, display a message in the ui
  */
  int index = this->repo.getIndex(Patient(name, 0, false, 0));
  if (index == -1) {
    throw std::runtime_error("Patient does not exist");
  }
  this->repo.healPatient(index);
}
void Service::updateInfection(int age) { this->repo.updateInfection(age); }

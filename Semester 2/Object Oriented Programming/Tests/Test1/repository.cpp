#include "repository.h"

Repo::Repo() { this->data = std::vector<Patient>(); }
Repo::~Repo() {}

void Repo::addPatient(Patient p) { this->data.push_back(p); }

std::vector<Patient> Repo::getPatients() { return this->data; }

void Repo::healPatient(int index) { this->data[index].setInfected(false); }

void Repo::infectPatient(int index) { this->data[index].setInfected(true); }

// void Repo::updateInfection(int age) {
//   for (int i = 0; i < this->getPatients().size(); ++i) {
//     if (this->data[i].getInfected()) {
//       for (int j = 0; j < this->getPatients().size(); ++j) {
//         if (j != i) {
//           if (this->data[j].getRoom() == this->data[i].getRoom())
//             this->infectPatient(j);
//           else if (this->data[j].getRoom() == (this->data[i].getRoom() - 1)
//           &&
//                    this->data[j].getAge() > age)
//             this->infectPatient(j);
//           else if (this->data[j].getRoom() == (this->data[i].getRoom() + 1)
//           &&
//                    this->data[j].getAge() > age)
//             this->infectPatient(j);
//         }
//       }
//     }
//   }
// }

void Repo::updateInfection(int age) {
  std::vector<Patient> infected = this->getInfected();
  for (int i = 0; i < infected.size(); ++i) {
    for (int j = 0; j < this->getPatients().size(); ++j) {
      if (this->getPatients()[j] != infected[i]) {
        if (this->getPatients()[j].getRoom() == infected[i].getRoom())
          this->infectPatient(j);
        else if (this->getPatients()[j].getRoom() ==
                     (infected[i].getRoom() - 1) &&
                 this->getPatients()[j].getAge() > age)
          this->infectPatient(j);
        else if (this->getPatients()[j].getRoom() ==
                     (infected[i].getRoom() + 1) &&
                 this->data[j].getAge() > age)
          this->infectPatient(j);
      }
    }
  }
}

std::vector<Patient> Repo::getInfected() {
  std::vector<Patient> infected = std::vector<Patient>();
  for (int i = 0; i < this->getPatients().size(); ++i) {
    if (this->getPatients()[i].getInfected()) {
      infected.push_back(getPatients()[i]);
    }
  }
  return infected;
}

int Repo::getIndex(Patient p) {
  for (int i = 0; i < this->getPatients().size(); ++i) {
    if (this->data[i] == p) return i;
  }
  return -1;
}
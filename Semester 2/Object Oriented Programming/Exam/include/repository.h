#pragma once
#include "domain.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

using namespace std;

class Repository {
private:
  vector<Doctor*> doctors;
  vector<Patient*> patients;

public:
  Repository() {
    loadFile();
    for (auto p : patients) {
      cout << p->toStr() << endl;
    }
  }
  ~Repository() {
    saveFile();
  }
  vector<Patient*> getPatientsWithSpecialisationAndUndiagnosed(string specialisation) {
    vector<Patient*> result;
    for (auto p : patients) {
      if (p->getDiagnosis() == "" || p->getSpecialisation() == specialisation) {
        result.push_back(p);
      }
    }
    sort(result.begin(), result.end(), [](Patient* p1, Patient* p2) { return !(p1->getAddmissionDate().toStr() < p2->getAddmissionDate().toStr()); });
    return result;
  }

  vector<Patient*> getPatientWithDoctorName(string name) {
    vector<Patient*> result;
    for (auto p : patients) {
      if (p->getDoctorName() == name) {
        result.push_back(p);
      }
    }
    return result;
  }

  bool addPatient(Patient* p) {
    if (p->getAddmissionDate().isOk()) {
      patients.push_back(p);
      return true;
    }
    return false;
  }

  string getDoctorNameBySpecialisation(string spec) {
    for (auto d : doctors) {
      if (d->getSpecialisation() == spec)
        return d->getName();
    }
    return "";
  }

  string getDiagnosis(string name) {
    for (auto p : patients) {
      if (p->getName() == name)
        return p->getDiagnosis();
    }
    return "";
  }

  void updatePatient(string name, string diagnosis, string specialisation, string doctor) {
    for (auto p : patients) {
      if (p->getName() == name) {
        p->setDiagnosis(diagnosis);
        p->setSpecialisation(specialisation);
        p->setDoctor(doctor);
        return;
      }
    }
  }

  vector<Doctor*> getDoctors() { return doctors; }

  Date createDate(string date) {
    string day, month, year;
    stringstream sd(date);
    getline(sd, day, '.');
    getline(sd, month, '.');
    getline(sd, year);
    return Date(stoi(day), stoi(month), stoi(year));
  }

  void loadFile() {
    ifstream file1("../doctors.txt");
    string line;
    while (getline(file1, line)) {
      stringstream ss(line);
      string name, specialisation;
      getline(ss, name, ';');
      getline(ss, specialisation);
      doctors.push_back(new Doctor(name, specialisation));
    }
    cout << "loaded dosctors\n";
    file1.close();
    ifstream file("../patients.txt");
    while (getline(file, line)) {
      stringstream ss(line);
      string name, diagnosis, specialisation, current_doctor, date;
      getline(ss, name, ';');
      getline(ss, diagnosis, ';');
      getline(ss, specialisation, ';');
      getline(ss, current_doctor, ';');
      getline(ss, date);

      string day, month, year;
      stringstream sd(date);
      getline(sd, day, '.');
      getline(sd, month, '.');
      getline(sd, year);
      Date admission_date(stoi(day), stoi(month), stoi(year));
      patients.push_back(new Patient(name, diagnosis, specialisation, current_doctor, admission_date));
    }
    cout << "loaded patients\n";
    file.close();
  }

  void saveFile() {
    ofstream file("../patients.txt");
    for (auto p : patients) {
      file << p->toStr() << endl;
    }
    cout << "file saved\n";
  }
};
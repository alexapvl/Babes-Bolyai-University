#pragma once
#include <string>

using namespace std;

class Date {
private:
  int day, month, year;

public:
  Date(int day = 0, int month = 0, int year = 0) : day(day), month(month), year(year) {}
  ~Date() {}
  bool operator<(Date& other) {
    if (year < other.year)
      return true;
    else if (year == other.year && month < other.month)
      return true;
    else if (year == other.year && month == other.month && day < other.day)
      return true;
    return false;
  }
  bool isOk() {
    Date date(21, 6, 2024);
    if (*this < date) {
      return false;
    }
    return true;
  }
  string toStr() { return to_string(day) + "." + to_string(month) + "." + to_string(year); }
};

class Doctor {
private:
  string name, specialisation;

public:
  Doctor(string name, string specialisation) : name(name), specialisation(specialisation) {}
  ~Doctor() {}
  string getName() { return name; }
  string getSpecialisation() { return specialisation; }
  string toStr() { return name + ";" + specialisation; }
};

class Patient {
private:
  string name, diagnosis, specialisation, current_doctor;
  Date addmision_date;

public:
  Patient(string name, string diagnosis = "undiagnosed", string specialisation = "", string current_doctor = "", Date addmision_date = Date(0, 0, 0)) : name(name), diagnosis(diagnosis), specialisation(specialisation), current_doctor(current_doctor), addmision_date(addmision_date) {}
  ~Patient() {}

  string getName() { return name; }
  string getDiagnosis() { return diagnosis; }
  string getSpecialisation() { return specialisation; }
  string getDoctorName() { return current_doctor; }
  Date getAddmissionDate() { return addmision_date; }
  string toStr() { return name + ";" + diagnosis + ";" + specialisation + ";" + current_doctor + ";" + addmision_date.toStr(); }

  void setDoctor(string doc) { current_doctor = doc; }
  void setDiagnosis(string diag) { diagnosis = diag; }
  void setSpecialisation(string spec) { specialisation = spec; }
};

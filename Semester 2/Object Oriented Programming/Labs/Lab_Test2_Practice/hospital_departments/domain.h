#pragma once
#include <string>
using namespace std;

class HospitalDepartment {
protected:
  string hospitalName;
  int numberOfDoctors;

public:
  HospitalDepartment(string hospitalName, int numberOfDoctors) : hospitalName(hospitalName), numberOfDoctors(numberOfDoctors){};
  virtual ~HospitalDepartment(){};
  virtual bool isEfficient() = 0;
  virtual string toString() { return "Hospital name: " + hospitalName + ", Number of doctors: " + to_string(numberOfDoctors); }
  string getName() { return hospitalName; }
};

class NeonatalUnit : public HospitalDepartment {
private:
  int numberOfMothers;
  int numberOfNewborns;
  double averageGrade;

public:
  NeonatalUnit(string hospitalName, int numberOfDoctors, int numberOfMothers, int numberOfNewborns, double averageGrade) : HospitalDepartment{hospitalName, numberOfDoctors}, numberOfMothers(numberOfMothers), numberOfNewborns(numberOfNewborns), averageGrade(averageGrade){};
  ~NeonatalUnit(){};
  bool isEfficient() override;
  string toString() override;
};

class Surgery : public HospitalDepartment {
private:
  int numberOfPatients;

public:
  Surgery(string hospitalName, int numberOfDoctors, int numberOfPatients) : HospitalDepartment{hospitalName, numberOfDoctors}, numberOfPatients(numberOfPatients){};
  ~Surgery(){};
  bool isEfficient() override;
  string toString() override;
};

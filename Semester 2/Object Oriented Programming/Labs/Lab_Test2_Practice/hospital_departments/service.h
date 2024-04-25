#pragma once
#include "domain.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Service {
private:
  vector<HospitalDepartment*> hospitals;

public:
  Service() {
    HospitalDepartment* d1 = new Surgery("Regina Maria", 50, 210);
    HospitalDepartment* d2 = new NeonatalUnit("Spital Neonatal", 20, 50, 52, 8.6);
    HospitalDepartment* d3 = new Surgery("Spital Urgente Cluj", 35, 400);
    addDepartment(d1);
    addDepartment(d2);
    addDepartment(d3);
  };
  ~Service(){};
  void addDepartment(HospitalDepartment* d);
  vector<HospitalDepartment*> getAllDepartments();
  vector<HospitalDepartment*> getAllEfficientDepartments();
  void writeToFile(string fileName);
};

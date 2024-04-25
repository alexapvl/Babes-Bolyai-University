#include "service.h"

void Service::addDepartment(HospitalDepartment* d) {
  hospitals.push_back(d);
}

vector<HospitalDepartment*> Service::getAllDepartments() {
  return hospitals;
}

vector<HospitalDepartment*> Service::getAllEfficientDepartments() {
  vector<HospitalDepartment*> result;
  for (auto h : hospitals) {
    if (h->isEfficient())
      result.push_back(h);
  }
  return result;
}

void Service::writeToFile(string fileName) {
  std::ofstream fout(fileName);
  vector<HospitalDepartment*> d = getAllDepartments();
  sort(d.begin(), d.end(), [](HospitalDepartment* d1, HospitalDepartment* d2) {
    return d1->getName() < d2->getName();
  });
  for (auto dep : d) {
    fout << dep->toString() << "\n";
  }
  fout.close();
}
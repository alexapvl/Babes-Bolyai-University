#pragma once
#include "repository.h"

class Service {
 private:
  Repo repo;

 public:
  Service();
  ~Service();

  void removeSchool(std::string name, Address add);
  std::vector<School> getSchools();
  std::vector<School> getSchoolsByDate(Date date);
};
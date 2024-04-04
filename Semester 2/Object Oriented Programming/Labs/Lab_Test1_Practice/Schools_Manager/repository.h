#pragma once
#include <vector>

#include "domain.h"

class Repo {
 private:
  std::vector<School> data;

 public:
  Repo();
  ~Repo();

  void addSchool(School school);
  void removeSchool(int index);
  int getIndex(School school);
  std::vector<School> getSchools();
  std::vector<School> getSchoolsByDate(Date date);
};

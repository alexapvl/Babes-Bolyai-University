#include "service.h"

Service::Service() {
  this->repo = Repo();
  School school1 =
      School("Avram_Iancu", Address(46.77, 23.60), Date(15, 4, 2022), true);
  School school2 =
      School("George_Cosbuc", Address(46.77, 23.58), Date(18, 4, 2022), false);
  School school3 = School("Alexandru_Vaida_Voievod", Address(46.77, 23.63),
                          Date(23, 4, 2022), false);
  School school4 =
      School("Romulus_Guga", Address(46.53, 24.57), Date(4, 5, 2022), false);
  School school5 = School("Colegiul_Transilvania", Address(46.54, 24.57),
                          Date(3, 5, 2022), false);

  this->repo.addSchool(school1);
  this->repo.addSchool(school2);
  this->repo.addSchool(school3);
  this->repo.addSchool(school4);
  this->repo.addSchool(school5);
}
Service::~Service() {}

void Service::removeSchool(std::string name, Address add) {
  /*
  Removes school found at given index
  If school was not found, throw exception
  */
  School school = School(name, add, Date(0, 0, 0), false);
  int index = this->repo.getIndex(school);
  if (index == -1) {
    throw std::runtime_error("School does not exist");
  }
  this->repo.removeSchool(index);
}

std::vector<School> Service::getSchools() {
  std::vector<School> schools = this->repo.getSchools();
  std::sort(schools.begin(), schools.end(),
            [](School s1, School s2) { return s1.getName() < s2.getName(); });
  return schools;
}

std::vector<School> Service::getSchoolsByDate(Date date) {
  return this->repo.getSchoolsByDate(date);
}

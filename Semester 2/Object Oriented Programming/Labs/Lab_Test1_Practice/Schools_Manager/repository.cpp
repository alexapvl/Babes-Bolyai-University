#include "repository.h"

Repo::Repo() { this->data = std::vector<School>(); }
Repo::~Repo() {}

void Repo::addSchool(School school) { this->data.push_back(school); }

void Repo::removeSchool(int index) {
  this->data.erase(this->data.begin() + index);
}

int Repo::getIndex(School school) {
  for (int i = 0; i < this->data.size(); ++i) {
    if (school == this->data[i]) return i;
  }
  return -1;
}

std::vector<School> Repo::getSchools() { return this->data; }

std::vector<School> Repo::getSchoolsByDate(Date date) {
  /*
  Creates a vector with the schools that need to be visited(aka are after the
  given date) and returns that vector Also changes in memory the state of the
  schools which were visited(aka the schools which are before the given date)
  */
  std::vector<School> dschool = std::vector<School>();
  for (int i = 0; i < this->getSchools().size(); ++i) {
    if (this->data[i].getDate() < date) {
      this->data[i].setVisited(true);
    } else {
      dschool.push_back(this->data[i]);  // add schools to be visited
    }
  }
  return dschool;
}
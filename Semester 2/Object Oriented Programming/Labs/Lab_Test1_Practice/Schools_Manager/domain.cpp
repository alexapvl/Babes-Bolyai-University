#include "domain.h"

School::School(std::string name, Address address, Date date, bool visited)
    : name(name), address(address), date(date), visited(visited) {}
School::~School() {}

void School::setName(std::string name) { this->name = name; }
void School::setAddress(Address address) { this->address = address; }
void School::setDate(Date date) { this->date = date; }
void School::setVisited(bool visited) { this->visited = visited; }

std::string School::getName() { return this->name; }
Address School::getAddress() { return this->address; }
Date School::getDate() { return this->date; }
bool School::getVisited() { return this->visited; }

std::string School::toStr() {
  std::string boolean;
  if (this->getVisited())
    boolean = "true";
  else
    boolean = "false";
  return this->getName() + " | " + this->getAddress().toStr() + " | " +
         this->getDate().toStr() + " | " + boolean;
}
bool School::operator==(School other) {
  return this->getName() == other.getName() &&
         this->getAddress() == other.getAddress();
}

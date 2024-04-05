#include "domain.h"

Patient::Patient(std::string name, int age, bool infected, int room)
    : name(name), age(age), infected(infected), room(room) {}
Patient::~Patient() {}

void Patient::setName(std::string name) { this->name = name; }
void Patient::setAge(int age) { this->age = age; }
void Patient::setInfected(bool infected) { this->infected = infected; }
void Patient::setRoom(int room) { this->room = room; }

std::string Patient::getName() { return this->name; }
int Patient::getAge() { return this->age; }
bool Patient::getInfected() { return this->infected; }
int Patient::getRoom() { return this->room; }
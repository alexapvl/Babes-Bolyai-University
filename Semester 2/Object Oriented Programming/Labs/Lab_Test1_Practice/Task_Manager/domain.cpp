#include "domain.h"

Task::Task(std::string description, int minutes, int priority)
    : description(description), minutes(minutes), priority(priority) {}
Task::~Task() {}

void Task::setDesc(std::string description) { this->description = description; }
void Task::setMin(int min) { this->minutes = min; }
void Task::setPriority(int priority) { this->priority = priority; }

std::string Task::getDesc() { return this->description; }
int Task::getMin() { return this->minutes; }
int Task::getPriority() { return this->priority; }

bool Task::operator==(Task other) { return this->getDesc() == other.getDesc(); }
bool Task::operator>(Task other) { return this->getMin() > other.getMin(); }

std::string Task::toStr() {
  return this->getDesc() + " | " + std::to_string(this->getMin()) + " | " +
         std::to_string(this->getPriority());
}
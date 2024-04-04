#include "repository.h"

#include <vector>

Repo::Repo() { this->data = std::vector<Task>(); }
Repo::~Repo() {}

void Repo::addTask(Task task) { this->data.push_back(task); }

std::vector<Task> Repo::getTasks() { return this->data; }

std::vector<Task> Repo::getPriorityTasks(int priority) {
  std::vector<Task> ptasks = std::vector<Task>();
  for (int i = 0; i < this->getTasks().size(); ++i) {
    if (this->data[i].getPriority() < priority) ptasks.push_back(this->data[i]);
  }
  return ptasks;
}
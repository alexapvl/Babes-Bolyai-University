#pragma once
#include <iostream>
#include <vector>

#include "repository.h"

class Service {
 private:
  Repo repo;

 public:
  Service();
  ~Service();

  void addTask(std::string desc, int min, int priority);
  std::vector<Task> getTasks();
  std::vector<Task> getPriorityTasksDescending(int priority);
};
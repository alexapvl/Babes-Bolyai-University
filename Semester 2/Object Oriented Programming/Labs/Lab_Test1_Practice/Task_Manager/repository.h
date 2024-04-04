#pragma once
#include <iostream>
#include <vector>

#include "domain.h"

class Repo {
 private:
  std::vector<Task> data;

 public:
  Repo();
  ~Repo();

  void addTask(Task task);
  std::vector<Task> getTasks();
  std::vector<Task> getPriorityTasks(int priority);
};
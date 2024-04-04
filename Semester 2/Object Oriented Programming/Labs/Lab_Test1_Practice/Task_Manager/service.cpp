#include "service.h"

Service::Service() {
  this->repo = Repo();
  Task task1 = Task("Solve_OOP_Assignment", 120, 1);
  Task task2 = Task("Respond_to_emails", 54, 2);
  Task task3 = Task("Eat_sushi", 30, 3);
  Task task4 = Task("Visit_parent", 240, 1);
  Task task5 = Task("Buy_a_jacket", 120, 2);

  this->repo.addTask(task1);
  this->repo.addTask(task2);
  this->repo.addTask(task3);
  this->repo.addTask(task4);
  this->repo.addTask(task5);
}

Service::~Service() {}

void Service::addTask(std::string desc, int min, int priority) {
  Task task = Task(desc, min, priority);
  bool found = false;
  for (int i = 0; i < this->repo.getTasks().size(); ++i) {
    if (this->repo.getTasks()[i] == task) found = true;
  }
  if (!found)
    this->repo.addTask(task);
  else
    throw std::runtime_error("Task already exists");
}

std::vector<Task> Service::getTasks() { return this->repo.getTasks(); }

std::vector<Task> Service::getPriorityTasksDescending(int priority) {
  std::vector<Task> ptasks = this->repo.getPriorityTasks(priority);
  std::sort(ptasks.begin(), ptasks.end(),
            [](Task task1, Task task2) { return task1 > task2; });

  return ptasks;
}
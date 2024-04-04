#pragma once
#include <assert.h>

#include <iostream>

#include "repository.h"

class Test {
 public:
  static void testAll() {
    Test::testAdd();
    Test::testPriority();
  }
  static void testAdd() {
    Repo repo = Repo();
    Task task1 = Task("Solve_OOP_Assignment", 120, 1);
    Task task2 = Task("Respond_to_emails", 54, 2);
    repo.addTask(task1);
    assert(repo.getTasks().size() == 1);
    repo.addTask(task2);
    assert(repo.getTasks().size() == 2);
  }
  static void testPriority() {
    Repo repo = Repo();
    Task task1 = Task("Solve_OOP_Assignment", 120, 1);
    Task task2 = Task("Respond_to_emails", 54, 2);
    repo.addTask(task1);
    repo.addTask(task2);
    assert(repo.getPriorityTasks(3).size() == 2);
    assert(repo.getPriorityTasks(2).size() == 1);
  }
};
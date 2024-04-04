#include "ui.h"

UI::UI() { this->service = Service(); }
UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Add task\n";
  std::cout << "2. Show all tasks\n";
  std::cout << "3. Show all tasks having the priority smaller than a given "
               "one(descending by priotiry)\n";
  std::cout << "4. Exit\n";
}

void UI::start() {
  int choice;
  while (true) {
    this->printMenu();
    std::cin >> choice;
    try {
      if (choice == 1) {
        std::string desc;
        int min, priority;
        std::cout << "Description: ";
        std::cin >> desc;
        std::cout << "Minutes estimated: ";
        std::cin >> min;
        std::cout << "Priority level: ";
        std::cin >> priority;
        this->service.addTask(desc, min, priority);
        std::cout << "Task added successfully\n";
      } else if (choice == 2) {
        std::vector<Task> tasks = this->service.getTasks();
        for (int i = 0; i < tasks.size(); ++i) {
          std::cout << tasks[i].toStr() << "\n";
        }
      } else if (choice == 3) {
        int priority;
        std::cout << "Priority level: ";
        std::cin >> priority;
        std::vector<Task> sortedPriorityTasks =
            this->service.getPriorityTasksDescending(priority);
        for (int i = 0; i < sortedPriorityTasks.size(); ++i) {
          std::cout << sortedPriorityTasks[i].toStr() << "\n";
        }
      } else if (choice == 4) {
        break;
      } else {
        throw std::runtime_error("Invalid option");
      }
    } catch (std::runtime_error& e) {
      std::cout << e.what() << "\n";
    }
  }
}
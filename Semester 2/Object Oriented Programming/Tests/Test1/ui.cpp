#include "ui.h"

UI::UI() { this->service = Service(); }

UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Add patient\n";
  std::cout << "2. Show all patients\n";
  std::cout << "3. Heal patient by given name\n";
  std::cout << "4. Update infection by given age\n";
  std::cout << "5. Exit\n";
}

void UI::start() {
  int choice;
  while (true) {
    this->printMenu();
    std::cout << ">> ";
    std::cin >> choice;
    try {
      if (choice == 1) {
        std::string name, infected;
        int age, room;
        std::cout << "Name: ";
        std::cin >> name;
        std::cout << "Age: ";
        std::cin >> age;
        std::cout << "Infected: ";
        std::cin >> infected;
        std::cout << "Room: ";
        std::cin >> room;
        if (infected == "true")
          this->service.addPatient(name, age, true, room);
        else if (infected == "false")
          this->service.addPatient(name, age, false, room);
        else
          throw std::runtime_error(
              "Invalid infection status, should be true or false");
        std::cout << "Patient added successfully\n";
      } else if (choice == 2) {
        std::vector<Patient> patients = this->service.getPatients();
        for (int i = 0; i < patients.size(); ++i) {
          std::cout << patients[i].toStr() << "\n";
        }
      } else if (choice == 3) {
        std::string name;
        std::cout << "Name: ";
        std::cin >> name;
        this->service.healPatient(name);
        std::cout << "Patient healed successfully\n";
      } else if (choice == 4) {
        int age;
        std::cout << "Age: ";
        std::cin >> age;
        this->service.updateInfection(age);
        std::cout << "Infection updated\n";
      } else if (choice == 5) {
        break;
      } else {
        throw std::runtime_error("Invalid option");
      }
    } catch (std::runtime_error& e) {
      std::cout << e.what() << "\n";
    }
  }
}
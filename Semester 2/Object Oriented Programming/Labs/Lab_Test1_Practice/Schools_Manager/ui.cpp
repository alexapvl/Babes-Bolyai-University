#include "ui.h"

UI::UI() { this->service = Service(); }
UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Remove school\n";
  std::cout << "2. Show all schools(sorted by name)\n";
  std::cout << "3. Show unvisited schools given by date\n";
  std::cout << "4. Exit\n";
}

void UI::start() {
  int choice;
  while (true) {
    this->printMenu();
    std::cin >> choice;
    try {
      if (choice == 1) {
        std::string name;
        double longitude, latitude;
        std::cout << "Name: ";
        std::cin >> name;
        std::cout << "Longitude: ";
        std::cin >> longitude;
        std::cout << "Latitude: ";
        std::cin >> latitude;
        this->service.removeSchool(name, Address(longitude, latitude));
        std::cout << "School removed successfully\n";
      } else if (choice == 2) {
        std::vector<School> schools = this->service.getSchools();
        for (int i = 0; i < schools.size(); ++i) {
          std::cout << schools[i].toStr() << "\n";
        }
      } else if (choice == 3) {
        int day, month, year;
        std::cout << "Day: ";
        std::cin >> day;
        std::cout << "Month: ";
        std::cin >> month;
        std::cout << "Year: ";
        std::cin >> year;
        std::vector<School> schools =
            this->service.getSchoolsByDate(Date(day, month, year));
        for (int i = 0; i < schools.size(); ++i) {
          std::cout << schools[i].toStr() << "\n";
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
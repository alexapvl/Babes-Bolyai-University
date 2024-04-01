#include "ui.h"

UI::UI() { this->service = Service(); }
UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Add car" << std::endl;
  std::cout << "2. Remove car" << std::endl;
  std::cout << "3. Show all cars" << std::endl;
  std::cout << "4. Show vintage cars" << std::endl;
  std::cout << "0. Exit" << std::endl;
}

void UI::addFiveCars() {
  this->service.addCar("Fiat", "Bravo", 2007, "red");
  this->service.addCar("Fiat", "Idea", 2003, "black");
  this->service.addCar("Audi", "A5", 2007, "blue");
  this->service.addCar("BMV", "Coupe", 2013, "pink");
  this->service.addCar("Ford", "Fiesta", 1976, "yellow");
}

void UI::start() {
  this->addFiveCars();
  int choice = -1;
  while (true) {
    try {
      UI::printMenu();
      std::cin >> choice;
      if (choice == 1) {
        std::string manufacturer, model, color;
        int year;
        std::cout << "Manufacturer: ";
        std::cin >> manufacturer;
        std::cout << "Model: ";
        std::cin >> model;
        std::cout << "Year: ";
        std::cin >> year;
        std::cout << "Color: ";
        std::cin >> color;
        this->service.addCar(manufacturer, model, year, color);
      } else if (choice == 2) {
        std::string manufacturer, model, color;
        int year;
        std::cout << "Manufacturer: ";
        std::cin >> manufacturer;
        std::cout << "Model: ";
        std::cin >> model;
        std::cout << "Year: ";
        std::cin >> year;
        std::cout << "Color: ";
        std::cin >> color;
        this->service.removeCar(manufacturer, model, year, color);
      } else if (choice == 3) {
        std::vector<Car> cars = this->service.showAllCars();
        for (Car car : cars) {
          std::cout << car.getManufacturer() << " | " << car.getModel() << " | "
                    << car.getYear() << " | " << car.getColor() << std::endl;
        }
      } else if (choice == 4) {
        std::vector<Car> cars = this->service.showVintage();
        for (Car car : cars) {
          std::cout << car.getManufacturer() << " | " << car.getModel() << " | "
                    << car.getYear() << " | " << car.getColor() << std::endl;
        }
      } else if (choice == 0) {
        break;
      } else {
        throw std::runtime_error("Invalid option");
      }

    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
    }
  }
}

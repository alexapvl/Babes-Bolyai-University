#include "ui.h"
#include <iostream>
using namespace std;

void UI::start() {
  while (true) {
    printMenu();
    int choice;
    cout << ">>>";
    cin >> choice;
    if (choice == 1) {
      addCar();
    } else if (choice == 2) {
      showCars();
    } else if (choice == 3) {
      saveToFile();
    } else if (choice == 0) {
      break;
    } else {
      cout << "Invalid input\n";
    }
  }
}

void UI::printMenu() {
  cout << "1. Add car\n";
  cout << "2. Show cars\n";
  cout << "3. Save to file\n";
  cout << "0. Exit\n";
}

void UI::addCar() {
  string bodyStyle, engineType, fuelType{""};
  int autonomy{0};
  cout << "Body Type(Sedan or Convertible): ";
  cin >> bodyStyle;
  cout << "\nEngine Type(Electric or Turbo): ";
  cin >> engineType;
  if (engineType == "Electric") {
    cout << "\nAutonomy: ";
    cin >> autonomy;
  } else { // Turbo
    cout << "\nFuel Type: ";
    cin >> fuelType;
  }
  Car car = s.addCar(bodyStyle, engineType, fuelType, autonomy);
  cout << "\nPrice of added car is: " << car.computePrice() << "\n";
}

void UI::showCars() {
  vector<Car> cars = s.getCars();
  for (auto car : cars) {
    cout << car.toString();
  }
}

void UI::saveToFile() {
  double maxPrice;
  cout << "\nMax price is: ";
  cin >> maxPrice;
  vector<Car> cars = s.getCarsWithMaxPrice(maxPrice);
  s.writeToFile(fileName, cars);
}
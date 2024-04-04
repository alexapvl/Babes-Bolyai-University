#include "ui.h"

UI::UI() { this->service = Service(); }

UI::~UI() {}

void UI::printMenu() {
  std::cout << "1. Remove bill\n";
  std::cout << "2. Show all bills\n";
  std::cout << "3. Sort unpaid bills by their due date\n";
  std::cout << "4. Calculate total unpaid bills\n";
  std::cout << "5. Exit\n";
}

void UI::start() {
  int choice;
  while (true) {
    this->printMenu();
    try {
      std::cin >> choice;
      if (choice == 1) {
        std::cout << "Bill serial no: ";
        std::string SN;
        std::cin >> SN;
        this->service.removeBill(SN);
      } else if (choice == 2) {
        std::vector<Bill> bills = this->service.getBills();
        for (int i = 0; i < bills.size(); ++i) {
          std::cout << bills[i].toString() << "\n";
        }
      } else if (choice == 3) {
        std::vector<Bill> unpaidSorted = this->service.getSortedUnpaidBills();
        for (int i = 0; i < unpaidSorted.size(); ++i) {
          std::cout << unpaidSorted[i].toString() << "\n";
        }
      } else if (choice == 4) {
        std::cout << "Sum to be paid: "
                  << std::to_string(this->service.totalUnpaid()) << "\n";
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
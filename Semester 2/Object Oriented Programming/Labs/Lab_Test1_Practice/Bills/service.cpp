#include "service.h"

Service::Service() {
  this->repo = Repo();
  // add 5 bills
  Bill bill1 = Bill("a", "Digi", Date(21, 4, 2016), 75, false);
  Bill bill2 = Bill("b", "Digi", Date(16, 3, 2016), 75, true);
  Bill bill3 = Bill("c", "Digi", Date(18, 4, 2016), 75, false);
  Bill bill4 = Bill("d", "Digi", Date(20, 4, 2016), 75, false);
  Bill bill5 = Bill("e", "Digi", Date(21, 4, 2016), 75, true);
  this->repo.addBill(bill1);
  this->repo.addBill(bill2);
  this->repo.addBill(bill3);
  this->repo.addBill(bill4);
  this->repo.addBill(bill5);
}
Service::~Service() {}

void Service::removeBill(std::string SN) {
  int index = this->repo.getBillIndex(SN);
  if (index == -1) throw std::runtime_error("No bill found");
  this->repo.removeBill(index);
}

std::vector<Bill> Service::getBills() { return this->repo.getAllBills(); }

std::vector<Bill> Service::getSortedUnpaidBills() {
  std::vector<Bill> unpaid = this->repo.getUnpaidBills();
  std::sort(unpaid.begin(), unpaid.end(), [](Bill bill1, Bill bill2) {
    return bill1.getDate() < bill2.getDate();
  });
  return unpaid;
}

int Service::totalUnpaid() {
  std::vector<Bill> unpaid = this->repo.getUnpaidBills();
  int sum = 0;
  for (int i = 0; i < unpaid.size(); ++i) {
    sum += unpaid[i].getSum();
  }
  return sum;
}
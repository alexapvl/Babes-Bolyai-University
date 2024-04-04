#include "repository.h"

Repo::Repo() { this->data = std::vector<Bill>(); }
Repo::~Repo() {}

void Repo::addBill(Bill bill) { this->data.push_back(bill); }
void Repo::removeBill(int index) {
  this->data.erase(this->data.begin() + index);
}
int Repo::getBillIndex(std::string SN) {
  for (int i = 0; i < this->data.size(); ++i)
    if (this->data[i].getSerialNo() == SN) return i;
  return -1;
}

std::vector<Bill> Repo::getAllBills() { return this->data; }

std::vector<Bill> Repo::getUnpaidBills() {
  std::vector<Bill> unpaid = std::vector<Bill>();
  for (int i = 0; i < this->getAllBills().size(); ++i) {
    if (this->data[i].getPaid() == false) unpaid.push_back(this->data[i]);
  }
  return unpaid;
}
#include "../include/service.h"
#include <iostream>
#include <string>

BillService::BillService() {
  bills = vector<Bill>();
  readFromFile();
}

vector<Bill> BillService::getBillsSortedByName() {
  vector<Bill> result = getBills();
  sort(result.begin(), result.end(), [](Bill b1, Bill b2) { return b1.getName() < b2.getName(); });
  return result;
}

vector<Bill> BillService::getPaidBills() {
  vector<Bill> result{};
  for (auto& bill : bills) {
    if (bill.getIsPaid()) {
      result.push_back(bill);
    }
  }
  return result;
}

vector<Bill> BillService::getUnpaidBills() {
  vector<Bill> result{};
  for (auto& bill : bills) {
    if (!bill.getIsPaid()) {
      result.push_back(bill);
    }
  }
  return result;
}

double BillService::calculateTotal(string& companyName) {
  double sum = 0;
  bool ok = false; // variable to check if the company exists in the database
  for (auto bill : bills) {
    if (bill.getName() == companyName) {
      ok = true;
      if (!bill.getIsPaid())
        sum += bill.getSum();
    }
  }
  if (!ok)
    return -1.0;
  else
    return sum;
}

void BillService::readFromFile() {
  ifstream fi("bills.txt");
  string line;
  while (getline(fi, line)) {
    // we read all lines in the file and split them using getline with a delimiter
    stringstream ss(line); // stream used as a string
    string name, serialNo, sum, isPaid;
    getline(ss, name, ';');
    getline(ss, serialNo, ';');
    getline(ss, sum, ';');
    getline(ss, isPaid); // no delimiter because delimiter will be '\n'
    double s = stod(sum);
    bool p;
    if (isPaid == "false")
      p = false;
    else
      p = true;
    bills.push_back(Bill(name, serialNo, s, p));
  }
}
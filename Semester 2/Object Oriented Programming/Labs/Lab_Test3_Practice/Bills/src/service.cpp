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

vector<string> BillService::splitParams(const string& str, char delimiter) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(str);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void BillService::readFromFile() {
  ifstream fi("../bills.txt");
  string line;
  char delimiter = ';';

  while (getline(fi, line)) {
    vector<string> tokens = BillService::splitParams(line, delimiter);
    bool paid = false;
    for (const auto& token : tokens) {
      if (tokens[3] == "true")
        paid = true;
      else if (tokens[3] == "false")
        paid = false;
    }
    Bill bill(tokens[0], tokens[1], stod(tokens[2]), paid);
    bills.push_back(bill);
  }
}
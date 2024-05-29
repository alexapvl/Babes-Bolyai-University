#pragma once
#include "domain.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class BillService {
private:
  vector<Bill> bills;

public:
  BillService();
  ~BillService() {}
  vector<Bill> getBills() { return bills; }
  vector<Bill> getBillsSortedByName();
  vector<Bill> getPaidBills();
  vector<Bill> getUnpaidBills();
  double calculateTotal(string& companyName);
  void readFromFile();
};
#pragma once

#include <string.h>

#include <iostream>
#include <vector>

#include "domain.h"

class Repo {
 private:
  std::vector<Bill> data;

 public:
  Repo();
  ~Repo();

  void addBill(Bill bill);
  void removeBill(int index);
  std::vector<Bill> getAllBills();
  std::vector<Bill> getUnpaidBills();
  int getBillIndex(std::string SN);
};
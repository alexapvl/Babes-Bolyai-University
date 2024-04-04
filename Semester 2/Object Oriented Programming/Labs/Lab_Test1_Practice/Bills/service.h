#pragma once

#include <iostream>

#include "repository.h"

class Service {
 private:
  Repo repo;

 public:
  Service();
  ~Service();

  void removeBill(std::string SN);
  std::vector<Bill> getBills();
  std::vector<Bill> getSortedUnpaidBills();
  int totalUnpaid();
};
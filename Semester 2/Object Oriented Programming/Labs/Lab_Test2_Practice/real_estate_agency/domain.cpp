#include "domain.h"

bool Person::isInterested(Dweling d) {
  return (d.getPrice() / 1000.0) <= totalIncome();
}

bool Company::isInterested(Dweling d) {
  return (d.getPrice() / 100.0) <= totalIncome() && d.getProfitable();
}
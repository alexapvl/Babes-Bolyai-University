#include "domain.h"

Bill::Bill(std::string SN, std::string company, Date date, int sum, bool paid)
    : SN(SN), company(company), date(date), sum(sum), paid(paid) {}
Bill::~Bill() {}

void Bill::setSerialNo(std::string SN) { this->SN = SN; }
void Bill::setCompany(std::string company) { this->company = company; }
void Bill::setDate(Date date) { this->date = date; }
void Bill::setSum(int sum) { this->sum = sum; }
void Bill::setPaid(bool paid) { this->paid = paid; }

std::string Bill::getSerialNo() { return this->SN; }
std::string Bill::getCompany() { return this->company; }
Date Bill::getDate() { return this->date; }
int Bill::getSum() { return this->sum; }
bool Bill::getPaid() { return this->paid; }
std::string Bill::toString() {
  std::string boolean;
  if (this->getPaid()) {
    boolean = "true";
  } else {
    boolean = "false";
  }
  return this->getSerialNo() + "; " + this->getCompany() + "; " +
         this->date.toString() + "; " + std::to_string(this->getSum()) + "; " +
         boolean;
}
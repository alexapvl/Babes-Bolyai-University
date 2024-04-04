#pragma once
#include <iostream>
#include <string>

class Date {
 public:
  Date(int day, int month, int year) : day(day), month(month), year(year){};
  ~Date() {}
  bool operator<(Date other) {
    return this->year < other.year ||
           (this->year == other.year && this->month < other.month) ||
           (this->year == other.year && this->month == other.month &&
            this->day < other.day);
  }
  std::string toString() {
    return std::to_string(this->day) + "." + std::to_string(this->month) + "." +
           std::to_string(this->year);
  }

 private:
  int day, month, year;
};

class Bill {
 private:
  std::string SN, company;
  Date date;
  int sum;
  bool paid;

 public:
  Bill(std::string SN, std::string company, Date date, int sum, bool paid);
  ~Bill();

  void setSerialNo(std::string SN);
  void setCompany(std::string company);
  void setDate(Date date);
  void setSum(int sum);
  void setPaid(bool paid);

  std::string getSerialNo();
  std::string getCompany();
  Date getDate();
  int getSum();
  bool getPaid();
  std::string toString();
};
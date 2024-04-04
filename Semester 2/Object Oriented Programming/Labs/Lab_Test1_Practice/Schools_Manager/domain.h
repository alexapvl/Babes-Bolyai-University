#pragma once
#include <iostream>

class Date {
 public:
  int day, month, year;
  Date(int d, int m, int y) : day(d), month(m), year(y) {}
  ~Date() {}
  std::string toStr() {
    return std::to_string(this->day) + "." + std::to_string(this->month) + "." +
           std::to_string(this->year);
  }
  bool operator<(Date other) {
    return this->year < other.year ||
           (this->year == other.year && this->month < other.month) ||
           (this->year == other.year && this->month == other.month &&
            this->day < other.day);
  }
};

class Address {
 public:
  double longitute, latitute;
  Address(double la, double lo) : longitute(lo), latitute(la) {}
  ~Address() {}
  std::string toStr() {
    return std::to_string(this->longitute) + ", " +
           std::to_string(this->latitute);
  }
  bool operator==(Address other) {
    return this->latitute == other.latitute &&
           this->longitute == other.longitute;
  }
};

class School {
 private:
  std::string name;
  Address address;
  Date date;
  bool visited;

 public:
  School(std::string name, Address address, Date date, bool visited);
  ~School();

  void setName(std::string name);
  void setAddress(Address address);
  void setDate(Date date);
  void setVisited(bool visited);

  std::string getName();
  Address getAddress();
  Date getDate();
  bool getVisited();

  std::string toStr();
  bool operator==(School other);
};

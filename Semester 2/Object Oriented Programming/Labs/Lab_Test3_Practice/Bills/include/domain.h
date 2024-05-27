#pragma once
#include <string>
using namespace std;

class Bill {
private:
  string name;
  string serialNo;
  double sum;
  bool isPaid;

public:
  Bill(string name = "", string serialNo = "", double sum = 0.0, bool isPaid = false) : name(name), serialNo(serialNo), sum(sum), isPaid(isPaid) {}
  ~Bill() {}
  string getName() { return name; }
  string getSerialNo() { return serialNo; }
  double getSum() { return sum; }
  bool getIsPaid() { return isPaid; }
  void setName(const string& n) { name = n; }
  void setSerialNo(const string& sn) { serialNo = sn; }
  void setSum(const double s) { sum = s; }
  void setIsPaid(const bool ip) { isPaid = ip; }
  string toStr() { return getName() + " -> " + to_string(sum); }
};
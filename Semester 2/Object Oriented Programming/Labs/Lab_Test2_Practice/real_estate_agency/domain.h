#pragma once
#include <string>
using namespace std;

class Dweling {
private:
  double price;
  bool isProfitable;

public:
  Dweling(double price, bool isProfitable) : price(price), isProfitable(isProfitable){};
  ~Dweling(){};
  double getPrice() { return price; };
  bool getProfitable() { return isProfitable; };
  string toString() {
    string profitable;
    if (isProfitable) {
      profitable = "true";
    } else {
      profitable = "false";
    }
    return "Price: " + to_string(price) + ", Is profitable: " + profitable;
  }
};

class Client {
protected:
  string name;
  double income;

public:
  Client(string name, double income) : name(name), income(income){};
  ~Client(){};
  virtual double totalIncome() { return income; };
  virtual string toString() { return "Name: " + name + ", Income: " + to_string(income); };
  virtual bool isInterested(Dweling d) = 0;
  virtual string getName() { return name; };
};

class Person : public Client {
public:
  Person(string name, double income) : Client{name, income} {};
  ~Person(){};
  string toString() override {
    return "Client | " + Client::toString();
  }
  bool isInterested(Dweling d) override;
};

class Company : public Client {
private:
  double moneyFromInvestments;

public:
  double totalIncome() override { return income + moneyFromInvestments; }
  Company(string name, double income, double moneyFromInvestments) : Client{name, income}, moneyFromInvestments(moneyFromInvestments){};
  ~Company(){};
  string toString() override {
    return "Company | " + Client::toString() + ", Money from investments: " + to_string(moneyFromInvestments) + ", Total income: " + to_string(totalIncome());
  }
  bool isInterested(Dweling d) override;
};
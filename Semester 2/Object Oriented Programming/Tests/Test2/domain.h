#pragma once
#include <string>
using namespace std;

class Instrument {
protected:
  string name;
  double buyPrice;
  double sellPrice;

public:
  Instrument(string name, double buyPrice, double sellPrice) : name(name), buyPrice(buyPrice), sellPrice(sellPrice){};
  virtual ~Instrument(){};
  virtual double computeProfit() { return 0.0; };
  virtual string toString() { return "Name: " + name; };
};

class Stock : public Instrument {
private:
  double shares;

public:
  Stock(string name, double buyPrice, double sellPrice, double shares) : Instrument{name, buyPrice, sellPrice}, shares(shares){};
  ~Stock(){};
  double computeProfit() override { return (sellPrice - buyPrice) * shares; };
  string toString() override { return Instrument::toString() + ",Type: Stock, Buy Price: $" + to_string(buyPrice) + ", Sell Price: $" + to_string(sellPrice) + ", Shares: " + to_string(shares); };
};

class Option : public Instrument {
private:
  double strikePrice;
  string optionType;

public:
  Option(string name, double buyPrice, double sellPrice, double strikePrice, string optionType) : Instrument{name, buyPrice, sellPrice}, strikePrice(strikePrice), optionType(optionType){};
  ~Option(){};
  double computeProfit() override;
  string toString() override { return Instrument::toString() + ", Type: Option, Buy Price: $" + to_string(buyPrice) + ", Sell Price: $" + to_string(sellPrice) + ", Strike Price: $" + to_string(strikePrice) + ", Type: " + optionType; };
};
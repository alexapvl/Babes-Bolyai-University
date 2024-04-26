#include "domain.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Portfolio {
private:
  vector<Instrument*> instruments;

public:
  Portfolio() {
    Instrument* i1 = new Stock("APPL", 150.0, 150.50, 100.0);
    Instrument* i2 = new Stock("BRKR", 7.0, 6.80, 20.0);
    Instrument* i3 = new Option("KO", 100.00, 101.50, 105.0, "CALL");
    instruments.push_back(i1);
    instruments.push_back(i2);
    instruments.push_back(i3);
  };
  ~Portfolio(){};
  void addInstrument(Instrument* a);
  vector<Instrument*> getAllInstruments();
  vector<Instrument*> getProfitableInstruments();
  void writeToFile(string filename, double maxProfit);
};
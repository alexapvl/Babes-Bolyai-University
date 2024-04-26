#include "service.h"

void Portfolio::addInstrument(Instrument* a) {
  instruments.push_back(a);
}

vector<Instrument*> Portfolio::getAllInstruments() {
  return instruments;
}

vector<Instrument*> Portfolio::getProfitableInstruments() {
  vector<Instrument*> result;
  for (auto i : instruments) {
    if (i->computeProfit() > 0) {
      result.push_back(i);
    }
  }
  return result;
}

void Portfolio::writeToFile(string filename, double maxProfit) {
  ofstream fout(filename);
  vector<Instrument*> result = getAllInstruments();
  sort(result.begin(), result.end(), [](Instrument* i1, Instrument* i2) {
    return i1->computeProfit() < i2->computeProfit();
  });
  for (auto i : result) {
    if (i->computeProfit() < maxProfit)
      fout << i->toString() << ", Profit: $" << i->computeProfit() << "\n";
  }
}
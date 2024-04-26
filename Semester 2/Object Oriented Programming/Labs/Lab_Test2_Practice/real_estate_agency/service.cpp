#include "service.h"
#include <fstream>

Dweling Service::addDweling(double price, bool isProfitable) {
  Dweling d{price, isProfitable};
  dwelings.push_back(d);
  return d;
}

void Service::removeClient(string name) {
  int index = 0;
  for (int i = 0; i < clients.size(); i++) {
    if (clients.at(i)->getName() == name) {
      index = i;
      break;
    }
  }
  clients.erase(clients.begin() + index);
}

vector<Client*> Service::getInterestedClients(Dweling d) {
  vector<Client*> result;
  for (auto c : clients) {
    if (c->isInterested(d))
      result.push_back(c);
  }
  return result;
}

void Service::writeToFile(string filename) {
  ofstream fout(filename);
  fout << "---Dwelings---\n";
  for (auto d : dwelings) {
    fout << d.toString() << "\n";
  }
  fout << "\n--Clients---\n";
  for (auto c : clients) {
    fout << c->toString() << "\n";
  }
  fout.close();
}
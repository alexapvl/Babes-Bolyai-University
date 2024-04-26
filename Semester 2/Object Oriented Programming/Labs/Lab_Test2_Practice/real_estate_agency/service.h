#pragma once
#include "domain.h"
#include <vector>
using namespace std;

class Service {
private:
  vector<Dweling> dwelings;
  vector<Client*> clients;

public:
  Service() {
    Client* c1 = new Person("Alex", 100);
    Client* c2 = new Person("Cristi", 100000);
    Client* c3 = new Company("Robite", 2000000, 5000);
    Client* c4 = new Company("Grandemar", 500, 10000);
    Dweling d1(50, true);
    dwelings.push_back(d1);
    clients.push_back(c1);
    clients.push_back(c2);
    clients.push_back(c3);
    clients.push_back(c4);
  };
  ~Service(){};
  Dweling addDweling(double price, bool isProfitable);
  void removeClient(string name);
  vector<Client*> getClients() { return clients; };
  vector<Dweling> getDwelings() { return dwelings; };
  vector<Client*> getInterestedClients(Dweling d);
  void writeToFile(string filename);
};
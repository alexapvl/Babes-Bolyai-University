#include "ui.h"
#include <iostream>
using namespace std;

void UI::start() {
  int choice;
  while (true) {
    cout << "1.Remove client\n";
    cout << "2.Show clients\n";
    cout << "3.Add dweling\n";
    cout << "4.Save to file\n";
    cout << "0.Exit\n";
    cout << ">>> ";
    cin >> choice;
    if (choice == 1) {
      string name;
      cout << "Name: ";
      cin >> name;
      s.removeClient(name);
    } else if (choice == 2) {
      vector<Dweling> dwelings = s.getDwelings();
      cout << "---Dwelings---\n";
      for (auto d : dwelings) {
        cout << d.toString() << "\n";
      }
      vector<Client*> clients = s.getClients();
      cout << "\n--Clients---\n";
      for (auto c : clients) {
        cout << c->toString() << "\n";
      }
    } else if (choice == 3) {
      double price;
      string profitable;
      cout << "Price: ";
      cin >> price;
      cout << "Profitable(true | false): ";
      cin >> profitable;
      Dweling d(0.0, false);
      if (profitable == "true") {
        Dweling d = s.addDweling(price, true);
      } else {
        Dweling d = s.addDweling(price, false);
      }
      vector<Client*> interestedClients = s.getInterestedClients(d);
      cout << "Interested clients: \n";
      for (auto c : interestedClients) {
        cout << c->toString() << "\n";
      }
    } else if (choice == 4) {
      s.writeToFile(filename);
    } else if (choice == 0) {
      break;
    } else {
      cout << "Invalid input\n";
    }
  }
}
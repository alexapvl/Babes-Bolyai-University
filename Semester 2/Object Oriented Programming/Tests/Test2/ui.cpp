#include "ui.h"

void UI::start() {
  int choice = 0;
  while (true) {
    cout << "1.Add instrument\n";
    cout << "2.Show all instruments\n";
    cout << "3.Show only profitable instruments\n";
    cout << "4.Write instruments under a certain profit to the file\n";
    cout << "0.Exit\n";
    cout << ">>> ";
    cin >> choice;
    if (choice == 1) {
      string type, name, optionType;
      double buyPrice, sellPrice, shares, strikePrice;
      cout << "Type(Stock | Option): ";
      cin >> type;
      cout << "Name: ";
      cin >> name;
      cout << "Buy Price: ";
      cin >> buyPrice;
      cout << "Sell Price: ";
      cin >> sellPrice;
      if (type == "Stock") {
        cout << "Shares: ";
        cin >> shares;
        Instrument* i = new Stock(name, buyPrice, sellPrice, shares);
        p.addInstrument(i);
      } else { // Option
        cout << "Strike Price: ";
        cin >> strikePrice;
        cout << "Option Type(CALL | PUT): ";
        cin >> optionType;
        Instrument* i = new Option(name, buyPrice, sellPrice, strikePrice, optionType);
        p.addInstrument(i);
      }
    } else if (choice == 2) {
      vector<Instrument*> result = p.getAllInstruments();
      for (auto i : result) {
        cout << i->toString() << ", Profit: $" << i->computeProfit() << "\n";
      }
    } else if (choice == 3) {
      vector<Instrument*> result = p.getProfitableInstruments();
      for (auto i : result) {
        cout << i->toString() << "\n";
      }
    } else if (choice == 4) {
      double maxProfit;
      cout << "Max profit: ";
      cin >> maxProfit;
      p.writeToFile(filename, maxProfit);
    } else if (choice == 0) {
      break;
    } else {
      cout << "Invalid input\n";
    }
  }
}
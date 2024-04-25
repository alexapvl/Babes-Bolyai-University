#include "ui.h"
#include <iostream>

using namespace std;

void UI::start() {
  int choice;
  while (true) {
    printMenu();
    cout << "\n>>> ";
    cin >> choice;
    if (choice == 1) {
      addDepartment();
    } else if (choice == 2) {
      showAllDepartments();
    } else if (choice == 3) {
      showAllEfficientDepartments();
    } else if (choice == 4) {
      writeToFile();
    } else if (choice == 0) {
      break;
    } else {
      cout << "\nInvalid input";
    }
  }
}
void UI::printMenu() {
  cout << "1.Add department\n";
  cout << "2.Show all departments\n";
  cout << "3.Show all efficient departments\n";
  cout << "4.Save to file\n";
  cout << "0.Exit\n";
}
void UI::addDepartment() {
  string type, name;
  cout << "Type of department(Surgery | Neonatal): ";
  cin >> type;
  cout << "Name: ";
  cin >> name;
  int noDoc;
  cout << "\nNumber of doctors: ";
  cin >> noDoc;
  if (type == "Surgery") {
    int noPatients;
    cout << "\nNumber of patients: ";
    cin >> noPatients;
    HospitalDepartment* d = new Surgery(name, noDoc, noPatients);
    service.addDepartment(d);
  } else if (type == "Neonatal") {
    int noMothers, noNewborns;
    double avgr;
    cout << "\nNumber of mothers: ";
    cin >> noMothers;
    cout << "\nNumber of newborns: ";
    cin >> noNewborns;
    cout << "\nAverage grade: ";
    cin >> avgr;
    HospitalDepartment* d = new NeonatalUnit(name, noDoc, noMothers, noNewborns, avgr);
    service.addDepartment(d);
  }
}
void UI::showAllDepartments() {
  vector<HospitalDepartment*> d = service.getAllDepartments();
  for (auto dep : d) {
    cout << dep->toString() << "\n";
  }
}
void UI::showAllEfficientDepartments() {
  vector<HospitalDepartment*> d = service.getAllEfficientDepartments();
  for (auto dep : d) {
    cout << dep->toString() << "\n";
  }
}
void UI::writeToFile() {
  service.writeToFile(fileName);
}

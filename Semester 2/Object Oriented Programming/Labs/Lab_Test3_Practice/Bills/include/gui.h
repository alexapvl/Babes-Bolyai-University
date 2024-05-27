#pragma once
#include "service.h"
#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <string>

using namespace std;

class GUI : public QWidget {
private:
  BillService& serv;

  QVBoxLayout* mainWindow;
  QListWidget* listWidget;
  QLineEdit* filter;
  QLineEdit* unpaidBills;
  QPushButton* buttonCalculateUnpaidBills;

  // methods
  void buildGUI();
  void populateBills(const vector<Bill>& b);
  void filterHandler();
  void buttonCalculateUnpaidBillsHandler();

public:
  GUI(BillService& serv);
  ~GUI() {}
};

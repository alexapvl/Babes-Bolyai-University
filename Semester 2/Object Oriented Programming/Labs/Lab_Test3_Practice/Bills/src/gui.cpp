#include "../include/gui.h"

GUI::GUI(BillService& serv) : serv(serv) {
  buildGUI();
  populateBills(serv.getBillsSortedByName());

  QObject::connect(filter, &QLineEdit::editingFinished, this, &GUI::filterHandler);
  QObject::connect(buttonCalculateUnpaidBills, &QAbstractButton::clicked, this, &GUI::buttonCalculateUnpaidBillsHandler);
}

void GUI::buildGUI() {
  mainWindow = new QVBoxLayout{this};

  listWidget = new QListWidget{};
  mainWindow->addWidget(listWidget);

  filter = new QLineEdit{};
  mainWindow->addWidget(filter);

  unpaidBills = new QLineEdit{};
  mainWindow->addWidget(unpaidBills);

  buttonCalculateUnpaidBills = new QPushButton("Calculate unpaid bills based on company name");
  mainWindow->addWidget(buttonCalculateUnpaidBills);
}

void GUI::populateBills(const vector<Bill>& b) {
  listWidget->clear();
  for (auto bill : b) {
    QListWidgetItem* item = new QListWidgetItem{QString::fromStdString(bill.toStr())};
    if (!bill.getIsPaid()) {
      item->setBackground(Qt::red);
    }
    listWidget->addItem(item);
  }
}

void GUI::filterHandler() {
  if (filter->text().toStdString() == "paid") {
    populateBills(serv.getPaidBills());
  } else if (filter->text().toStdString() == "unpaid") {
    populateBills(serv.getUnpaidBills());
  } else if (filter->text().toStdString() == "all") {
    populateBills(serv.getBillsSortedByName());
  } else if (filter->text().toStdString() == "") {
    filter->clear();
    return;
  } else {
    QMessageBox::critical(this, "Error", "usage: paid/unpaid");
  }
  filter->clear();
}

void GUI::buttonCalculateUnpaidBillsHandler() {
  string companyName = unpaidBills->text().toStdString();
  double result = serv.calculateTotal(companyName);
  if (result != -1.0) {
    if (result == 0.0) {
      string s = "There is nothing to be paid to " + companyName;
      QMessageBox::about(this, QString::fromStdString(companyName), QString::fromStdString(s));
    } else {
      string s1 = "Total to be paid to " + companyName + " is: " + to_string(result);
      QMessageBox::about(this, QString::fromStdString(companyName), QString::fromStdString(s1));
    }
  } else {
    QMessageBox::critical(this, "Error", "Company does not exist");
  }
  unpaidBills->clear();
}
#include "service.h"
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <string>

class GUI : public QWidget {
private:
  Service serv;
  QVBoxLayout* mainWindow;
  QListWidget* equations;
  QLineEdit* updateCoef;
  QPushButton* updateCoefButton;
  QPushButton* displaySolutions;

  void buildGUI() {
    mainWindow = new QVBoxLayout{this};

    equations = new QListWidget();
    mainWindow->addWidget(equations);

    updateCoef = new QLineEdit();
    mainWindow->addWidget(updateCoef);

    updateCoefButton = new QPushButton();
    updateCoefButton->setText("Update");
    mainWindow->addWidget(updateCoefButton);

    displaySolutions = new QPushButton();
    displaySolutions->setText("Display solutions");
    mainWindow->addWidget(displaySolutions);
  }

public:
  GUI(Service serv) : serv(serv) {
    buildGUI();
    populateList();

    QObject::connect(updateCoefButton, &QPushButton::clicked, this, &GUI::updateHandler);
    QObject::connect(displaySolutions, &QPushButton::clicked, this, &GUI::solutionsHandler);
  }
  ~GUI() {}

  void populateList() {
    equations->clear();
    for (auto e : serv.getData()) {
      QString s = QString::fromStdString(e.toStr());
      QListWidgetItem* item = new QListWidgetItem(s);
      if (e.hasRealSolutions()) {
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
      }
      equations->addItem(item);
    }
  }

  void updateHandler() {
    int pos = equations->currentRow();
    if (pos == -1)
      return;
    string text = updateCoef->text().toStdString();
    stringstream ss(text);
    string x1, x2, x3;
    getline(ss, x1, ',');
    getline(ss, x2, ',');
    getline(ss, x3);
    double a, b, c;
    a = atof(x1.c_str());
    b = atof(x2.c_str());
    c = atof(x3.c_str());
    serv.updatePosition(pos, a, b, c);
    populateList();
    updateCoef->clear();
  }

  void solutionsHandler() {
    int pos = equations->currentRow();
    if (pos == -1)
      return;
    Equation e = serv.getPosition(pos);
    pair<string, string> result = e.computeSolutions();
    string s = "x1 = " + result.first + " | x2 = " + result.second;

    QMessageBox::about(this, "Solutions", QString::fromStdString(s));
  }
};
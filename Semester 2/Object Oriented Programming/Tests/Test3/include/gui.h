#pragma once
#include "service.h"

#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

using namespace std;

class GUI : public QWidget {
private:
  Service serv;
  QVBoxLayout* mainLayout;
  QListWidget* mealList;
  QLineEdit* maxCalories;

  QVBoxLayout* secondLayout;
  QListWidget* secondList;
  QLineEdit* mealType;
  QLineEdit* startTime;
  QPushButton* showMeals;
  QLineEdit* number;

public:
  GUI(Service serv) : serv(serv) {
    buildGUI();
    populateList();

    QObject::connect(maxCalories, &QLineEdit::textChanged, this, &GUI::filterHandler);
    QObject::connect(showMeals, &QPushButton::clicked, this, &GUI::secondListHandler);
  }
  ~GUI() {}

  void buildGUI() {
    mainLayout = new QVBoxLayout{this};

    mealList = new QListWidget();
    mainLayout->addWidget(mealList);

    maxCalories = new QLineEdit();
    mainLayout->addWidget(maxCalories);

    mealType = new QLineEdit();
    startTime = new QLineEdit();
    mainLayout->addWidget(mealType);
    mainLayout->addWidget(startTime);
    showMeals = new QPushButton();
    showMeals->setText("Show meals and total calories");
    mainLayout->addWidget(showMeals);

    secondLayout = new QVBoxLayout{this};
    secondLayout->addWidget(secondList);
    secondList = new QListWidget();
    number = new QLineEdit();
  }

  void
  populateList(int calories = -1) {
    mealList->clear();
    for (auto meal : serv.getMealsAscendingOrder()) {
      QListWidgetItem* item = new QListWidgetItem();
      item->setText(QString::fromStdString(meal.toStr()));
      if (meal.getCalories() < calories || calories == -1)
        mealList->addItem(item);
    }
  }

  void populateSecondList() {
    secondList->clear();
    string type = mealType->text().toStdString();
    int start = stoi(startTime->text().toStdString());
    vector<Meal> meals = serv.mealsSecondList(type, start);
    if (meals.size() == 0) {
      QMessageBox::about(this, "Error", "No results found");
      return;
    }
    int result = 0;
    for (auto meal : meals) {
      QListWidgetItem* item = new QListWidgetItem();
      item->setText(QString::fromStdString(meal.toStr()));
      result += meal.getCalories();
      number->setText(QString::fromStdString(to_string(result)));
      secondList->addItem(item);
    }
    secondList->show();
    number->show();
  }

  void filterHandler() {
    QString s = maxCalories->text();
    string ss = s.toStdString();
    if (ss.empty())
      populateList();
    else {
      int calories = stoi(ss);
      populateList(calories);
    }
  }

  void secondListHandler() {
    populateSecondList();
  }
};

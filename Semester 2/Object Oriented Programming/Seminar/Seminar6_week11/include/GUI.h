#pragma once

#include "Service.h"
#include <QWidget>

class GUI : public QWidget {
  Q_OBJECT

private:
  Service& service;

public:
  GUI(Service& serv, QWidget* parent = nullptr);
  ~GUI();

private:
  // Ui::GUIClass ui;

  void populateList();
  void addButtonHandler();
  void itemChangedHandler();
  void deleteButtonHandler();
  void filterLineEditHandler();
};

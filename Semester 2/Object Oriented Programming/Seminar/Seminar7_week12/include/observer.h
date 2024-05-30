#pragma once
#include "chatSession.h"
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <string>

class Observer {
public:
  Observer() {}
  virtual ~Observer() {}
  virtual void update() = 0;
};

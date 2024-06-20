#pragma once
#include "user.h"
#include "userSession.h"
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class UserWindow : public Observer, public QWidget {
private:
  User* user;
  UserSession* session;
  QVBoxLayout* layout;
  QListWidget* issuesList;
  QPushButton* solveIssueButton;
  QLineEdit* description;
  QPushButton* addIssueButton;

public:
  UserWindow(User* user, UserSession* session) : user(user), session(session) {
    session->registerObserver(this);
    string tabName = user->getName() + " - " + user->getType();
    setWindowTitle(QString::fromStdString(tabName));
    layout = new QVBoxLayout(this);
    issuesList = new QListWidget(this);
    layout->addWidget(issuesList);
    if (user->getType() == "programmer") { // programmer
      solveIssueButton = new QPushButton("Solve Issue", this);
      layout->addWidget(solveIssueButton);
      QPushButton::connect(solveIssueButton, &QPushButton::clicked, this, &UserWindow::solveIssueHandler);
    } else { // tester
      description = new QLineEdit(this);
      layout->addWidget(description);
      addIssueButton = new QPushButton("Add Issue", this);
      layout->addWidget(addIssueButton);
      QPushButton::connect(addIssueButton, &QPushButton::clicked, this, &UserWindow::addIssueHandler);
    }
    update();
    this->show();
  }

  void update() override {
    issuesList->clear();
    for (auto i : session->getIssues()) {
      issuesList->addItem(QString::fromStdString(i->toStr()));
    }
  }

  void solveIssueHandler() {
    int index = issuesList->currentRow();
    if (!session->solveIssue(index, user)) {
      QMessageBox::critical(this, "Error", "Issue already solved");
    }
  }

  void addIssueHandler() {
    string desc = description->text().toStdString();
    if (!session->addIssue(user, desc)) {
      QMessageBox::critical(this, "Error", "Issue already exists");
    }
  }
};
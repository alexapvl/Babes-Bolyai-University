#pragma once
#include "observer.h"
#include <QWidget>

class ChatWindow : public QWidget, public Observer {
private:
  QWidget* parent;
  QListWidget* chatList;
  QPushButton* sendButton;
  QLineEdit* messageBox;
  ChatSession* chat;
  std::string user;

public:
  ChatWindow(ChatSession* chat, std::string user) : chat(chat), user(user) {
    setWindowTitle(QString::fromStdString(user));
    QVBoxLayout* layout = new QVBoxLayout(this);
    chat->registerObserver(*this);
    chatList = new QListWidget(this);
    layout->addWidget(chatList);
    messageBox = new QLineEdit(this);
    layout->addWidget(messageBox);
    sendButton = new QPushButton("Send", this);
    layout->addWidget(sendButton);
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
  }
  void update() override {
    chatList->clear();
    for (auto m : chat->getMessages()) {
      chatList->addItem(QString::fromStdString(m.first + ": " + m.second));
    }
  }

  void sendMessage() {
    std::string message = messageBox->text().toStdString();
    chat->addMessage(user, message);
  }
};
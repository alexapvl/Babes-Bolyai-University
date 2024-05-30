#pragma once
#include "subject.h"

class ChatSession : public Subject {
private:
  std::vector<std::pair<std::string, std::string>> messages;

public:
  ChatSession() {}
  ~ChatSession() {}
  void addMessage(std::string user, std::string message) {
    messages.push_back(std::pair(user, message));
    notify();
  }

  std::vector<std::pair<std::string, std::string>> getMessages() {
    return messages;
  }
};
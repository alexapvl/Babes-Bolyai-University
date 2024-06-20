#pragma once
#include "service.h"
#include "subject.h"

class UserSession : public Subject {
private:
  Service* service;

public:
  UserSession(Service* service) : service(service) {}
  ~UserSession() { delete service; }
  vector<Issue*> getIssues() { return service->getIssues(); }

  bool addIssue(User* user, string desc) {
    if (service->addIssue(user, desc)) {
      notify();
      return true;
    }
    return false;
  }

  bool solveIssue(int index, User* user) {
    if (service->solveIssue(index, user)) {
      notify();
      return true;
    }
    return false;
  }
};
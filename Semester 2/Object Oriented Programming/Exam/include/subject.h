#pragma once
#include "observer.h"
#include <vector>

using namespace std;

class Subject {
protected:
  vector<Observer*> observers;

public:
  Subject() {}
  virtual ~Subject() {}
  virtual void registerObserver(Observer& obs) {
    observers.push_back(&obs);
  }

  virtual void unregisterObserver(Observer& obs) {
    auto it = std::find(observers.begin(), observers.end(), &obs);
    observers.erase(it);
  }

  virtual void notify() {
    for (auto o : observers) {
      o->update();
    }
  }
};
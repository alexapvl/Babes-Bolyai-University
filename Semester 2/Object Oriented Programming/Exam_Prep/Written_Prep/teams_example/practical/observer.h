#pragma once
class Observer {
public:
  Observer() {}
  virtual ~Observer() {}
  virtual void update() = 0;
};
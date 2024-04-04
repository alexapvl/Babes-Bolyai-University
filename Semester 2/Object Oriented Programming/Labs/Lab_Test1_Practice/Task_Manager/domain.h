#pragma once
#include <iostream>

class Task {
 private:
  std::string description;
  int minutes, priority;

 public:
  Task(std::string description, int minutes, int priority);
  ~Task();

  void setDesc(std::string description);
  void setMin(int minutes);
  void setPriority(int priority);

  std::string getDesc();
  int getMin();
  int getPriority();

  bool operator==(Task other);
  bool operator>(Task other);

  std::string toStr();
};
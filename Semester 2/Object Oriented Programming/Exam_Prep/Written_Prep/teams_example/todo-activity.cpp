#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Activity {
private:
  string description;
  string start_time;

public:
  Activity(string description, string start_time) : description(description), start_time(start_time) {}
  ~Activity() {}
  friend ostream& operator<<(ostream& os, const Activity& a) {
    os << "Activity " << a.description << " will take place at " << a.start_time << endl;
    return os;
  }
};

template <typename T>
class ToDo {
private:
  vector<T> activities;

public:
  ToDo() {}
  ~ToDo() {}

  ToDo& operator+=(T elem) {
    activities.push_back(elem);
    return *this;
  }

  vector<T>::iterator begin() { return activities.begin(); }
  vector<T>::const_iterator begin() const { return activities.begin(); }
  vector<T>::iterator end() { return activities.end(); }
  vector<T>::const_iterator end() const { return activities.end(); }

  void reversePrint(ostream& os) {
    vector<T> rev = activities;
    for (auto it = activities.size(); it > 0; it--) {
      os << activities[it - 1] << endl;
    }
  }
};

void ToDoList() {
  ToDo<Activity> todo;
  Activity tiff("go to TIFF movie", "20:00");
  todo += tiff;
  Activity project("present project assignment", "09.20");
  todo += project;
  // iterates through the activities and prints them as follows:
  // Activity go to TIFF movie will take place at 20.00.
  // Activity present project assignment will take place at 09.20.
  for (auto a : todo)
    std::cout << a << '\n';
  // Prints the activities as follows:
  // Activity present project assignment will take place at 09.20.
  // Activity go to TIFF movie will take place at 20.00.
  todo.reversePrint(std::cout);
}

int main() {
  ToDoList();
  return 0;
}

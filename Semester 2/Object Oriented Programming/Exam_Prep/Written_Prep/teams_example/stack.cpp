#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

template <typename T>
class Stack {
private:
  vector<T> elements;
  int maxCapacity;

public:
  Stack(int maxCapacity) : maxCapacity(maxCapacity) {}
  ~Stack() {}
  int getMaxCapacity() {
    return maxCapacity;
  }

  Stack& operator+(const T& element) { // this function adds an elements to the stack
    if (elements.size() >= maxCapacity) {
      throw runtime_error("Stack is full!");
    }
    elements.push_back(element);
    return *this;
  }

  T pop() {
    T result = elements.back();
    elements.pop_back();
    return result;
  }
};

void testStack() {
  Stack<string> s(2);
  assert(s.getMaxCapacity() == 2);
  try {
    s = s + "examination";
    s = s + "oop";
    s = s + "test";
  } catch (exception& e) {
    assert(strcmp(e.what(), "Stack is full!") == 0);
  }
  assert(s.pop() == "oop");
  assert(s.pop() == "examination");
}

int main() {
  testStack();
  return 0;
}
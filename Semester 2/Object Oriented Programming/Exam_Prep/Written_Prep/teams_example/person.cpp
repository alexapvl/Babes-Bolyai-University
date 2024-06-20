#include <iostream>
class Person {
public:
  Person() { std::cout << "Person{}"; }
  virtual void print() = 0;
  virtual ~Person() { std::cout << "~Person()"; }
};
class Student : public Person {
public:
  Student() { std::cout << "Student{}"; }
  void print() override { std::cout << "Student"; }
  virtual ~Student() { std::cout << "~Student()"; }
};
int main() {
  Person* p = new Person();  // Person{}
  delete p;                  // ~Person()
  Person* s = new Student(); // Person{}Student{} !!!!!!!!!!!!!!!!!!!!!pointer -> calls both constructors
  s->print();                // Student
  delete s;                  // ~Student()~Person()
  return 0;
}

// will print:
// Person{}~Person()Person()Student()Student~Student()~Person()
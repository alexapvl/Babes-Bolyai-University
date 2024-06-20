#include <iostream>

class B {
public:
  B() { std::cout << "B{}"; }
  virtual void print() { std::cout << "B"; }
  virtual ~B() { std::cout << "~B()"; }
};
class D : public B {
public:
  D() { std::cout << "D{}"; }
  void print() override { std::cout << "D"; }
  virtual ~D() { std::cout << "~D()"; }
};

int main() {
  B* b[] = {new B(), new D()}; // B{}B{}D{}
  b[0]->print();               // B
  b[1]->print();               // D
  delete b[0];                 // ~B()
  delete b[1];                 // ~D()~B()
  return 0;
}

// will print:
// B{}B{}D{}BD~B()~D()~B()
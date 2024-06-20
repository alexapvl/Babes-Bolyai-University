#include <iostream>

using namespace std;

class B {
public:
  B() {}
  B(const B&) { cout << "copy "; }
  virtual B f() {
    cout << "B.f ";
    return *this;
  }
  virtual ~B() { cout << "~B "; }
};

class D : public B {
private:
  B* b;

public:
  D(B* _b) : b{_b} { cout << "D "; }
  B f() override {
    cout << "D.f ";
    return b->f();
  }
};

int main() {
  B* b = new B();
  B* d = new D{b};
  d->f();
  delete d;
  delete b;
  return 0;
}
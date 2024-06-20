#include <iostream>

class E {
public:
  E() { std::cout << "E{}"; }
  virtual ~E() { std::cout << "~E()"; }
};

class DE : public E {
public:
  static int n;
  DE() { n++; }
};

int DE::n = 0;

int fct2(int x) {
  if (x < 0) { // 2. enters here
    throw E(); // 3. creates object of type E and then throws exception
    std::cout << "number less than 0" << std::endl;
  } else if (x == 0) {
    throw DE();
    std::cout << "number equal to 0" << std::endl;
  }
  return x % 10;
}

int main() {
  try {
    int res = 0;
    res = fct2(-5); // 1. calls function
    std::cout << DE::n;
    res = fct2(0);
    std::cout << DE::n;
    res = fct2(25);
    std::cout << DE::n;
  } catch (E&) {        // 4. catches exception
    std::cout << DE::n; // prints 0
  } // frees memory, i.e. frees the object E in created at step 3
  return 0;
}

// will print:
// E{}0~E()

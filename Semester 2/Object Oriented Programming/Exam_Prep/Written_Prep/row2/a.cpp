#include <iostream>

using namespace std;

int except(int v) {
  if (v < 0) {
    throw string("1 ");
  } else if (v > 0) {
    throw runtime_error("2 ");
  }
  return 0;
}

int main() {
  try {
    cout << except(10 < 2) << " ";
    cout << except(-2) << " ";
    cout << except(2) << " ";
  } catch (runtime_error& e) {
    cout << e.what();
  } catch (string x) {
    cout << x;
  }
  cout << "Done ";
  return 0;
}
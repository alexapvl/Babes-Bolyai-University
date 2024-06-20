#include <deque>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> v{10, 9, 8, 7, 6, 5};
  vector<int>::iterator it = v.begin() + 2;
  v.erase(it);
  it = v.begin() + 3;
  *it = 11;
  it++;
  *it = 1;
  deque<int> x;
  copy_if(v.begin(), v.end(), front_inserter(x), [](int a) { return a % 2 == 1; });
  for (auto a : x) {
    cout << a << " ";
  }
  return 0;
}
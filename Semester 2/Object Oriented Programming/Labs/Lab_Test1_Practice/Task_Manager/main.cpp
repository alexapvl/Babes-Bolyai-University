#include "tests.h"
#include "ui.h"

int main() {
  Test::testAll();
  UI ui = UI();
  ui.start();
  return 0;
}
#include "tests.h"
#include "ui.h"

int main() {
  Test::testAll();
  UI ui;
  ui.start();
  return 0;
}
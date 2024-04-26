#include "ui.h"

int main() {
  Portfolio p;
  UI ui(p, "file.txt");
  ui.start();
  return 0;
}
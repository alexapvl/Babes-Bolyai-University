#include "ui.h"

int main() {
  Service s;
  UI ui(s, "file.txt");
  ui.start();
  return 0;
}
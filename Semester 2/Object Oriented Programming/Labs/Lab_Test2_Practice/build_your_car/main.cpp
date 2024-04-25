#include "ui.h"

int main() {
  Service service;
  UI ui(service, "file.txt");
  ui.start();
  return 0;
}
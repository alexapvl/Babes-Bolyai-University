#include "include/gui.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  BillService service;
  GUI gui(service);
  gui.show();

  return app.exec();
}
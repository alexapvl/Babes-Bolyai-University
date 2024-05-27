// Local movie database
#include "../include/gui/gui.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  Service service;
  GUI gui = GUI(service);
  gui.setWindowTitle("Movie Database");
  gui.show();
  return app.exec();
}
#include "../include/gui.h"
#include <QApplication>

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  Repo repo;
  Service service(repo);
  GUI gui(service);
  gui.show();
  return app.exec();
}
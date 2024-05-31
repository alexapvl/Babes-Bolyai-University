#include "../include/gui.h"
#include <QApplication>

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  Repo repo;
  Service serv(repo);
  GUI gui(serv);

  gui.show();

  return app.exec();
}
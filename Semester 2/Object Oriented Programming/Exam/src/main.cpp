#include "../include/doctorWindow.h"
#include <QApplication>

using namespace std;

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  Repository* repo = new Repository();
  Service* serv = new Service(repo);
  DoctorSession session(serv);
  for (auto d : serv->getDoctors()) {
    DoctorWindow* doc = new DoctorWindow(d, &session);
  }

  return app.exec();
}
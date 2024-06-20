#include "../userWindow.h"
#include <QApplication>

using namespace std;

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  Repository* repo = new Repository();
  Service* serv = new Service(repo);
  UserSession session(serv);
  User* u1 = new User("Alex", "programmer");
  UserWindow* w1 = new UserWindow(u1, &session);
  User* u2 = new User("Robert", "programmer");
  UserWindow* w2 = new UserWindow(u2, &session);
  User* u3 = new User("Katia", "tester");
  UserWindow* w3 = new UserWindow(u3, &session);
  User* u4 = new User("Alexia", "tester");
  UserWindow* w4 = new UserWindow(u4, &session);

  return app.exec();
}
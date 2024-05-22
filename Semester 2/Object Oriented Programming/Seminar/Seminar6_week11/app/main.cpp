#include "../include/CSVPlaylist.h"
#include "../include/GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  Repository repo("Songs.txt");
  FilePlaylist* p = new CSVPlaylist{};
  Service serv(repo, p, SongValidator{});
  GUI gui{serv};
  gui.show();

  return a.exec();
}

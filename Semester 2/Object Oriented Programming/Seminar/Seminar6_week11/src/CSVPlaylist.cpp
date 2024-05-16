#include "../include/CSVPlaylist.h"
#include "../include/RepositoryExceptions.h"
#include <fstream>

using namespace std;

void CSVPlaylist::writeToFile() {
  ofstream f(this->filename);

  if (!f.is_open())
    throw FileException("The file could not be opened!");

  for (auto s : this->songs)
    f << s;

  f.close();
}

void CSVPlaylist::displayPlaylist() const {
  string aux = "open ../" + this->filename; // if the path contains spaces, we must put it inside quotations
  system(aux.c_str());
}

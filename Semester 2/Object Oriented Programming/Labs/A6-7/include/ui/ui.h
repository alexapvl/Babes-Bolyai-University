#pragma once
#include <iostream>
#include <string>

#include "../service/service.h"

namespace AdminUI {
const int addMovie = 1;
const int removeMovie = 2;
const int updateMovie = 3;
const int printMovies = 4;
const int exit = 0;
} // namespace AdminUI

namespace UserUI {
const int browseMovies = 1;
const int removeMovieFromWatchList = 2;
const int printWatchList = 3;
const int openWatchListFile = 4;
const int exit = 0;
} // namespace UserUI

class UI {
private:
  Service service;

public:
  UI(const int& choice);
  ~UI();
  // UI Specific Methods
  void runApp();
  void printUserFileTypeMenu();
  void printInitialMenu();
  static void readInt(int& value);
  static void readChar(char& c);
  void openFile(std::string file) const;

  // Admin Interface
  void printAdminMenu();
  void printAllMovies();
  void adminAddMovie();
  void adminRemoveMovie();
  void adminUpdateMovie();

  // User Interface
  void printUserMenu();
  void userBrowseMovies();
  void userRemoveMovieFromWatchList();
  void userPrintWatchList();
  void userOpenWatchListFile();
};

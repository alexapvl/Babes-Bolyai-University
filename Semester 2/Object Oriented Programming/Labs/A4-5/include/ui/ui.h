#pragma once
#include <iostream>
#include <string>

#include "../service/service.h"

class UI {
 private:
  Service service;

 public:
  UI();
  ~UI();
  // UI Specific Methods
  void runApp();
  static void readInt(int& value);
  static void readChar(char& c);

  // Admin Interface
  void printInitialMenu();
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
};

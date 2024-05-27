#pragma once
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <string>

#include "../domain/movie.h"
#include "../exceptions/repoExceptions.h"
#include "../repository/fileRepository.h"

class Service {
private:
  Repository* repository;

public:
  Service();
  ~Service();
  bool addMovie(Movie movieToBeAdded);
  bool addMovieToWatchList(int index);
  bool removeMovieByPosition(int positionOfMovie);
  bool removeMovieByPositionWatchList(int positionOfMovie);
  bool updateMovie(int position, Movie& movie);
  bool validatePositionWatchList(int index);
  std::vector<Movie> getMovies() const;
  std::vector<int> getWatchList() const;
  int getMoviesCount() const;
  int getWatchListCount() const;
  int returnPosition(Movie& movie);
  Movie getMovieByPosition(int position) const;
  std::vector<int> getMoviesByGenre(const std::string& genre);
  std::vector<Movie> getMoviesSortedByNumberOfLikes();
  void incrementLikes(int index);
  void openMovieTrailer(int index);
  void generateRandomMovies();
  void removeAllMovies();
  void setCSV();
  void setHTML();
  std::string getFileNameWatchList();
  void saveWatchList();
};

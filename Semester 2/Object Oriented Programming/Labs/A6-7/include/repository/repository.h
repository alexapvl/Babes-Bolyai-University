#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "../domain/movie.h"
#include "../exceptions/repoExceptions.h"

class Repository {
protected:
  std::vector<Movie> movies;
  std::vector<int> watchList;

public:
  Repository();
  virtual ~Repository();
  void addMovie(const Movie& movieToBeAdded);
  void addMovieToWatchList(const int& index);
  void removeMovieByPosition(int positionOfMovieToBeRemoved);
  void removeMovieByPositionWatchList(int positionOfMovieToBeRemoved);
  void updateMovie(int positionOfMovieToBeUpdated, const Movie& movieWithUpdateData);
  bool validatePosition(int position) const;
  bool validatePositionWatchList(int position) const;
  std::vector<Movie> getMovies() const;
  std::vector<int> getWatchList() const;
  int getMoviesCount() const;
  int getWatchListCount() const;
  Movie getMovieByPosition(int position) const;
  int returnPosition(Movie& movie);
  int returnPositionWatchList(int& index);
  void modifyNumberOfLikesOfMovie(int position);
  std::vector<int> getMoviesByGenre(const std::string& genre);

  // Define the following methods as virtual to allow the use of polymorphism
  // Movies File Repository
  virtual void loadMoviesIntoRepository() {};
  virtual void saveMovies() {};

  // Watch List File Repository
  virtual void saveWatchList() {};
  virtual void clearWatchListFile() {};
  virtual void setCSV() {};
  virtual void setHTML() {};
  virtual std::string getFileNameWatchList() = 0;
};

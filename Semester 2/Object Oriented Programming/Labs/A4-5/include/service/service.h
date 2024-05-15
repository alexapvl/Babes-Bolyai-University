#pragma once
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <string>

#include "../domain/dynamicVector.h"
#include "../domain/movie.h"
#include "../repository/repository.h"

class Service {
 private:
  Repository repository;

 public:
  Service();
  ~Service();
  bool addMovie(Movie movieToBeAdded);
  bool addMovieToWatchList(int index);
  bool removeMovieByPosition(int positionOfMovie);
  bool removeMovieByPositionWatchList(int positionOfMovie);
  bool updateMovie(int position, Movie movie);
  bool validatePositionWatchList(int index);
  DynamicVector<Movie> getMovies() const;
  DynamicVector<int> getWatchList() const;
  int getMoviesCount() const;
  Movie getMovieByPosition(int position) const;
  DynamicVector<int> getMoviesByGenre(const std::string& genre);
  void incrementLikes(int index);
  void openMovieTrailer(int index);
  void generateRandomMovies();
  void removeAllMovies();
};

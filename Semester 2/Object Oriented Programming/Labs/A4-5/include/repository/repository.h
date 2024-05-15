#pragma once
#include <iostream>
#include <string>

#include "../domain/dynamicVector.h"
#include "../domain/movie.h"

class Repository {
 private:
  DynamicVector<Movie> movies;
  DynamicVector<int> watchList;

 public:
  Repository();
  ~Repository();
  void addMovie(const Movie& movieToBeAdded);
  void addMovieToWatchList(const int& index);
  void removeMovieByPosition(int positionOfMovieToBeRemoved);
  void removeMovieByPositionWatchList(int positionOfMovieToBeRemoved);
  void updateMovie(int positionOfMovieToBeUpdated,
                   const Movie& movieWithUpdateData);
  bool validatePosition(int position) const;
  bool validatePositionWatchList(int position) const;
  DynamicVector<Movie> getMovies() const;
  DynamicVector<int> getWatchList() const;
  int getMoviesCount() const;
  Movie getMovieByPosition(int position) const;
  int returnPosition(Movie& movie);
  int returnPositionWatchList(int& index);
  void modifyNumberOfLikesOfMovie(int position);
  DynamicVector<int> getMoviesByGenre(const std::string& genre);
};

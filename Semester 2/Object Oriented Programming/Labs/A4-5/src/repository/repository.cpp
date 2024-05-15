#include "../../include/repository/repository.h"

// Constructor
Repository::Repository() {
  this->movies = DynamicVector<Movie>();
  this->watchList = DynamicVector<int>();
}

// Destructor
Repository::~Repository() {
  /*
  nothing to do here, the destructor of
  the dynamic vector will be called automatically
  */
}

void Repository::addMovie(const Movie& movieToBeAdded) {
  /*
  params: movieToBeAdded - the movie to be added in the repository
  Adds a movie in the repository
  */
  this->movies.addElementInDynamicVector(movieToBeAdded);
}

void Repository::addMovieToWatchList(const int& index) {
  /*
  params: index - the index of the movie to be added in the watch list
  Adds a movie in the watch list
  */
  this->watchList.addElementInDynamicVector(index);
}

void Repository::removeMovieByPosition(int positionOfMovieToBeRemoved) {
  /*
  params: positionOfMovieToBeRemoved - the position of the movie to be removed
  Removes a movie from the repository by the position of the movie
  */
  this->movies.removeElementFromDynamicVector(positionOfMovieToBeRemoved);
}

void Repository::removeMovieByPositionWatchList(
    int positionOfMovieToBeRemoved) {
  /*
  params: positionOfMovieToBeRemoved - the position of the movie to be removed
  Removes a movie from the watch list by the position of the movie
  */
  this->watchList.removeElementFromDynamicVector(positionOfMovieToBeRemoved);
}

void Repository::updateMovie(int positionOfMovieToBeUpdated,
                             const Movie& movieWithUpdateData) {
  /*
  params: positionOfMovieToBeUpdated - the position of the movie to be updated
          movieWithUpdateData - the movie with the new data
  Updates a movie from the repository by the position of the movie
  */
  this->movies.updateElementInDynamicVector(positionOfMovieToBeUpdated,
                                            movieWithUpdateData);
}

bool Repository::validatePosition(int position) const {
  /*
  params: position - the position to be validated
  Returns true if the position is valid in the repo, false otherwise
  */
  return position >= 0 && position < this->getMovies().getSize();
}

bool Repository::validatePositionWatchList(int position) const {
  /*
  params: position - the position to be validated
  Returns true if the position is valid in the watch list, false otherwise
  */
  return position >= 0 && position < this->getWatchList().getSize();
}

DynamicVector<Movie> Repository::getMovies() const {
  /*
  Returns the movies from the repository
  */
  return this->movies;
}

DynamicVector<int> Repository::getWatchList() const {
  /*
  Returns the watch list from the repository
  */
  return this->watchList;
}

int Repository::getMoviesCount() const {
  /*
  Returns the number of movies from the repository
  */
  return this->getMovies().getSize();
}

Movie Repository::getMovieByPosition(int position) const {
  /*
  params: position - the position of the movie to be returned
  Returns the movie from the repository by the position
  */
  return this->movies.getElement(position);
}

int Repository::returnPosition(Movie& movie) {
  /*
  params: movie - the movie to be searched for
  Returns the position of the movie in the repository
  */
  int position = this->movies.returnPosition(movie);
  return position;
}

int Repository::returnPositionWatchList(int& index) {
  /*
  params: index - the index of the movie to be searched for
  Returns the position of the movie in the watch list
  */
  int position = this->watchList.returnPosition(index);
  return position;
}

void Repository::modifyNumberOfLikesOfMovie(int position) {
  /*
  params: position - the position of the movie to be updated
  Increments the number of likes of a movie
  */
  Movie movieToBeUpdated = this->movies.getElement(position);
  movieToBeUpdated.setNumberOfLikes(movieToBeUpdated.getNumberOfLikes() + 1);
  this->movies.updateElementInDynamicVector(position, movieToBeUpdated);
}

DynamicVector<int> Repository::getMoviesByGenre(const std::string& genre) {
  /*
  params: genre - the genre of the movies to be returned
  Returns the indexes of the movies by genre in a dynamic vector
  */
  DynamicVector<int> indexesOfMoviesByGenre = DynamicVector<int>();
  for (int index = 0; index < this->movies.getSize(); ++index)
    if (this->movies.getElement(index).getGenre() == genre || genre == "")
      indexesOfMoviesByGenre.addElementInDynamicVector(index);
  return indexesOfMoviesByGenre;
}

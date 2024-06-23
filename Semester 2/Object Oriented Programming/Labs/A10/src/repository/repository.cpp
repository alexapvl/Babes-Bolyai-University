#include "../../include/repository/repository.h"

// Constructor
Repository::Repository() {
  movies = std::vector<Movie>();
  watchList = std::vector<int>();
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
  movies.push_back(movieToBeAdded);
}

void Repository::addMovieToWatchList(const int& index) {
  /*
  params: index - the index of the movie to be added in the watch list
  Adds a movie in the watch list
  */
  watchList.push_back(index);
}

void Repository::removeMovieByPosition(int positionOfMovieToBeRemoved) {
  /*
  params: positionOfMovieToBeRemoved - the position of the movie to be removed
  Removes a movie from the repository by the position of the movie
  */
  movies.erase(movies.begin() + positionOfMovieToBeRemoved);
}

void Repository::removeMovieByPositionWatchList(int positionOfMovieToBeRemoved) {
  /*
  params: positionOfMovieToBeRemoved - the position of the movie to be removed
  Removes a movie from the watch list by the position of the movie
  */
  watchList.erase(watchList.begin() + positionOfMovieToBeRemoved);
}

void Repository::updateMovie(int positionOfMovieToBeUpdated, const Movie& movieWithUpdateData) {
  /*
  params: positionOfMovieToBeUpdated - the position of the movie to be updated
          movieWithUpdateData - the movie with the new data
  Updates a movie from the repository by the position of the movie
  */
  movies[positionOfMovieToBeUpdated] = movieWithUpdateData;
}

bool Repository::validatePosition(int position) const {
  /*
  params: position - the position to be validated
  Returns true if the position is valid in the repo, false otherwise
  */
  return position >= 0 && position < getMoviesCount();
}

bool Repository::validatePositionWatchList(int position) const {
  /*
  params: position - the position to be validated
  Returns true if the position is valid in the watch list, false otherwise
  */
  return position >= 0 && position < getWatchList().size();
}

std::vector<Movie> Repository::getMovies() const {
  /*
  Returns the movies from the repository
  */
  return movies;
}

std::vector<int> Repository::getWatchList() const {
  /*
  Returns the watch list from the repository
  */
  return watchList;
}

int Repository::getMoviesCount() const {
  /*
  Returns the number of movies from the repository
  */
  return getMovies().size();
}

// get watchlist count
// also modify to not call .size() method for watchlist in any other file
int Repository::getWatchListCount() const {
  /*
  Returns the number of movies from the watch list
  */
  return getWatchList().size();
}

Movie Repository::getMovieByPosition(int position) const {
  /*
  params: position - the position of the movie to be returned
  Returns the movie from the repository by the position
  */
  if (position < 0 || position >= getMoviesCount())
    throw RepositoryException("The position is invalid!");
  return movies.at(position);
}

int Repository::returnPosition(Movie& movie) {
  /*
  params: movie - the movie to be searched for
  Returns the position of the movie in the repository
  */
  auto it = std::find(movies.begin(), movies.end(), movie);
  if (it != movies.end()) {
    return std::distance(movies.begin(), it); // Calculate index from iterator
  }
  return -1; // Element not found
}

int Repository::returnPositionWatchList(int& index) {
  /*
  params: index - the index of the movie to be searched for
  Returns the position of the movie in the watch list
  */
  auto it = std::find(watchList.begin(), watchList.end(), index);
  if (it != watchList.end()) {
    return std::distance(watchList.begin(), it); // Calculate index from iterator
  }
  return -1; // Element not found
}

void Repository::modifyNumberOfLikesOfMovie(int position) {
  /*
  params: position - the position of the movie to be updated
  Increments the number of likes of a movie
  */
  Movie movieToBeUpdated = movies.at(position);
  MovieValidator::validateNumberOfLikes(movieToBeUpdated.getNumberOfLikes() + 1);
  movieToBeUpdated.setNumberOfLikes(movieToBeUpdated.getNumberOfLikes() + 1);
  updateMovie(position, movieToBeUpdated);
}

std::vector<int> Repository::getMoviesByGenre(const std::string& genre) {
  /*
  params: genre - the genre of the movies to be returned
  Returns the indexes of the movies by genre in a dynamic vector
  */
  std::vector<int> indexesOfMoviesByGenre = std::vector<int>();
  for (Movie movie : getMovies()) {
    if (movie.getGenre() == genre || genre == "")
      indexesOfMoviesByGenre.push_back(returnPosition(movie));
  }
  return indexesOfMoviesByGenre;
}

std::vector<Movie> Repository::getMoviesSortedByNumberOfLikes() {
  std::vector<Movie> result = getMovies();
  std::sort(result.begin(), result.end(), [](Movie& m1, Movie& m2) { return m1.getNumberOfLikes() < m2.getNumberOfLikes(); });
  return result;
}

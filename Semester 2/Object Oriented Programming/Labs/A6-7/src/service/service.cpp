#include "../../include/service/service.h"

// Constructor
Service::Service(const int& choice) {
  if (choice == repoType::CSV) {
    this->repository = new CSVRepository("movies.csv", "watchlist.csv", repoType::CSV);
  } else if (choice == repoType::HTML) {
    this->repository = new HTMLRepository("movies.html", "watchlist.html", repoType::HTML);
  } else {
    throw FileException("Invalid repo type");
  }
}

// Destructor
Service::~Service() {
  delete this->repository;
}

bool Service::addMovie(Movie movieToBeAdded) {
  /*
  params: movieToBeAdded - the movie to be added in the repository
  Adds a movie in the repository if it does not already exist
  Returns true if the movie was added, false otherwise
  */
  if (this->repository->validatePosition(
          this->repository->returnPosition(movieToBeAdded)))
    return false;

  this->repository->addMovie(movieToBeAdded);
  return true;
}

bool Service::addMovieToWatchList(int index) {
  /*
  params: index - the index of the movie to be added in the watch list
  Adds a movie in the watch list if it does not already exist
  Returns true if the movie was added, false otherwise
  */
  if (this->repository->validatePositionWatchList(
          this->repository->returnPositionWatchList(index)))
    return false;

  this->repository->addMovieToWatchList(index);
  return true;
}

bool Service::removeMovieByPosition(int positionOfMovie) {
  /*
  params: positionOfMovie - the position of the movie to be removed
  Removes a movie from the repository by the position of the movie
  Returns true if the movie was removed, false otherwise
  */
  if (!this->repository->validatePosition(positionOfMovie))
    return false;

  this->repository->removeMovieByPosition(positionOfMovie);
  return true;
}

bool Service::removeMovieByPositionWatchList(int positionOfMovie) {
  /*
  params: positionOfMovie - the position of the movie to be removed
  Removes a movie from the watch list by the position of the movie
  Returns true if the movie was removed, false otherwise
  */
  if (!this->repository->validatePositionWatchList(positionOfMovie))
    return false;

  this->repository->removeMovieByPositionWatchList(positionOfMovie);
  return true;
}

bool Service::updateMovie(int position, Movie movie) {
  /*
  params: position - the position of the movie to be updated
          movie - the movie with the new data
  Updates a movie from the repository by the position of the movie
  Returns true if the movie was updated, false otherwise
  */
  if (!this->repository->validatePosition(position))
    return false;

  this->repository->updateMovie(position, movie);
  return true;
}

bool Service::validatePositionWatchList(int index) {
  /*
  params: index - the index to be validated
  Returns true if the index is valid in the watch list, false otherwise
  */
  return this->repository->validatePositionWatchList(index);
}

std::vector<Movie> Service::getMovies() const {
  /*
  Returns all the movies from the repository
  */
  return this->repository->getMovies();
}

std::vector<int> Service::getWatchList() const {
  /*
  Returns all the movies from the watch list
  */
  return this->repository->getWatchList();
}

int Service::getMoviesCount() const {
  /*
  Returns the number of movies from the repository
  */
  return this->repository->getMoviesCount();
}

int Service::getWatchListCount() const {
  /*
  Returns the number of movies in the watch list
  */
  return this->repository->getWatchListCount();
}

Movie Service::getMovieByPosition(int position) const {
  /*
  params: position - the position of the movie to be returned
  Returns the movie from the repository by the position
  */
  return this->repository->getMovieByPosition(position);
}

std::vector<int> Service::getMoviesByGenre(const std::string& genre) {
  /*
  params: genre - the genre of the movies to be returned
  Returns all the movies from the repository by the genre
  */
  return this->repository->getMoviesByGenre(genre);
}

void Service::incrementLikes(int index) {
  /*
  params: index - the index of the movie to be incremented
  Increments the number of likes of a movie
  */
  getMovieByPosition(index); // used only for for validation of movie
  this->repository->modifyNumberOfLikesOfMovie(index);
}

void Service::openMovieTrailer(int index) {
  /*
  params: index - the index of the movie to be opened
  Opens the trailer of a movie in the default browser
  */
  Movie movie = this->getMovieByPosition(index);
  std::string trailer = movie.getTrailer();
  std::string command = "open " + trailer;
  system(command.c_str());
}

void Service::generateRandomMovies() {
  /*
  Generates 15 random movies and adds them to the repository
  */
  Movie entryMovie1 = Movie("The Godfather", "Crime", 1972, rand() % 100 + 1, "https://www.youtube.com/watch?v=sY1S34973zA");
  Movie entryMovie2 = Movie("Schindler's List", "Biography", 1993, rand() % 100 + 1, "https://www.youtube.com/watch?v=gG22XNhtnoY");
  Movie entryMovie3 = Movie("2012", "Action", 2009, rand() % 100 + 1, "https://www.youtube.com/watch?v=sFXGrTng0gQ");
  Movie entryMovie4 = Movie("The Shawshank Redemption", "Drama", 1994, rand() % 100 + 1, "https://www.youtube.com/watch?v=6hB3S9bIaco");
  Movie entryMovie5 = Movie("The Dark Knight", "Action", 2008, rand() % 100 + 1, "https://www.youtube.com/watch?v=EXeTwQWrcwY");
  Movie entryMovie6 = Movie("The Lord of the Rings: The Return of the King", "Adventure", 2003, rand() % 100 + 1, "https://www.youtube.com/watch?v=r5X-hFf6Bwo");
  Movie entryMovie7 = Movie("The Godfather: Part II", "Crime", 1974, rand() % 100 + 1, "https://www.youtube.com/watch?v=9O1Iy9od7-A");
  Movie entryMovie8 = Movie("The Lord of the Rings: The Fellowship of the Ring", "Adventure", 2001, rand() % 100 + 1, "https://www.youtube.com/watch?v=V75dMMIW2B4");
  Movie entryMovie9 = Movie("Pulp Fiction", "Crime", 1994, rand() % 100 + 1, "https://www.youtube.com/watch?v=s7EdQ4FqbhY");
  Movie entryMovie10 = Movie("The Good, the Bad and the Ugly", "Western", 1966, rand() % 100 + 1, "https://www.youtube.com/watch?v=WCN5JJY_wiA");
  Movie entryMovie11 = Movie("The Lord of the Rings: The Two Towers", "Adventure", 2002, rand() % 100 + 1, "https://www.youtube.com/watch?v=LbfMDwc4azU");
  Movie entryMovie12 = Movie("Inception", "Action", 2010, rand() % 100 + 1, "https://www.youtube.com/watch?v=YoHD9XEInc0");
  Movie entryMovie13 = Movie("Fight Club", "Drama", 1999, rand() % 100 + 1, "https://www.youtube.com/watch?v=SUXWAEX2jlg");
  Movie entryMovie14 = Movie("Forrest Gump", "Drama", 1994, rand() % 100 + 1, "https://www.youtube.com/watch?v=bLvqoHBptjg");
  Movie entryMovie15 = Movie("Star Wars: Episode V - The Empire Strikes Back", "Action", 1980, rand() % 100 + 1, "https://www.youtube.com/watch?v=JNwNXF9Y6kY");

  this->addMovie(entryMovie1);
  this->addMovie(entryMovie2);
  this->addMovie(entryMovie3);
  this->addMovie(entryMovie4);
  this->addMovie(entryMovie5);
  this->addMovie(entryMovie6);
  this->addMovie(entryMovie7);
  this->addMovie(entryMovie8);
  this->addMovie(entryMovie9);
  this->addMovie(entryMovie10);
  this->addMovie(entryMovie11);
  this->addMovie(entryMovie12);
  this->addMovie(entryMovie13);
  this->addMovie(entryMovie14);
  this->addMovie(entryMovie15);
}

void Service::removeAllMovies() {
  /*
  Removes all the movies from the repository
  */
  while (this->getMoviesCount() > 0) {
    this->removeMovieByPosition(0);
  }
}

void Service::setCSV() {
  repository->setCSV();
}

void Service::setHTML() {
  repository->setHTML();
}

std::string Service::getFileNameWatchList() {
  return repository->getFileNameWatchList();
}

void Service::saveWatchList() {
  repository->saveWatchList();
}
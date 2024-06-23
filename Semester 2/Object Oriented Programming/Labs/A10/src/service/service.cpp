#include "../../include/service/service.h"

// Constructor
Service::Service() {
  repository = new FileRepository("../movies.txt", "../watchlist.csv", repoType::CSV);
  repository->loadMoviesIntoRepository();
}

// Destructor
Service::~Service() {
  delete repository;
}

bool Service::addMovie(Movie movieToBeAdded) {
  /*
  params: movieToBeAdded - the movie to be added in the repository
  Adds a movie in the repository if it does not already exist
  Returns true if the movie was added, false otherwise
  */
  if (repository->validatePosition(repository->returnPosition(movieToBeAdded)))
    return false;

  Action* action = new ActionAdd(*repository, movieToBeAdded);
  undoActions.push_back(action);
  redoActions.clear();
  repository->addMovie(movieToBeAdded);
  return true;
}

bool Service::addMovieToWatchList(int index) {
  /*
  params: index - the index of the movie to be added in the watch list
  Adds a movie in the watch list if it does not already exist
  Returns true if the movie was added, false otherwise
  */
  if (repository->validatePositionWatchList(repository->returnPositionWatchList(index)))
    return false;

  repository->addMovieToWatchList(index);
  return true;
}

bool Service::removeMovieByPosition(int positionOfMovie) {
  /*
  params: positionOfMovie - the position of the movie to be removed
  Removes a movie from the repository by the position of the movie
  Returns true if the movie was removed, false otherwise
  */
  if (!repository->validatePosition(positionOfMovie))
    return false;

  Action* action = new ActionRemove(*repository, getMovieByPosition(positionOfMovie));
  undoActions.push_back(action);
  redoActions.clear();
  repository->removeMovieByPosition(positionOfMovie);
  return true;
}

bool Service::removeMovieByPositionWatchList(int positionOfMovie) {
  /*
  params: positionOfMovie - the position of the movie to be removed
  Removes a movie from the watch list by the position of the movie
  Returns true if the movie was removed, false otherwise
  */
  if (!repository->validatePositionWatchList(positionOfMovie))
    return false;

  repository->removeMovieByPositionWatchList(positionOfMovie);
  return true;
}

bool Service::updateMovie(int position, Movie& movie) {
  /*
  params: position - the position of the movie to be updated
          movie - the movie with the new data
  Updates a movie from the repository by the position of the movie
  Returns true if the movie was updated, false otherwise
  */
  if (!repository->validatePosition(position))
    return false;

  Action* action = new ActionUpdate(*repository, getMovieByPosition(position), movie);
  undoActions.push_back(action);
  redoActions.clear();
  repository->updateMovie(position, movie);
  return true;
}

bool Service::validatePositionWatchList(int index) {
  /*
  params: index - the index to be validated
  Returns true if the index is valid in the watch list, false otherwise
  */
  return repository->validatePositionWatchList(index);
}

std::vector<Movie> Service::getMovies() const {
  /*
  Returns all the movies from the repository
  */
  return repository->getMovies();
}

std::vector<int> Service::getWatchList() const {
  /*
  Returns all the movies from the watch list
  */
  return repository->getWatchList();
}

int Service::getMoviesCount() const {
  /*
  Returns the number of movies from the repository
  */
  return repository->getMoviesCount();
}

int Service::getWatchListCount() const {
  /*
  Returns the number of movies in the watch list
  */
  return repository->getWatchListCount();
}

int Service::returnPosition(Movie& movie) {
  /*
  params: movie - the movie to be returned
  Returns the position of the movie in the repository
  */
  return repository->returnPosition(movie);
}

Movie Service::getMovieByPosition(int position) const {
  /*
  params: position - the position of the movie to be returned
  Returns the movie from the repository by the position
  */
  return repository->getMovieByPosition(position);
}

std::vector<int> Service::getMoviesByGenre(const std::string& genre) {
  /*
  params: genre - the genre of the movies to be returned
  Returns all the movies from the repository by the genre
  */
  return repository->getMoviesByGenre(genre);
}

std::vector<Movie> Service::getMoviesSortedByNumberOfLikes() {
  return repository->getMoviesSortedByNumberOfLikes();
}

void Service::incrementLikes(int index) {
  /*
  params: index - the index of the movie to be incremented
  Increments the number of likes of a movie
  */
  getMovieByPosition(index); // used only for for validation of movie
  repository->modifyNumberOfLikesOfMovie(index);
}

void Service::openMovieTrailer(int index) {
  /*
  params: index - the index of the movie to be opened
  Opens the trailer of a movie in the default browser
  */
  Movie movie = getMovieByPosition(index);
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

  addMovie(entryMovie1);
  addMovie(entryMovie2);
  addMovie(entryMovie3);
  addMovie(entryMovie4);
  addMovie(entryMovie5);
  addMovie(entryMovie6);
  addMovie(entryMovie7);
  addMovie(entryMovie8);
  addMovie(entryMovie9);
  addMovie(entryMovie10);
  addMovie(entryMovie11);
  addMovie(entryMovie12);
  addMovie(entryMovie13);
  addMovie(entryMovie14);
  addMovie(entryMovie15);
}

void Service::removeAllMovies() {
  /*
  Removes all the movies from the repository
  */
  while (getMoviesCount() > 0) {
    removeMovieByPosition(0);
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

void Service::undo() {
  if (undoActions.empty())
    throw RepositoryException("No more undo actions!");

  Action* action = undoActions.back();
  action->executeUndo();
  redoActions.push_back(action);
  undoActions.pop_back();
}

void Service::redo() {
  if (redoActions.empty())
    throw RepositoryException("No more redo actions!");

  Action* action = redoActions.back();
  action->executeRedo();
  undoActions.push_back(action);
  redoActions.pop_back();
}
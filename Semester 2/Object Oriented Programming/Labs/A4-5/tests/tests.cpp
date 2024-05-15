#include "tests.h"

void testAll() {
  testAdd();
  testRemove();
  testUpdate();
  testGetMovies();
  testValidatePositionWatchList();
  testGetMovieByPosition();
  testGetMoviesByGenre();
  testIncrementLikes();
  testGenerateAndRemoveAllMovies();
  testOpenTrailer();
  testMovie();
}

void testAdd() {
  Service service;
  service.removeAllMovies();
  // Add a movie to the repository
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  assert(service.addMovie(movie1) == true);
  // Try adding the same movie again, should fail
  assert(service.addMovie(movie1) == false);

  // Add to watch list
  assert(service.addMovieToWatchList(0) == true);
  // Try adding to watch list again, should fail
  assert(service.addMovieToWatchList(0) == false);
}

void testRemove() {
  Service service;
  service.removeAllMovies();
  Movie movie("The Godfather", "Crime", 1972, 100,
              "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie);

  // Try removing the movie at position 0
  assert(service.removeMovieByPosition(0) == true);
  // Try removing again, should fail
  assert(service.removeMovieByPosition(0) == false);

  // Try removing from the watch list
  assert(service.addMovieToWatchList(0) == true);
  assert(service.removeMovieByPositionWatchList(0) == true);
  // Try removing again, should fail
  assert(service.removeMovieByPositionWatchList(0) == false);
}

void testUpdate() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);

  // Update the movie at position 0
  Movie movie2("Inception", "Sci-Fi", 2010, 148,
               "https://www.youtube.com/watch?v=YoHD9XEInc0");
  assert(service.updateMovie(0, movie2) == true);
  // Try updating a non-existent movie, should fail
  assert(service.updateMovie(1, movie2) == false);
}

void testGetMovies() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);
  Movie movie2("Inception", "Sci-Fi", 2010, 148,
               "https://www.youtube.com/watch?v=YoHD9XEInc0");
  service.addMovie(movie2);

  DynamicVector<Movie> movies = service.getMovies();
  assert(service.getMoviesCount() == 2);

  // Now for the watch list
  service.addMovieToWatchList(0);
  service.addMovieToWatchList(1);
  DynamicVector<int> watchList = service.getWatchList();
  assert(watchList.getSize() == 2);
}

void testValidatePositionWatchList() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);
  service.addMovieToWatchList(0);

  assert(service.validatePositionWatchList(0) == true);
  assert(service.validatePositionWatchList(1) == false);
}

void testGetMovieByPosition() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);
  Movie movie2("Inception", "Sci-Fi", 2010, 148,
               "https://www.youtube.com/watch?v=YoHD9XEInc0");
  service.addMovie(movie2);

  Movie movie = service.getMovieByPosition(0);
  assert(movie.getTitle() == "The Godfather");
  movie = service.getMovieByPosition(1);
  assert(movie.getTitle() == "Inception");

  try {
    service.getMovieByPosition(2);
    assert(false);
  } catch (std::runtime_error& error) {
    assert(true);
  }
}

void testGetMoviesByGenre() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);
  Movie movie2("Inception", "Sci-Fi", 2010, 148,
               "https://www.youtube.com/watch?v=YoHD9XEInc0");
  service.addMovie(movie2);

  DynamicVector<int> movies = service.getMoviesByGenre("Crime");
  assert(movies.getSize() == 1);
  movies = service.getMoviesByGenre("Sci-Fi");
  assert(movies.getSize() == 1);
  movies = service.getMoviesByGenre("Comedy");
  assert(movies.getSize() == 0);
}

void testIncrementLikes() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);

  service.incrementLikes(0);
  Movie movie = service.getMovieByPosition(0);
  assert(movie.getNumberOfLikes() == 101);

  try {
    service.incrementLikes(1);
    assert(false);
  } catch (std::runtime_error& error) {
    assert(true);
  }
}

void testGenerateAndRemoveAllMovies() {
  Service service;
  service.removeAllMovies();
  assert(service.getMoviesCount() == 0);
  service.generateRandomMovies();
  assert(service.getMoviesCount() == 15);
}

void testOpenTrailer() {
  Service service;
  service.removeAllMovies();
  Movie movie1("The Godfather", "Crime", 1972, 100,
               "https://www.youtube.com/watch?v=sY1S34973zA");
  service.addMovie(movie1);
  service.openMovieTrailer(0);
  // The trailer should be opened in the default browser
}

void testMovie() {
  Movie movie = Movie("The Godfather", "Crime", 1972, 100,
                      "https://www.youtube.com/watch?v=sY1S34973zA");
  movie = movie;
  movie.setTitle("Inception");
  assert(movie.getTitle() == "Inception");
  movie.setGenre("Sci-Fi");
  assert(movie.getGenre() == "Sci-Fi");
  movie.setYearOfRelease(2010);
  assert(movie.getYearOfRelease() == 2010);
  try {
    movie.setYearOfRelease(-1);
    assert(false);
  } catch (std::invalid_argument& error) {
    assert(true);
  }
  movie.setTrailer("https://www.youtube.com/watch?v=YoHD9XEInc0");
  assert(movie.getTrailer() == "https://www.youtube.com/watch?v=YoHD9XEInc0");
  try {
    movie.setTrailer("invalid");
    assert(false);
  } catch (std::invalid_argument& error) {
    assert(true);
  }
  try {
    movie.setNumberOfLikes(-1);
    assert(false);
  } catch (std::invalid_argument& error) {
    assert(true);
  }
  movie.validateMovie();
  std::string movieString = movie.toStr();
  assert(movieString ==
         "🍿Inception | 🎞️ Sci-Fi | 🗓️ 2010 | 👍100 | "
         "🔗https://www.youtube.com/watch?v=YoHD9XEInc0");
}
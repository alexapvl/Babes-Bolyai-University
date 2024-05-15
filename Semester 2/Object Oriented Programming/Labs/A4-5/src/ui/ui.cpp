#include "../../include/ui/ui.h"

UI::UI() { this->service = Service(); }

UI::~UI() {}

/*
----------UI Specific Methods----------
*/

void UI::runApp() {
  int choice;
  while (true) {
    this->printInitialMenu();
    UI::readInt(choice);
    if (choice == 1) {  // admin menu
      while (true) {
        this->printAdminMenu();
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1)
          this->adminAddMovie();
        else if (choice == 2)
          this->adminRemoveMovie();
        else if (choice == 3)
          this->adminUpdateMovie();
        else if (choice == 4)
          this->printAllMovies();
        else if (choice == 0)
          break;
        else
          std::cout << "Invalid choice! Please try again\n";
      }
    } else if (choice == 2) {  // user menu
      while (true) {
        this->printUserMenu();
        UI::readInt(choice);
        if (choice == 1) {
          this->userBrowseMovies();
        } else if (choice == 2) {
          this->userRemoveMovieFromWatchList();
        } else if (choice == 3) {
          this->userPrintWatchList();
        } else if (choice == 0) {
          break;
        }
      }
    } else if (choice == 0) {
      std::cout << "Thank you for using the app!\n";
      break;
    } else
      std::cout << "Invalid choice! Please try again\n";
  }
}

void UI::printInitialMenu() {
  std::cout << "\nWelcome to the Movie Management App!\n";
  std::cout << "1. Admin\n";
  std::cout << "2. User\n";
  std::cout << "0. Exit\n";
  std::cout << ">>> ";
}

void UI::readInt(int &value) {
  while (true) {
    std::cin >> value;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input! Please try again: ";
    } else {
      std::cin.ignore();
      break;
    }
  }
}

void UI::readChar(char &c) {
  while (true) {
    std::cin >> c;
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid input! Please try again: ";
    } else {
      std::cin.ignore();
      break;
    }
  }
}

/*
----------Admin Interface----------
*/

void UI::printAdminMenu() {
  std::cout << "\nWelcome to the Admin Menu!\n";
  std::cout << "1. Add movie\n";
  std::cout << "2. Remove movie\n";
  std::cout << "3. Update movie\n";
  std::cout << "4. Print all movies\n";
  std::cout << "0. Go back\n";
  std::cout << ">>> ";
}

void UI::printAllMovies() {
  DynamicVector<Movie> movies = this->service.getMovies();
  std::cout << "Number of movies: " << movies.getSize() << "\n";
  for (int index = 0; index < movies.getSize(); ++index) {
    std::cout
        << "==================================================================="
           "=========================================================\n";
    std::cout << index << ". " << movies[index].toStr() << "\n";
  }
}

void UI::adminAddMovie() {
  std::string title, genre, trailer;
  int yearOfRelease, numberOfLikes;
  std::cout << "Enter the title: ";
  getline(std::cin, title);
  std::cout << "Enter the genre: ";
  getline(std::cin, genre);
  std::cout << "Enter the year of release: ";
  UI::readInt(yearOfRelease);
  std::cout << "Enter the number of likes: ";
  UI::readInt(numberOfLikes);
  std::cout << "Enter the trailer: ";
  getline(std::cin, trailer);
  Movie movie = Movie(title, genre, yearOfRelease, numberOfLikes, trailer);

  try {
    movie.validateMovie();
  } catch (std::runtime_error &e) {
    std::cout << e.what() << "\n";
    return;
  }

  if (this->service.addMovie(movie))
    std::cout << "Movie added successfully\n";
  else
    std::cout << "Movie already exists\n";
}

void UI::adminRemoveMovie() {
  this->printAllMovies();
  int positionOfMovie;
  std::cout << "Enter the position of the movie you want to remove: ";
  UI::readInt(positionOfMovie);
  if (this->service.removeMovieByPosition(positionOfMovie))
    std::cout << "Movie removed successfully\n";
  else
    std::cout << "Movie does not exist\n";
}

void UI::adminUpdateMovie() {
  this->printAllMovies();
  int positionOfMovie;
  std::cout << "Please enter the position of the movie you want to update: ";
  UI::readInt(positionOfMovie);
  Movie movieToBeUpdated;
  try {
    movieToBeUpdated = this->service.getMovieByPosition(positionOfMovie);
  } catch (const std::runtime_error &e) {
    std::cout << e.what() << "\n";
    return;
  }
  std::string titleOfMovieToBeUpdated = movieToBeUpdated.getTitle(),
              genreOfMovieToBeUpdated = movieToBeUpdated.getGenre(),
              trailerOfMovieToBeUpdated = movieToBeUpdated.getTrailer(),
              readInput;
  int yearOfRelease = movieToBeUpdated.getYearOfRelease(),
      likesCount = movieToBeUpdated.getNumberOfLikes();
  std::cout << "Please enter the title of the movie. If you do not want to "
               "update this field, just press enter: ";
  std::getline(std::cin, readInput);  // read the newline character
  if (readInput != "") titleOfMovieToBeUpdated = readInput;
  std::cout << "Please enter the genre of the movie. If you do not want to "
               "update this field, just press enter: ";
  std::getline(std::cin, readInput);  // read the newline character
  if (readInput != "") genreOfMovieToBeUpdated = readInput;
  int integerField;
  std::cout << "Please enter the year of release of the movie. If you do not "
               "want to update this field, press 0: ";
  UI::readInt(integerField);
  if (integerField != 0) yearOfRelease = integerField;
  std::cout << "Please enter the likes count of the movie. If you do not want "
               "to update this field, press 0: ";
  UI::readInt(integerField);
  if (integerField != 0) likesCount = integerField;
  std::cout << "Please enter the trailer of the movie: If you do not want to "
               "update this field, just press enter:  ";
  std::getline(std::cin, readInput);  // read the newline character
  if (readInput != "") trailerOfMovieToBeUpdated = readInput;
  Movie updatedMovie =
      Movie(titleOfMovieToBeUpdated, genreOfMovieToBeUpdated, yearOfRelease,
            likesCount, trailerOfMovieToBeUpdated);

  try {
    updatedMovie.validateMovie();
  } catch (const std::runtime_error &e) {
    std::cout << e.what() << "\n";
    return;
  }

  if (this->service.updateMovie(positionOfMovie, updatedMovie))
    std::cout << "Movie updated successfully\n";
  else
    std::cout << "Movie does not exist\n";
}

/*
----------User Interface----------
*/

void UI::printUserMenu() {
  std::cout << "\nWelcome to the User Menu!\n";
  std::cout << "1. See all movies based on a genre\n";
  std::cout << "2. Remove movie from watch list\n";
  std::cout << "3. Show watch list\n";
  std::cout << "0. Go back\n";
  std::cout << ">>> ";
}

void UI::userBrowseMovies() {
  /*
  The user will be able to browse movies and watch trailers in order to add the
  movies to his/her watch list.
  If an already added movie should be added, a message will be displayed
  */
  std::string genre;
  std::cout << "Enter the prefered genre you want to see. Press enter to show "
               "all movies regardless of genre: ";
  std::getline(std::cin, genre);
  DynamicVector<int> indexesOfMoviesByGenre =
      this->service.getMoviesByGenre(genre);

  int index = 0;
  while (true) {
    if (indexesOfMoviesByGenre.getSize() == 0) {
      std::cout << "There are no movies with the given genre\n";
      break;
    }
    std::cout
        << "==================================================================="
           "=========================================================\n";
    std::cout
        << "Title: 🍿"
        << this->service.getMovies()[indexesOfMoviesByGenre[index]].getTitle()
        << "\n"
        << "Genre: 🎞️ "
        << this->service.getMovies()[indexesOfMoviesByGenre[index]].getGenre()
        << "\n"
        << "Year of release: 🗓️ "
        << this->service.getMovies()[indexesOfMoviesByGenre[index]]
               .getYearOfRelease()
        << "\n"
        << "Number of likes: 👍"
        << this->service.getMovies()[indexesOfMoviesByGenre[index]]
               .getNumberOfLikes()
        << "\n";
    this->service.openMovieTrailer(indexesOfMoviesByGenre[index]);
    system("sleep 3");  // to not show the below message too fast
    char addMovie = 'a';
    std::cout << "Do you want to add the movie to your watch list?(y/n) ";
    while (addMovie != 'n' && addMovie != 'y') {
      UI::readChar(addMovie);
    }
    if (addMovie == 'y') {
      // add movie to watch list
      if (this->service.addMovieToWatchList(indexesOfMoviesByGenre[index]))
        std::cout << "Movie added to watch list\n";
      else {
        std::cout << "Movie aready in the watch list\n";
      }
    }
    ++index;
    if (index == indexesOfMoviesByGenre.getSize()) {
      // reset index to go through the selected movies again
      index = 0;
    }
    char next = 'a';
    std::cout << "Do you want to see the next trailer?(y/n) ";
    while (next != 'n' && next != 'y') {
      UI::readChar(next);
    }
    if (next == 'n') break;
  }
}

void UI::userRemoveMovieFromWatchList() {
  this->userPrintWatchList();
  if (this->service.getWatchList().getSize() == 0) return;
  int positionOfMovieInWatchList = -1;
  while (true) {
    std::cout << "Enter the position of the movie you want to remove: ";
    UI::readInt(positionOfMovieInWatchList);
    if (!this->service.validatePositionWatchList(positionOfMovieInWatchList)) {
      std::cout << "Invalid index\n";
    } else {
      break;
    }
  }

  int positionOfMovieInRepository =
      this->service.getWatchList()[positionOfMovieInWatchList];

  char likeMovie = 'a';  // default value to enter the loop
  std::cout << "Did you enjoy watching the movie?(y/n) ";
  while (likeMovie != 'n' && likeMovie != 'y') {
    UI::readChar(likeMovie);
  }
  if (likeMovie == 'y') {
    /* if the user liked the movie, increase number of likes in repo */
    this->service.incrementLikes(positionOfMovieInRepository);
    std::cout << "Movie liked successfully\n";
  }
  // remove the index from the watch list
  this->service.removeMovieByPositionWatchList(positionOfMovieInWatchList);
}

void UI::userPrintWatchList() {
  DynamicVector<int> watchList = this->service.getWatchList();
  std::cout << "Number of movies: " << watchList.getSize() << "\n";
  for (int index = 0; index < watchList.getSize(); ++index) {
    std::cout
        << "==================================================================="
           "=========================================================\n";
    std::cout << index << ". "
              << this->service.getMovies()[watchList[index]].toStr() << "\n";
  }
}

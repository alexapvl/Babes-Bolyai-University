#include "../../include/ui/ui.h"

UI::UI(const int& choice) : service(choice) {}

UI::~UI() {}

/*
----------UI Specific Methods----------
*/

void UI::runApp() {
  int choice;
  while (true) {
    printInitialMenu();
    readInt(choice);
    if (choice == 1) { // admin menu
      while (true) {
        printAdminMenu();
        std::cin >> choice;
        std::cin.ignore();
        if (choice == AdminUI::addMovie)
          adminAddMovie();
        else if (choice == AdminUI::removeMovie)
          adminRemoveMovie();
        else if (choice == AdminUI::updateMovie)
          adminUpdateMovie();
        else if (choice == AdminUI::printMovies)
          printAllMovies();
        else if (choice == AdminUI::exit)
          break;
        else
          std::cout << "Invalid choice! Please try again\n";
      }
    } else if (choice == 2) { // user menu
      while (true) {
        printUserMenu();
        UI::readInt(choice);
        if (choice == UserUI::browseMovies) {
          userBrowseMovies();
        } else if (choice == UserUI::removeMovieFromWatchList) {
          userRemoveMovieFromWatchList();
        } else if (choice == UserUI::printWatchList) {
          userPrintWatchList();
        } else if (choice == UserUI::openWatchListFile) {
          userOpenWatchListFile();
        } else if (choice == UserUI::exit) {
          break;
        } else {
          std::cout << "Invalid choice! Please try again\n";
        }
      }
    } else if (choice == 0) {
      std::cout << "Thank you for using the app!\n";
      break;
    } else
      std::cout << "Invalid choice! Please try again\n";
  }
}

void UI::printUserFileTypeMenu() {
  std::cout << "Choose the file type for the user:\n";
  std::cout << "1. CSV\n";
  std::cout << "2. HTML\n";
  std::cout << ">>> ";
}

void UI::printInitialMenu() {
  std::cout << "\nWelcome to the Movie Management App!\n";
  std::cout << "1. Admin\n";
  std::cout << "2. User\n";
  std::cout << "0. Exit\n";
  std::cout << ">>> ";
}

void UI::readInt(int& value) {
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

void UI::readChar(char& c) {
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

void UI::openFile(std::string file) const {
  std::string command = "open " + file;
  system(command.c_str());
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
  std::cout << "Number of movies: " << service.getMoviesCount() << "\n";
  int index = 0;
  for (auto movie : service.getMovies()) {
    std::cout << "============================================================================================================================\n";
    std::cout << index++ << ". " << movie.toStr() << "\n";
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
  Movie movie;

  try {
    movie = Movie(title, genre, yearOfRelease, numberOfLikes, trailer);
  } catch (DomainException& e) {
    std::cout << e.what() << "\n";
    return;
  }

  if (service.addMovie(movie))
    std::cout << "Movie added successfully\n";
  else
    std::cout << "Movie already exists\n";
}

void UI::adminRemoveMovie() {
  printAllMovies();
  int positionOfMovie;
  std::cout << "Enter the position of the movie you want to remove: ";
  UI::readInt(positionOfMovie);
  if (service.removeMovieByPosition(positionOfMovie))
    std::cout << "Movie removed successfully\n";
  else
    std::cout << "Movie does not exist\n";
}

void UI::adminUpdateMovie() {
  printAllMovies();
  int positionOfMovie;
  std::cout << "Please enter the position of the movie you want to update: ";
  UI::readInt(positionOfMovie);
  Movie movieToBeUpdated;
  try {
    movieToBeUpdated = service.getMovieByPosition(positionOfMovie);
  } catch (const RepositoryException& e) {
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
  std::getline(std::cin, readInput); // read the newline character
  if (readInput != "")
    titleOfMovieToBeUpdated = readInput;
  std::cout << "Please enter the genre of the movie. If you do not want to "
               "update this field, just press enter: ";
  std::getline(std::cin, readInput); // read the newline character
  if (readInput != "")
    genreOfMovieToBeUpdated = readInput;
  int integerField;
  std::cout << "Please enter the year of release of the movie. If you do not "
               "want to update this field, press 0: ";
  UI::readInt(integerField);
  if (integerField != 0)
    yearOfRelease = integerField;
  std::cout << "Please enter the likes count of the movie. If you do not want "
               "to update this field, press 0: ";
  UI::readInt(integerField);
  if (integerField != 0)
    likesCount = integerField;
  std::cout << "Please enter the trailer of the movie: If you do not want to "
               "update this field, just press enter:  ";
  std::getline(std::cin, readInput); // read the newline character
  if (readInput != "")
    trailerOfMovieToBeUpdated = readInput;
  Movie updatedMovie;

  try {
    updatedMovie = Movie(titleOfMovieToBeUpdated, genreOfMovieToBeUpdated, yearOfRelease, likesCount, trailerOfMovieToBeUpdated);
  } catch (const DomainException& e) {
    std::cout << e.what() << "\n";
    return;
  }

  if (service.updateMovie(positionOfMovie, updatedMovie))
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
  std::cout << "4. Open watch list file\n";
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
  std::vector<int> indexesOfMoviesByGenre =
      service.getMoviesByGenre(genre);

  int index = 0;
  while (true) {
    if (indexesOfMoviesByGenre.size() == 0) {
      std::cout << "There are no movies with the given genre\n";
      break;
    }
    std::cout
        << "==================================================================="
           "=========================================================\n";
    std::cout
        << "Title: 🍿"
        << service.getMovies()[indexesOfMoviesByGenre[index]].getTitle()
        << "\n"
        << "Genre: 🎞️ "
        << service.getMovies()[indexesOfMoviesByGenre[index]].getGenre()
        << "\n"
        << "Year of release: 🗓️ "
        << service.getMovies()[indexesOfMoviesByGenre[index]]
               .getYearOfRelease()
        << "\n"
        << "Number of likes: 👍"
        << service.getMovies()[indexesOfMoviesByGenre[index]]
               .getNumberOfLikes()
        << "\n";
    service.openMovieTrailer(indexesOfMoviesByGenre[index]);
    system("sleep 2"); // to not show the below message too fast
    char addMovie = 'a';
    std::cout << "Do you want to add the movie to your watch list?(y/n) ";
    while (addMovie != 'n' && addMovie != 'y') {
      UI::readChar(addMovie);
    }
    if (addMovie == 'y') {
      // add movie to watch list
      if (service.addMovieToWatchList(indexesOfMoviesByGenre[index])) {
        std::cout << "Movie added to watch list\n";
      } else {
        std::cout << "Movie aready in the watch list\n";
      }
    }
    ++index;
    if (index == indexesOfMoviesByGenre.size()) {
      // reset index to go through the selected movies again
      index = 0;
    }
    char next = 'a';
    std::cout << "Do you want to see the next trailer?(y/n) ";
    while (next != 'n' && next != 'y') {
      UI::readChar(next);
    }
    if (next == 'n')
      break;
  }
}

void UI::userRemoveMovieFromWatchList() {
  userPrintWatchList();
  if (service.getWatchListCount() == 0)
    return;
  int positionOfMovieInWatchList = -1;
  while (true) {
    std::cout << "Enter the position of the movie you want to remove: ";
    UI::readInt(positionOfMovieInWatchList);
    if (!service.validatePositionWatchList(positionOfMovieInWatchList)) {
      std::cout << "Invalid index\n";
    } else {
      break;
    }
  }

  int positionOfMovieInRepository =
      service.getWatchList()[positionOfMovieInWatchList];

  char likeMovie = 'a'; // default value to enter the loop
  std::cout << "Did you enjoy watching the movie?(y/n) ";
  while (likeMovie != 'n' && likeMovie != 'y') {
    UI::readChar(likeMovie);
  }
  if (likeMovie == 'y') {
    /* if the user liked the movie, increase number of likes in repo */
    service.incrementLikes(positionOfMovieInRepository);
    std::cout << "Movie liked successfully\n";
  }
  // remove the index from the watch list
  service.removeMovieByPositionWatchList(positionOfMovieInWatchList);
}

void UI::userPrintWatchList() {
  std::cout << "Number of movies: " << service.getWatchList().size() << "\n";
  int index = 0;
  for (auto element : service.getWatchList()) {
    std::cout << "============================================================================================================================\n";
    std::cout << index++ << ". " << service.getMovies()[element].toStr() << "\n";
  }
}

void UI::userOpenWatchListFile() {
  service.saveWatchList();
  openFile(service.getFileNameWatchList());
}

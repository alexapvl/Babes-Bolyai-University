#include "../../include/domain/movie.h"

// Constructor
Movie::Movie(const std::string& title, const std::string& genre,
             int yearOfRelease, int numberOfLikes, const std::string& trailer)
    : title(title),
      genre(genre),
      yearOfRelease(yearOfRelease),
      numberOfLikes(numberOfLikes),
      trailer(trailer){};

// Destructor
Movie::~Movie() {}

// Copy function
Movie::Movie(const Movie& movie) {
  this->title = movie.title;
  this->genre = movie.genre;
  this->yearOfRelease = movie.yearOfRelease;
  this->numberOfLikes = movie.numberOfLikes;
  this->trailer = movie.trailer;
}

// Assignment operator
Movie& Movie::operator=(const Movie& movie) {
  if (this == &movie) {
    return *this;
  }
  this->title = movie.title;
  this->genre = movie.genre;
  this->yearOfRelease = movie.yearOfRelease;
  this->numberOfLikes = movie.numberOfLikes;
  this->trailer = movie.trailer;
  return *this;
}

// Equality operator
bool Movie::operator==(const Movie& movie) const {
  return this->title == movie.title && this->genre == movie.genre &&
         this->yearOfRelease == movie.yearOfRelease;
}

// Getters
std::string Movie::getTitle() const { return this->title; }

std::string Movie::getGenre() const { return this->genre; }

int Movie::getYearOfRelease() const { return this->yearOfRelease; }

int Movie::getNumberOfLikes() const { return this->numberOfLikes; }

std::string Movie::getTrailer() const { return this->trailer; }

// Setters
void Movie::setTitle(const std::string& title) { this->title = title; }

void Movie::setGenre(const std::string& genre) { this->genre = genre; }

void Movie::setYearOfRelease(int yearOfRelease) {
  // defensive programming
  validateYearOfRelease(yearOfRelease);
  this->yearOfRelease = yearOfRelease;
}

void Movie::setNumberOfLikes(int numberOfLikes) {
  // defensive programming
  validateNumberOfLikes(numberOfLikes);
  this->numberOfLikes = numberOfLikes;
}

void Movie::setTrailer(const std::string& trailer) {
  // defensive programming
  validateTrailer(trailer);
  this->trailer = trailer;
}

// Validators
void Movie::validateTrailer(const std::string& trailer) {
  std::regex trailerRegex(
      "((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//"
      "=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
  if (!std::regex_match(trailer, trailerRegex)) {
    throw std::invalid_argument("Invalid trailer link");
  }
}

void Movie::validateYearOfRelease(int yearOfRelease) {
  std::time_t result = std::time(nullptr);
  std::istringstream iss(ctime(&result));

  iss.ignore(20);
  int currentYear;
  iss >> currentYear;
  if (yearOfRelease < 0 || yearOfRelease > currentYear)
    throw std::invalid_argument("Year of release must be a positive integer");
}

void Movie::validateNumberOfLikes(int numberOfLikes) {
  if (numberOfLikes < 0)
    throw std::invalid_argument("Number of likes must be a positive integer");
}

void Movie::validateMovie() {
  validateTrailer(this->trailer);
  validateYearOfRelease(this->yearOfRelease);
  validateNumberOfLikes(this->numberOfLikes);
}

// String
std::string Movie::toStr() {
  return "🍿" + this->getTitle() + " | 🎞️ " + this->getGenre() + " | 🗓️ " +
         std::to_string(this->getYearOfRelease()) + " | 👍" +
         std::to_string(this->getNumberOfLikes()) + " | 🔗" +
         this->getTrailer();
}
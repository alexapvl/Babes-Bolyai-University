#include "../../include/domain/movie.h"

// Constructor
Movie::Movie(const std::string& title, const std::string& genre, int yearOfRelease, int numberOfLikes, const std::string& trailer) {
  this->setTitle(title);
  this->setGenre(genre);
  this->setYearOfRelease(yearOfRelease);
  this->setNumberOfLikes(numberOfLikes);
  this->setTrailer(trailer);
  MovieValidator::validateMovie(*this);
}

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

// Operators
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

bool Movie::operator==(const Movie& movie) const {
  return this->title == movie.title && this->genre == movie.genre && this->yearOfRelease == movie.yearOfRelease;
}

std::ostream& operator<<(std::ostream& output, const Movie& movie) {
  output << movie.getTitle() + "|" + movie.getGenre() + "|" + std::to_string(movie.getYearOfRelease()) + "|" + std::to_string(movie.getNumberOfLikes()) + "|" + movie.getTrailer();
  return output;
}

std::istream& operator>>(std::istream& input, Movie& movie) {
  std::string lineWithContentsOfMovie;
  std::getline(input, lineWithContentsOfMovie);

  std::istringstream iss(lineWithContentsOfMovie);
  std::string token; // will be used in the parsing of the line
  std::vector<std::string> vectorWithTokens;

  while (std::getline(iss, token, '|')) {
    vectorWithTokens.push_back(token);
  }

  int numberOfTokens = vectorWithTokens.size();
  if (numberOfTokens != 5)
    throw DomainException("Invalid number of arguments when reading the movie. Expected 5 and got " + std::to_string(numberOfTokens) + " arguments instead.");

  const std::string& title = vectorWithTokens[indexes::TITLE];
  const std::string& genre = vectorWithTokens[indexes::GENRE];
  const int& year_of_release = std::stoi(vectorWithTokens[indexes::NUMBER_OF_LIKES]);
  const int& number_of_likes = std::stoi(vectorWithTokens[indexes::NUMBER_OF_LIKES]);
  const std::string& trailer = vectorWithTokens[indexes::TRAILER];

  movie.setTitle(title);                   // no vallidation needed
  movie.setGenre(genre);                   // no validation needed
  movie.setYearOfRelease(year_of_release); // validations are done here
  movie.setNumberOfLikes(number_of_likes); // validations are done here
  movie.setTrailer(trailer);               // validations are done here

  return input;
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

void Movie::setYearOfRelease(int yearOfRelease) { this->yearOfRelease = yearOfRelease; }

void Movie::setNumberOfLikes(int numberOfLikes) { this->numberOfLikes = numberOfLikes; }

void Movie::setTrailer(const std::string& trailer) { this->trailer = trailer; }

// Validators
void MovieValidator::validateTrailer(const std::string& trailer) {
  std::regex trailerRegex("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
  if (!std::regex_match(trailer, trailerRegex)) {
    throw DomainException("Invalid trailer link");
  }
}

void MovieValidator::validateYearOfRelease(int yearOfRelease) {
  std::time_t result = std::time(nullptr);
  std::istringstream iss(ctime(&result));

  iss.ignore(20);
  int currentYear;
  iss >> currentYear;
  if (yearOfRelease < 0 || yearOfRelease > currentYear)
    throw DomainException("Year of release must be a positive integer");
}

void MovieValidator::validateNumberOfLikes(int numberOfLikes) {
  if (numberOfLikes < 0)
    throw DomainException("Number of likes must be a positive integer");
}

void MovieValidator::validateMovie(const Movie& movie) {
  validateTrailer(movie.getTrailer());
  validateYearOfRelease(movie.getYearOfRelease());
  validateNumberOfLikes(movie.getNumberOfLikes());
}

// String
std::string Movie::toStr() {
  return "🍿" + this->getTitle() + " | 🎞️ " + this->getGenre() + " | 🗓️ " + std::to_string(this->getYearOfRelease()) + " | 👍" + std::to_string(this->getNumberOfLikes()) + " | 🔗" + this->getTrailer();
}
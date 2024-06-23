#pragma once
#include "../exceptions/domainExceptions.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace indexes {
const int TITLE = 0;
const int GENRE = 1;
const int YEAR_OF_RELEASE = 2;
const int NUMBER_OF_LIKES = 3;
const int TRAILER = 4;
} // namespace indexes

class Movie {
private:
  std::string title, genre;
  int yearOfRelease, numberOfLikes;
  std::string trailer; // link to the trailer

public:
  // Constructor
  Movie(const std::string& title = "", const std::string& genre = "",
        int yearOfRelease = 0, int numberOfLikes = 0,
        const std::string& trailer = "https://www.youtube.com/temporary-trailer-link");
  // Destructor
  ~Movie();
  // Copy function
  Movie(const Movie& movie);
  // Operators
  Movie& operator=(const Movie& movie);
  bool operator==(const Movie& movie) const;
  friend std::ostream& operator<<(std::ostream& output, const Movie& movie);
  friend std::istream& operator>>(std::istream& input, Movie& movie);
  // Getters
  std::string getTitle() const;
  std::string getGenre() const;
  int getYearOfRelease() const;
  int getNumberOfLikes() const;
  std::string getTrailer() const;
  // Setters
  void setTitle(const std::string& title);
  void setGenre(const std::string& genre);
  void setYearOfRelease(int yearOfRelease);
  void setNumberOfLikes(int numberOfLikes);
  void setTrailer(const std::string& trailer);
  // String
  std::string toStr();
};

class MovieValidator {
public:
  static void validateTrailer(const std::string& trailer);
  static void validateYearOfRelease(int yearOfRelease);
  static void validateNumberOfLikes(int numberOfLikes);
  static void validateMovie(const Movie& movie);
};
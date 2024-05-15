#pragma once
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

class Movie {
 private:
  std::string title, genre;
  int yearOfRelease, numberOfLikes;
  std::string trailer;  // link to the trailer

 public:
  // Constructor
  Movie(const std::string& title = "", const std::string& genre = "",
        int yearOfRelease = 0, int numberOfLikes = 0,
        const std::string& trailer = "");
  // Destructor
  ~Movie();
  // Copy function
  Movie(const Movie& movie);
  // Assignment operator
  Movie& operator=(const Movie& movie);
  // Equality operator
  bool operator==(const Movie& movie) const;
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
  // Validators
  void validateTrailer(const std::string& trailer);
  void validateYearOfRelease(int yearOfRelease);
  void validateNumberOfLikes(int numberOfLikes);
  void validateMovie();
  // String
  std::string toStr();
};
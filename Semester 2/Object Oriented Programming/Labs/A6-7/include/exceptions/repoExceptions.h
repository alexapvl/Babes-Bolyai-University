#pragma once
#include <exception>
#include <string>

class FileException : public std::exception {
private:
  std::string message;

public:
  FileException(const std::string& message) : message(message) {}
  const char* what() const _NOEXCEPT { return message.c_str(); }
};

class RepositoryException : public std::exception {
private:
  std::string message;

public:
  RepositoryException(const std::string& message) : message(message) {}
  const char* what() const _NOEXCEPT { return message.c_str(); }
};
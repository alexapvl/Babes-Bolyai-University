#pragma once
#include <exception>
#include <string>

class DomainException : public std::exception {
private:
  std::string message;

public:
  DomainException(const std::string& message) : message(message) {}
  const char* what() const _NOEXCEPT { return message.c_str(); }
};
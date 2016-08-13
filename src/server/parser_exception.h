#ifndef MEDIS_SERVER_PARSER_EXCEPTION_H_
#define MEDIS_SERVER_PARSER_EXCEPTION_H_

#include <exception>
#include <string>
#include <utility>

class ParserException : public std::exception {
 public:
  explicit ParserException(const std::string& s) : _s(std::move(s)) {}

  const char* what() const noexcept;

 private:
  std::string _s;
};

#endif  // MEDIS_SERVER_PARSER_EXCEPTION_H_

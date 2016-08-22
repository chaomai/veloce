#ifndef MEDIS_LIB_THREAD_EXCEPTION_H_
#define MEDIS_LIB_THREAD_EXCEPTION_H_

#include <exception>
#include <string>
#include <utility>

class ThreadException : public std::exception {
 public:
  explicit ThreadException(const std::string& s) : _s(std::move(s)) {}
  const char* what() const noexcept;

 private:
  std::string _s;
};

#endif  // MEDIS_LIB_THREAD_EXCEPTION_H_

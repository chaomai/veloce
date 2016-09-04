#ifndef MEDIS_LIB_SOCKET_EXCEPTION_H_
#define MEDIS_LIB_SOCKET_EXCEPTION_H_

#include <exception>
#include <string>
#include <utility>

class SocketException : public std::exception {
 public:
  explicit SocketException(const std::string& s) : _s(std::move(s)) {}
  const char* what() const noexcept;
  SocketException(const SocketException& rhs) = default;
  SocketException(SocketException&& rhs) = default;
  SocketException& operator=(const SocketException& rhs) = default;
  SocketException& operator=(SocketException&& rhs) = default;
  ~SocketException() = default;

 private:
  std::string _s;
};

#endif  // MEDIS_LIB_SOCKET_EXCEPTION_H_

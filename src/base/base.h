#ifndef VELOCE_BASE_BASE_H_
#define VELOCE_BASE_BASE_H_

#include <algorithm>
#include <cctype>   // for tolower
#include <cerrno>   // for errno
#include <cstddef>  // for size_t
#include <cstdio>   // for snprintf,
#include <cstring>  // for strerror
#include <memory>
#include <string>
#include <vector>

struct Args {
  std::size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
};

enum ItemType {
  VELOCE_INT,
  VELOCE_DOUBLE,
  VELOCE_DICT,
  VELOCE_LIST,
  VELOCE_STRING
};

struct Item {
  ItemType _type;
  void* _value_ptr;
};

constexpr std::size_t DBS_SIZE = 3;
constexpr std::size_t BUF_SIZE = 1024;

constexpr const char* STATUS_REP_PREFIX = "+";
constexpr const char* ERROR_REP_PREFIX = "-";
constexpr const char* INTEGER_REP_PREFIX = ":";
constexpr const char* BULK_REP_PREFIX = "$";
constexpr const char* MULTI_BULK_REP_PREFIX = "*";

constexpr const char* MSG_OK = "OK";
constexpr const char* MSG_ERR = "ERR";
constexpr const char* MSG_WRONGTYPE = "WRONGTYPE";
constexpr const char* MSG_PONG = "PONG";

constexpr const char* MSG_ERR_ARG_NUM =
    "wrong number of arguments for '%s' command";

constexpr const char* MSG_SPACE = " ";
constexpr const char* MSG_CRLF = "\r\n";
constexpr const char MSG_CR_TAG = '\r';
constexpr const char MSG_SIGLE_TAG = '*';
constexpr const char MSG_BATCH_TAG = '$';

inline std::string get_errno_str(int errnum = errno) {
  std::size_t size = static_cast<std::size_t>(
      snprintf(nullptr, 0, "%s", strerror(errnum)) + 1);
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, "%s", strerror(errnum));
  return std::string(buf.get(), buf.get() + size - 1);
}

inline void to_lower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), tolower);
}

template <typename... Args>
inline std::string string_format(const std::string& fmt, Args... args) {
  // + 1 for '\0'
  std::size_t size =
      static_cast<std::size_t>(snprintf(nullptr, 0, fmt.c_str(), args...) + 1);
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, fmt.c_str(), args...);

  // leave out '\0'
  return std::string(buf.get(), buf.get() + size - 1);
}

#endif  // VELOCE_BASE_BASE_H_

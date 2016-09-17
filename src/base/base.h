#ifndef MEDIS_BASE_BASE_H_
#define MEDIS_BASE_BASE_H_

#include <algorithm>
#include <cctype>   // for tolower
#include <cerrno>   // for errno
#include <cstddef>  // for size_t
#include <cstdio>   // for sprintf
#include <cstring>  // for strerror
#include <string>
#include <vector>

struct Args {
  std::size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
};

enum ItemType { MEDIS_INT, MEDIS_DOUBLE, MEDIS_DICT, MEDIS_LIST, MEDIS_STRING };

struct Item {
  ItemType _type;
  void* _value_ptr;
};

constexpr std::size_t DBS_SIZE = 3;
constexpr std::size_t BUF_SIZE = 1024;

constexpr const char* MSG_CRLF = "\r\n";
constexpr const char* MSG_OK = "+OK\r\n";
constexpr const char* MSG_ERR = "-ERROR\r\n";

constexpr const char MSG_CR_TAG = '\r';
constexpr const char MSG_NULL_TAG = '\0';
constexpr const char MSG_SIGLE_TAG = '*';
constexpr const char MSG_BATCH_TAG = '$';

inline std::string get_errno_str(int errnum = errno) {
  char err_str[BUF_SIZE] = {};
  sprintf(err_str, "%s", strerror(errnum));
  return std::string(err_str);
}

inline void to_lower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), tolower);
}

#endif  // MEDIS_BASE_BASE_H_

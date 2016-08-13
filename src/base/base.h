#ifndef MEDIS_SERVER_BASE_H_
#define MEDIS_SERVER_BASE_H_

#include <string>
#include <vector>

struct Args {
  size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
};

constexpr int BUF_SIZE = 1024;

constexpr const char* MSG_CRLF = "\r\n";
constexpr const char* MSG = "+OK\r\n";
constexpr const char* MSG_ERR = "-ERROR\r\n";

constexpr const char MSG_CR_TAG = '\r';
constexpr const char MSG_NULL_TAG = '\0';
constexpr const char MSG_SIGLE_TAG = '*';
constexpr const char MSG_BATCH_TAG = '$';

constexpr const char* FMT_STR = "%s";

#endif  // MEDIS_SERVER_BASE_H_

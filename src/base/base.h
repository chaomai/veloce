#ifndef MEDIS_SERVER_BASE_H_
#define MEDIS_SERVER_BASE_H_

#include <cstddef>  // for size_t
#include <string>
#include <vector>

struct Args {
  std::size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
};

enum ItemType {
  MEDIS_INT,
  MEDIS_DOUBLE,
  MEDIS_DICT,
  MEDIS_LIST,
  MEDIS_STRING,
  MEDIS_KDTREE,
  MEDIS_ITEM
};

struct Item {
  std::size_t _key_hash;
  std::string _key;
  ItemType _type;
  void* _value_ptr;
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

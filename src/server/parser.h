#ifndef MEDIS_SERVER_PARSER_H_
#define MEDIS_SERVER_PARSER_H_

#include <cstddef>  // for size_t
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"

class Parser {
 public:
  enum class State { OK, ERROR };

  std::size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
  State _state = State::ERROR;
  std::string _state_msg;

  Parser(const std::string* in) : _in(in) {}
  Parser(const Parser& rhs) = delete;
  Parser(Parser&& rhs) = delete;
  Parser& operator=(const Parser& rhs) = delete;
  Parser& operator=(Parser&& rhs) = delete;
  ~Parser() = default;

  void parse();

 private:
  const std::string* _in = nullptr;
  // using at can ensure a valid postion.
  std::string::size_type _cur_pos = 0;

  // inline functions are only used in Parser, so it's unnecessary to define
  // them in the header file.
  inline std::pair<std::string, std::string::size_type> get_next_str();

  inline std::size_t get_next_size_t();
  inline std::string get_next_arg(std::string::size_type arg_len);

  inline void parse_command_args_count();
  inline void parse_command();
  inline void parse_args();
};

#endif  // MEDIS_SERVER_PARSER_H_

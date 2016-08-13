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

  size_t _command_args_count = 0;
  std::string _command;
  std::vector<std::string> _command_args;
  State _state = State::ERROR;
  std::string _state_msg;

  Parser() = default;
  Parser(const Parser& rhs) = delete;
  Parser(Parser&& rhs) = delete;
  Parser& operator=(const Parser& rhs) = delete;
  Parser& operator=(Parser&& rhs) = delete;
  ~Parser() = default;

  void parse(const std::string* in);

 private:
  const std::string* _in = nullptr;
  std::string::size_type _cur_pos = 0;

  void init(const std::string* in) {
    _command_args_count = 0;
    _command.clear();
    _command_args.clear();
    _state = State::ERROR;
    _state_msg.clear();

    _in = in;
    _cur_pos = 0;
  }

  inline std::pair<std::string, std::string::size_type> get_next_str();

  inline size_t get_next_size_t();
  inline std::string get_next_arg(std::string::size_type arg_len);

  inline void parse_command_args_count();
  inline void parse_command();
  inline void parse_args();
};

#endif  // MEDIS_SERVER_PARSER_H_

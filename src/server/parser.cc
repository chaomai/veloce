#include "parser.h"

#include <string>
#include <utility>

#include "base/base.h"
#include "parser_exception.h"

using std::pair;
using std::string;
using std::stoul;

void Parser::parse() {
  try {
    parse_command_args_count();
    _state = State::OK;
  } catch (ParserException& e) {
    _state_msg = string(e.what());
    _state = State::ERROR;
  } catch (...) {
    _state_msg = string(MSG_ERR);
    _state = State::ERROR;
    return;
  }
}

pair<string, string::size_type> Parser::get_next_str() {
  auto begin = _cur_pos;
  string::size_type len = 0;

  while (_in->at(_cur_pos) != MSG_CR_TAG) {
    ++len;
    ++_cur_pos;
  }

  if (begin == _cur_pos) {
    throw ParserException(MSG_ERR);
  }

  auto str_len = _cur_pos - begin;

  return {_in->substr(begin, str_len), str_len};
}

size_t Parser::get_next_size_t() {
  auto ret = stoul(get_next_str().first);
  _cur_pos += 2;
  return ret;
}

string Parser::get_next_arg(string::size_type str_len) {
  auto next_str = get_next_str();

  if (next_str.second != str_len) {
    throw ParserException(MSG_ERR);
  }

  _cur_pos += 2;
  return next_str.first;
}

void Parser::parse_command_args_count() {
  if (_in->at(_cur_pos++) != MSG_SIGLE_TAG) {
    throw ParserException(MSG_ERR);
  }

  _command_args_count = get_next_size_t();

  parse_command();
}

void Parser::parse_command() {
  if (_in->at(_cur_pos++) != MSG_BATCH_TAG) {
    throw ParserException(MSG_ERR);
  }

  auto str_len = get_next_size_t();
  _command = get_next_arg(str_len);

  parse_args();
}

void Parser::parse_args() {
  for (string::size_type i = 0; i < _command_args_count - 1; ++i) {
    if (_in->at(_cur_pos++) != MSG_BATCH_TAG) {
      throw ParserException(MSG_ERR);
    }

    auto str_len = get_next_size_t();
    _command_args.push_back(get_next_arg(str_len));
  }

  if (_cur_pos != _in->size()) {
    throw ParserException(MSG_ERR);
  }
}

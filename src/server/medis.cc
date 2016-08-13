#include "medis.h"

#include <cstddef>  // for size_t
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"
#include "server/parser.h"

using std::string;
using std::vector;

void Medis::handle(const string& in, string& out) {
  Args args;
  _parser.parse(&in);

  switch (_parser._state) {
    case (Parser::State::OK): {
      // size_t command_args_count = _parser._command_args_count;
      // string command = std::move(_parser._command);
      // vector<string> command_args = std::move(_parser._command_args);
      out = _parser._command;
      break;
    }
    case (Parser::State::ERROR): {
      out = _parser._state_msg;
      break;
    }
  }
}

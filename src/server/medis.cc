#include "medis.h"

#include <cstddef>  // for size_t
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"
#include "parser.h"

using std::string;
using std::vector;

void Medis::handle(const string& in, string& out) {
  // parser has members, cannot be shared among multiple threads.
  // use it as local object.
  Parser _parser(&in);
  _parser.parse();

  switch (_parser._state) {
    case (Parser::State::OK): {
      Args args;
      args._command_args_count = _parser._command_args_count;
      // avoid copy.
      args._command = std::move(_parser._command);
      args._command_args = std::move(_parser._command_args);

      out = args._command;
      break;
    }
    case (Parser::State::ERROR): {
      out = _parser._state_msg;
      break;
    }
  }
}

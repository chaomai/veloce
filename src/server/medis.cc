#include "medis.h"

#include <cstddef>  // for size_t
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"
#include "parser.h"

using std::string;
using std::vector;

Medis::Medis() { init_handler(); }

void Medis::handle(const string& in, string& out) {
  // parser has members, cannot be shared among multiple threads.
  // use it as local object.
  Parser _parser(&in);
  _parser.parse();

  switch (_parser._state) {
    case (Parser::State::OK): {
      Args args;
      args._command_args_count = _parser._command_args_count;
      args._command = std::move(_parser._command);
      args._command_args = std::move(_parser._command_args);

      _handlers[args._command](args, out);
      break;
    }
    case (Parser::State::ERROR): {
      out = _parser._state_msg;
      break;
    }
  }
}

void Medis::init_handler() {
  // connection
  _handlers.insert("echo", [](const Args& args, string& out) {
    if (args._command_args_count != 1) {
      out = MSG_ERR;
    } else {
      out = args._command_args[0];
    }
  });

  _handlers.insert("ping", [](const Args& args, string& out) { out = "pong"; });

  // string
  // append
  // bitcount
  // decr
  // decrby
  // get
  // getrange
  // getset
  // incr
  // incrby
  // incrbyfloat
  // mget
  // mset
  // msetnx
  // set
  // setnx
  // setrange
  // strlen

  // list
  // llen
  // lpop
  // lpush
  // lpushx
  // lrem
  // lset
  // rpop
  // rpush
  // rpushx
}

#include "medis.h"

#include <cstddef>  // for size_t
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"
#include "client_info.h"
#include "parser.h"

using std::string;
using std::vector;

Medis::Medis() : _handlers(100) { init_handler(); }

void Medis::handle(ClientInfo& client_info) {
  // parser has members, cannot be shared among multiple threads.
  // use it as local object.
  Parser _parser(&client_info._in);
  _parser.parse();

  switch (_parser._state) {
    case (Parser::State::OK): {
      Args args;
      args._command_args_count = _parser._command_args_count;
      args._command = std::move(_parser._command);
      args._command_args = std::move(_parser._command_args);

      try {
        _handlers[args._command](args, client_info);
      } catch (std::out_of_range& e) {
        client_info._out = MSG_ERR;
      }

      break;
    }
    case (Parser::State::ERROR): {
      client_info._out = _parser._state_msg;
      break;
    }
  }
}

void Medis::init_handler() {
  // connection
  _handlers.insert("echo", [](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      client_info._out = MSG_ERR;
    } else {
      client_info._out = args._command_args[0];
    }
  });

  _handlers.insert("ping", [](const Args& args, ClientInfo& client_info) {
    client_info._out = "pong";
  });

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
  _handlers.insert("set", [](const Args& args, ClientInfo& client_info) {});
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

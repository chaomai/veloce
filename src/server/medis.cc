#include "medis.h"

#include <cstddef>  // for size_t
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "base/base.h"
#include "client_info.h"
#include "db.h"
#include "parser.h"

using std::size_t;
using std::stoul;
using std::string;
using std::vector;

Medis::Medis() : _dbs(new Db[DBS_SIZE]), _handlers(100) { init_handler(); }

Medis::~Medis() { delete[] _dbs; }

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

  _handlers.insert("select", [](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      client_info._out = MSG_ERR;
    }

    size_t db_num = stoul(args._command_args[0]);

    if (db_num >= DBS_SIZE) {
      client_info._out = MSG_ERR;
    } else {
      client_info._current_db_num = db_num;
      client_info._out = MSG_OK;
    }
  });

  // string
  _handlers.insert("append",
                   [this](const Args& args, ClientInfo& client_info) {});
  // bitcount
  // decr

  _handlers.insert("get", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      client_info._out = MSG_ERR;
    }

    Item* item = _dbs[client_info._current_db_num].get(args);

    if (item != nullptr) {
      switch (item->_type) {
        case MEDIS_INT:
        case MEDIS_DOUBLE:
        case MEDIS_STRING: {
          string* str = reinterpret_cast<string*>(item->_value_ptr);
          client_info._out = *str;
          break;
        }
      }
    } else {
      client_info._out = MSG_ERR;
    }
  });
  // getrange
  // getset
  // incr
  // incrby
  // incrbyfloat
  // mget
  // mset
  // msetnx

  _handlers.insert("set", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      client_info._out = MSG_ERR;
    } else {
      if (_dbs[client_info._current_db_num].set(args)) {
        client_info._out = MSG_OK;
      } else {
        client_info._out = MSG_ERR;
      }
    }
  });

  _handlers.insert("setnx", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      client_info._out = MSG_ERR;
    } else {
      if (_dbs[client_info._current_db_num].setnx(args)) {
        client_info._out = MSG_OK;
      } else {
        client_info._out = MSG_ERR;
      }
    }
  });
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

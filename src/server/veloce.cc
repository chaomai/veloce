#include "veloce.h"

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
using std::to_string;
using std::vector;

Veloce::Veloce() : _dbs(new Db[DBS_SIZE]), _handlers(100) { init_handler(); }

Veloce::~Veloce() { delete[] _dbs; }

void Veloce::handle(ClientInfo& client_info) {
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
        append_error(client_info._out, MSG_ERR);
      }

      break;
    }
    case (Parser::State::ERROR): {
      append_error(client_info._out, MSG_ERR, _parser._state_msg);
      break;
    }
  }
}

void Veloce::init_handler() {
  /*
   * connection
   */
  _handlers.insert("echo", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      append_error(client_info._out, MSG_ERR,
                   string_format(MSG_ERR_ARG_NUM, "echo"));
    } else {
      append_item(client_info._out, args._command_args[0]);
    }
  });

  _handlers.insert("ping", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count == 0) {
      client_info._out += STATUS_REP_PREFIX;
      client_info._out += MSG_PONG;
    } else if (args._command_args_count == 1) {
      append_item(client_info._out, args._command_args[0]);
    } else {
      append_error(client_info._out, MSG_ERR,
                   string_format(MSG_ERR_ARG_NUM, "ping"));
    }
  });

  _handlers.insert("select", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      append_error(client_info._out, MSG_ERR,
                   string_format(MSG_ERR_ARG_NUM, "select"));
    } else {
      size_t db_num = stoul(args._command_args[0]);

      if (db_num >= DBS_SIZE) {
        append_error(client_info._out, MSG_ERR);
      } else {
        client_info._current_db_num = db_num;
        append_item(client_info._out, MSG_OK);
      }
    }
  });

  /*
   * keys
   */
  _handlers.insert("del", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count < 1) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].del(args);

      if (ret_pair.second == Db::State::OK) {
        client_info._out = to_string(ret_pair.first);
      }
    }
  });

  _handlers.insert("exists", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].exists(args);

      if (ret_pair.second == Db::State::OK) {
        client_info._out = to_string(ret_pair.first);
      } else {
        // client_info._out = MSG_NIL;
      }
    }
  });

  /*
   * strings
   */
  _handlers.insert("append", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].append(args);

      if (ret_pair.second == Db::State::OK) {
        client_info._out = to_string(ret_pair.first);
      } else {
        append_error(client_info._out, MSG_ERR);
      }
    }
  });

  // bitcount
  // decr

  _handlers.insert("get", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].get(args);

      if (ret_pair.second == Db::State::OK) {
        Item* item = ret_pair.first;

        if (item != nullptr) {
          string* str = reinterpret_cast<string*>(item->_value_ptr);
          client_info._out = *str;
        } else {
          // client_info._out = MSG_NIL;
        }
      } else {
        append_error(client_info._out, MSG_ERR);
      }
    }
  });

  // getrange

  _handlers.insert("getset", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].getset(args);

      if (ret_pair.second == Db::State::OK) {
        Item* item = ret_pair.first;

        if (item != nullptr) {
          string* str = reinterpret_cast<string*>(item->_value_ptr);
          client_info._out = *str;
        } else {
          // client_info._out = MSG_NIL;
        }
      } else {
        append_error(client_info._out, MSG_ERR);
      }
    }

  });

  // incr
  // incrby
  // incrbyfloat
  // mget
  // mset
  // msetnx

  _handlers.insert("set", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].set(args);

      if (ret_pair.second == Db::State::OK) {
        client_info._out = to_string(ret_pair.first);
      } else {
        append_error(client_info._out, MSG_ERR);
      }
    }
  });

  _handlers.insert("setnx", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      append_error(client_info._out, MSG_ERR);
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].setnx(args);

      if (ret_pair.second == Db::State::OK) {
        client_info._out = to_string(ret_pair.first);
      } else {
        append_error(client_info._out, MSG_ERR);
      }
    }
  });

  // setrange

  _handlers.insert("strlen", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      append_error(client_info._out, MSG_ERR);
    } else {
    }
  });

  /*
   * list
   */
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

void Veloce::append_error(string& out, const string& err, const string& info) {
  out += ERROR_REP_PREFIX;
  out += MSG_ERR;

  if (info.size() > 0) {
    out += MSG_SPACE;
    out += info;
  }

  out += MSG_CRLF;
}

void Veloce::append_item(std::string& out, const Item* item) {}

void Veloce::append_item(std::string& out, const std::string& str) {
  out += MSG_BATCH_TAG;
  out += to_string(str.size());
  out += MSG_CRLF;
  out += str;
  out += MSG_CRLF;
}

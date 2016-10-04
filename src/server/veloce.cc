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
        build_error_reply(client_info._out, MSG_ERR);
      }

      break;
    }
    case (Parser::State::ERROR): {
      build_error_reply(client_info._out, MSG_ERR, _parser._state_msg);
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
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "echo"));
    } else {
      build_bulk_reply(client_info._out, args._command_args[0]);
    }
  });

  _handlers.insert("ping", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count == 0) {
      build_status_reply(client_info._out, MSG_PONG);
    } else if (args._command_args_count == 1) {
      build_bulk_reply(client_info._out, args._command_args[0]);
    } else {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "ping"));
    }
  });

  _handlers.insert("select", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "select"));
    } else {
      size_t db_num = stoul(args._command_args[0]);

      if (db_num >= DBS_SIZE) {
        build_error_reply(client_info._out, MSG_ERR);
      } else {
        client_info._current_db_num = db_num;
        build_status_reply(client_info._out, MSG_OK);
      }
    }
  });

  /*
   * keys
   */
  _handlers.insert("del", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count < 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "del"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].del(args);

      if (ret_pair.second == Db::State::OK) {
        build_integer_reply(client_info._out, ret_pair.first);
      }
    }
  });

  _handlers.insert("exists", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "exists"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].exists(args);

      // state is always OK
      build_integer_reply(client_info._out, ret_pair.first);
    }
  });

  /*
   * strings
   */
  _handlers.insert("append", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "append"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].append(args);

      if (ret_pair.second == Db::State::OK) {
        build_integer_reply(client_info._out, ret_pair.first);
      } else {
        build_error_reply(client_info._out, MSG_ERR);
      }
    }
  });

  // bitcount
  // decr

  _handlers.insert("get", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "get"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].get(args);

      if (ret_pair.second == Db::State::OK) {
        Item* item = ret_pair.first;

        if (item != nullptr) {
          string* str = reinterpret_cast<string*>(item->_value_ptr);
          build_bulk_reply(client_info._out, *str);
        } else {
          build_bulk_reply(client_info._out, "");
        }
      } else {
        build_error_reply(client_info._out, MSG_ERR);
      }
    }
  });

  // getrange

  _handlers.insert("getset", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "getset"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].getset(args);

      if (ret_pair.second == Db::State::OK) {
        Item* item = ret_pair.first;

        if (item != nullptr) {
          string* str = reinterpret_cast<string*>(item->_value_ptr);
          build_bulk_reply(client_info._out, *str);
        } else {
          build_bulk_reply(client_info._out, "");
        }
      } else {
        build_error_reply(client_info._out, MSG_ERR);
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
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "set"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].set(args);

      if (ret_pair.second == Db::State::OK) {
        build_status_reply(client_info._out, MSG_OK);
      } else {
        build_error_reply(client_info._out, MSG_ERR);
      }
    }
  });

  _handlers.insert("setnx", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "setnx"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].setnx(args);

      if (ret_pair.second == Db::State::OK) {
        build_integer_reply(client_info._out, ret_pair.first);
      } else {
        build_error_reply(client_info._out, MSG_ERR);
      }
    }
  });

  // setrange

  _handlers.insert("strlen", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "strlen"));
    } else {
      auto ret_pair = _dbs[client_info._current_db_num].strlen(args);

      if (ret_pair.second == Db::State::OK) {
        build_integer_reply(client_info._out, ret_pair.first);
      } else {
        build_error_reply(client_info._out, MSG_ERR);
      }
    }
  });

  /*
   * list
   */
  _handlers.insert("llen", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "llen"));
    } else {
    }
  });

  _handlers.insert("lpop", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 1) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "lpop"));
    } else {
    }
  });

  _handlers.insert("lpush", [this](const Args& args, ClientInfo& client_info) {
    if (args._command_args_count != 2) {
      build_error_reply(client_info._out, MSG_ERR,
                        string_format(MSG_ERR_ARG_NUM, "lpush"));
    } else {
    }
  });
}

void Veloce::build_status_reply(std::string& out, const std::string& str) {
  out += STATUS_REP_PREFIX;
  out += str;
  out += MSG_CRLF;
}

void Veloce::build_error_reply(string& out, const string& err,
                               const string& info) {
  out += ERROR_REP_PREFIX;
  out += MSG_ERR;

  if (info.size() > 0) {
    out += MSG_SPACE;
    out += info;
  }

  out += MSG_CRLF;
}

void Veloce::build_integer_reply(std::string& out, int val) {
  out += INTEGER_REP_PREFIX;
  out += to_string(val);
  out += MSG_CRLF;
}

void Veloce::build_bulk_reply(std::string& out, const std::string& str) {
  out += MSG_BATCH_TAG;

  if (str.size() == 0) {
    out += "-1";
  } else {
    out += to_string(str.size());
    out += MSG_CRLF;
    out += str;
  }

  out += MSG_CRLF;
}

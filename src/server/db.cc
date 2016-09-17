#include "db.h"

#include <string>

#include "base/base.h"

using std::string;

int Db::set(const Args& args) {
  const string& key = args._command_args[0];
  string* value = new string(args._command_args[1]);

  Item* item = new Item({MEDIS_STRING, value});
  _dict.set(key, item);

  return 1;
}

int Db::setnx(const Args& args) {
  const string& key = args._command_args[0];
  Item* item = get(args);

  if (item == nullptr) {
    return set(args);
  }

  return 0;
}

Item* Db::get(const Args& args) {
  const string& key = args._command_args[0];
  return _dict.get(key);
}

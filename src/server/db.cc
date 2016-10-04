#include "db.h"

#include <atomic>
#include <string>
#include <utility>

#include "base/base.h"
#include "ds/list.h"

using std::atomic_int;
using std::string;
using std::pair;

using ds::List;

/*
 * keys
 */
pair<int, Db::State> Db::del(const Args& args) {
  atomic_int count(0);

  for (const string& key : args._command_args) {
    if (_dict.remove(key)) {
      ++count;
    }
  }
  return {count, State::OK};
}

pair<bool, Db::State> Db::exists(const Args& args) {
  const string& key = args._command_args[0];

  if (_dict.exists(key)) {
    return {true, State::OK};
  }
  return {false, State::OK};
}

/*
 * strings
 */
pair<int, Db::State> Db::append(const Args& args) {
  while (true) {
    auto old_pair = get(args);

    if (old_pair.second == State::TYPE_ERROR) {
      return {0, State::TYPE_ERROR};
    }

    Item* old_item = old_pair.first;

    if (old_item != nullptr) {
      const string& key = args._command_args[0];
      string* str = reinterpret_cast<string*>(old_item->_value_ptr);
      string* value = new string(*str + args._command_args[1]);

      Item* new_item = new Item({VELOCE_STRING, value});

      if (_dict.compare_and_set(key, old_item, new_item)) {
        return {value->size(), State::OK};
      }
    } else {
      const string& key = args._command_args[0];
      string* value = new string(args._command_args[1]);
      Item* new_item = new Item({VELOCE_STRING, value});

      if (_dict.set(key, new_item)) {
        return {value->size(), State::OK};
      }
    }
  }
}

pair<Item*, Db::State> Db::get(const Args& args) {
  Item* item = get_raw(args);

  if (check_type(item, {VELOCE_INT, VELOCE_DOUBLE, VELOCE_STRING}) ==
      State::OK) {
    return {item, State::OK};
  } else {
    return {nullptr, State::TYPE_ERROR};
  }
}

pair<Item*, Db::State> Db::getset(const Args& args) {
  while (true) {
    auto old_pair = get(args);

    if (old_pair.second == State::TYPE_ERROR) {
      return {nullptr, State::TYPE_ERROR};
    }

    Item* old_item = old_pair.first;

    const string& key = args._command_args[0];
    string* value = new string(args._command_args[1]);
    Item* new_item = new Item({VELOCE_STRING, value});

    // if key doesn't exist.
    if (old_item == nullptr && _dict.set(key, new_item)) {
      return {old_item, State::OK};
    }

    // if failed, try again.
    if (_dict.compare_and_set(key, old_item, new_item)) {
      return {old_item, State::OK};
    }
  }
}

pair<int, Db::State> Db::set(const Args& args) {
  while (true) {
    const string& key = args._command_args[0];
    string* value = new string(args._command_args[1]);
    Item* new_item = new Item({VELOCE_STRING, value});

    if (_dict.set(key, new_item)) {
      return {1, State::OK};
    }

    // key is already taken, get key first and check the type.
    auto old_pair = get(args);

    if (old_pair.second == State::TYPE_ERROR) {
      return {0, State::TYPE_ERROR};
    }

    Item* old_item = old_pair.first;

    // if failed, try again.
    if (_dict.compare_and_set(key, old_item, new_item)) {
      return {1, State::OK};
    }
  }
}

pair<int, Db::State> Db::setnx(const Args& args) {
  auto old_pair = get(args);

  if (old_pair.second == State::TYPE_ERROR) {
    return {0, State::TYPE_ERROR};
  }

  if (old_pair.first != nullptr) {
    return {0, State::OK};
  }

  Item* old_item = old_pair.first;
  const string& key = args._command_args[0];
  string* value = new string(args._command_args[1]);
  Item* new_item = new Item({VELOCE_STRING, value});

  if (_dict.compare_and_set(key, old_item, new_item)) {
    return {1, State::OK};
  } else {
    return {0, State::OK};
  }
}

pair<int, Db::State> Db::strlen(const Args& args) {
  auto ret_pair = get(args);

  if (ret_pair.second == State::TYPE_ERROR) {
    return {0, State::TYPE_ERROR};
  }

  Item* item = ret_pair.first;

  if (item != nullptr) {
    string* str = reinterpret_cast<string*>(item->_value_ptr);
    return {static_cast<int>(str->size()), State::OK};
  } else {
    return {0, State::OK};
  }
}

/*
 * list
 */

Item* Db::get_raw(const Args& args) {
  const string& key = args._command_args[0];
  Item* item = _dict.get(key);
  return item;
}

Db::State Db::check_type(Item* item,
                         const std::initializer_list<ItemType>& valid_types) {
  if (item == nullptr) {
    return State::OK;
  }

  for (auto iter = valid_types.begin(); iter != valid_types.end(); ++iter) {
    if (item->_type == *iter) {
      return State::OK;
    }
  }

  return State::TYPE_ERROR;
}

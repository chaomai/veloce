#include "dict.h"

#include "base/base.h"

namespace ds {

bool Dict::exists(const key_type& key) {
  auto ret = _hash_table.find(key);

  if (ret) {
    return true;
  }

  return false;
}

bool Dict::set(const key_type& key, Item* item) {
  return _hash_table.insert(key, item);
}

bool Dict::compare_and_set(const key_type& key, Item* old_item,
                           Item* new_item) {
  return _hash_table.compare_and_set(key, old_item, new_item);
}

Item* Dict::get(const key_type& key) {
  auto ret = _hash_table.find(key);

  if (ret) {
    return ret.value();
  }

  return nullptr;
}

bool Dict::remove(const key_type& key) { return _hash_table.erase(key); }
}

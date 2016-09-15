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

void Dict::set(const key_type& key, Item* item) {
  _hash_table.insert(key, item);
}

Item* Dict::get(const key_type& key) {
  auto ret = _hash_table.find(key);

  if (ret) {
    return ret.value();
  }

  return nullptr;
}

void Dict::remove(const key_type& key) { _hash_table.erase(key); }
}

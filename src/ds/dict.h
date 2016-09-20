#ifndef MEDIS_DS_DICT_H_
#define MEDIS_DS_DICT_H_

#include <functional>  // for hash
#include <string>

#include "base/base.h"
#include "ds/coarse_grained_hash_table.h"

namespace ds {

class Dict {
  using key_type = std::string;
  using hasher = std::hash<key_type>;
  using key_equal = std::equal_to<key_type>;
  using hash_table =
      ds::CoarseGrainedHashTable<key_type, Item*, hasher, key_equal>;
  using size_type = std::size_t;

 public:
  Dict() = default;
  Dict(const Dict& rhs) = delete;
  Dict(Dict&& rhs) = delete;
  Dict& operator=(const Dict& rhs) = delete;
  Dict& operator=(Dict&& rhs) = delete;
  ~Dict() = default;

  bool exists(const key_type& key);
  inline size_type size() const;

  bool set(const key_type& key, Item* item);
  bool compare_and_set(const key_type& key, Item* old_item, Item* new_item);
  Item* get(const key_type& key);
  bool remove(const key_type& key);

 private:
  hash_table _hash_table;
};

// size is inline and public, so it must be defined in the header file.
typename Dict::size_type Dict::size() const { return _hash_table.size(); }
}

#endif  // MEDIS_DS_DICT_H_

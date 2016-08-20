#ifndef MEDIS_DS_HASH_TABLE_H_
#define MEDIS_DS_HASH_TABLE_H_

#include <cstddef>     // for size_t
#include <functional>  // for hash

// thread safe hash table
template <typename K, typename V, typename Hash = std::hash<K>,
          typename KeyEqual = std::equal_to<K>>
class HashTable {
 public:
  HashTable();
  HashTable(const HashTable &rhs);
  HashTable(HashTable &&rhs);
  HashTable &operator=(const HashTable &rhs);
  HashTable &operator=(HashTable &&rhs);
  ~HashTable();
};

#endif  // MEDIS_DS_HASH_TABLE_H_

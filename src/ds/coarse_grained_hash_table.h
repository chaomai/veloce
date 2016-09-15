#ifndef MEDIS_DS_COARSE_GRAINED_HASH_TABLE_H_
#define MEDIS_DS_COARSE_GRAINED_HASH_TABLE_H_

#include <atomic>
#include <cstddef>  // for size_t
#include <experimental/optional>
#include <functional>  // for hash
#include <mutex>
#include <shared_mutex>
#include <utility>

#include "list.h"

namespace ds {

namespace impl {
/**
 * @brief Slot of hash table
 */
template <typename T>
struct Slot {
  using iterator = typename List<T>::iterator;

  Slot();
  Slot(const Slot &rhs) = delete;
  Slot(Slot &&rhs) = delete;
  Slot &operator=(const Slot &rhs) = delete;
  Slot &operator=(Slot &&rhs) = delete;
  ~Slot() = default;

  List<T> _chain;
  std::atomic_size_t _slot_size;
  std::shared_timed_mutex _slot_mutex;
};

template <typename T>
Slot<T>::Slot() : _slot_size(0) {}

/**
 * @brief Thread safe hash table
 */
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class CoarseGrainedHashTable {
  using shared_mutex = std::shared_timed_mutex;
  using shared_lock = std::shared_lock<shared_mutex>;
  using unique_lock = std::unique_lock<shared_mutex>;
  using atomic_size_t = std::atomic_size_t;
  using atomic_float = std::atomic<float>;
  using mapped_type_optional = std::experimental::optional<T>;
  using hash_table_slot = Slot<std::pair<const Key, T>>;
  using hash_table_slot_iterator =
      typename Slot<std::pair<const Key, T>>::iterator;

 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using hasher = Hash;
  using key_equal = KeyEqual;

  CoarseGrainedHashTable(size_type slots_size = 4194304);
  CoarseGrainedHashTable(std::initializer_list<value_type> init);
  CoarseGrainedHashTable(const CoarseGrainedHashTable &rhs) = delete;
  CoarseGrainedHashTable(CoarseGrainedHashTable &&rhs) = delete;
  CoarseGrainedHashTable &operator=(const CoarseGrainedHashTable &rhs) = delete;
  CoarseGrainedHashTable &operator=(CoarseGrainedHashTable &&rhs) = delete;
  ~CoarseGrainedHashTable();

  mapped_type &operator[](const key_type &key);

  bool empty() const;
  size_type size() const;

  void insert(const Key &key, const T &value);
  void erase(const Key &key);

  mapped_type_optional find(const Key &key);

  inline float load_factor() const;

 private:
  inline size_type get_slot_num(const Key &key);
  std::pair<hash_table_slot_iterator, hash_table_slot &> find_impl(
      const Key &key);
  void insert_impl(hash_table_slot *slots, const Key &key, const T &value);

  hash_table_slot *_slots;

  atomic_size_t _slots_size;
  atomic_size_t _elements_count;

  hasher _hash_func;
  key_equal _key_eq_func;
};

/**
 * @brief Construct a hash table
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::CoarseGrainedHashTable(
    size_type slots_size)
    : _slots_size(slots_size), _elements_count(0), _hash_func(Hash()) {
  _slots = new hash_table_slot[slots_size];
}

/**
 * @brief Construct a hash table with a initializer list
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::CoarseGrainedHashTable(
    std::initializer_list<value_type> init)
    : CoarseGrainedHashTable(init.size()) {
  for (const auto &elem : init) {
    insert_impl(_slots, elem.first, elem.second);
  }
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::~CoarseGrainedHashTable() {
  delete[] _slots;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::mapped_type
    &CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::operator[](
        const key_type &key) {
  auto ret = find_impl(key);
  return (*ret.first).second;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
bool CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::empty() const {
  return _elements_count == 0;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
typename CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::size_type
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::size() const {
  return _elements_count;
}

/**
 * @brief Insert value into hash table. If value exists, then do update
 * @param value Value to insert
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
void CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::insert(const Key &key,
                                                            const T &value) {
  insert_impl(_slots, key, value);
}

/**
 * @brief Remove a key from hash table
 * @param key Key to remove
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
void CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::erase(const Key &key) {
  size_type slot_num = get_slot_num(key);

  hash_table_slot &slot = _slots[slot_num];
  unique_lock slot_lock(slot._slot_mutex);

  auto iter = slot._chain.find(
      [&key](const value_type &val) -> bool { return val.first == key; });

  if (iter != slot._chain.end()) {
    slot._chain.erase(iter);
    --slot._slot_size;
    --_elements_count;
  }
}

/**
 * @brief find value of key, return the copy of value
 * @param key Key to find
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
typename CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::mapped_type_optional
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::find(const Key &key) {
  auto ret = find_impl(key);

  if (ret.first != ret.second._chain.end()) {
    return mapped_type_optional((*ret.first).second);
  }

  return mapped_type_optional();
}

/**
 * @brief Get average load factor
 * @return load factor
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
float CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::load_factor() const {
  return static_cast<float>(_elements_count) / _slots_size;
}

/**
 * @brief Get slot number
 * @param key Key
 * @return slot number
 */
template <typename Key, typename T, typename Hash, typename KeyEqual>
typename CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::size_type
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::get_slot_num(const Key &key) {
  const size_type hash_value = _hash_func(key);
  return hash_value % _slots_size;
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
std::pair<
    typename CoarseGrainedHashTable<Key, T, Hash,
                                    KeyEqual>::hash_table_slot_iterator,
    typename CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::hash_table_slot &>
CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::find_impl(const Key &key) {
  size_type slot_num = get_slot_num(key);

  hash_table_slot &slot = _slots[slot_num];
  shared_lock slot_lock(slot._slot_mutex);

  return {slot._chain.find([&key](const value_type &val) -> bool {
            return val.first == key;
          }),
          slot};
}

template <typename Key, typename T, typename Hash, typename KeyEqual>
void CoarseGrainedHashTable<Key, T, Hash, KeyEqual>::insert_impl(
    hash_table_slot *slots, const Key &key, const T &value) {
  size_type slot_num = get_slot_num(key);
  hash_table_slot &slot = slots[slot_num];
  unique_lock slot_lock(slot._slot_mutex);

  auto iter = slot._chain.find(
      [&key](const value_type &val) { return val.first == key; });

  if (iter == slot._chain.end()) {
    slot._chain.push_back(value_type(key, value));

    ++slot._slot_size;
    ++_elements_count;
  } else {
    iter->second = value;
  }
}
}

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
using CoarseGrainedHashTable =
    impl::CoarseGrainedHashTable<Key, T, Hash, KeyEqual>;
}

#endif  // MEDIS_DS_COARSE_GRAINED_HASH_TABLE_H_

#ifndef MEDIS_DS_LIST_H_
#define MEDIS_DS_LIST_H_

#include <cstddef>  // for size_t
#include <functional>
#include <memory>

#include "lib/mutex.h"

/**
 * @brief thread safe list, using pthread mutex and fine-grained synchronization
 * to ensure thread safe.
 */
template <typename T, typename Hash = std::hash<T>,
          typename KeyEqual = std::equal_to<T>>
class List final {
  struct ListNode;
  class Iterator;

 public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = Iterator;

  List();
  List(const List& rhs) = delete;
  List(List&& rhs) = delete;
  List operator=(const List& rhs) = delete;
  List operator=(List& rhs) = delete;
  ~List();

  value_type front() const;
  value_type back() const;

  iterator begin();
  iterator end();

  bool empty() const {}
  size_type size() const { return _size; }

  void clear();
  void push_back();
  value_type pop_back();
  void push_front();
  value_type pop_front();

  Hash hash_function() const { return _hasher; }
  KeyEqual key_eq() const { return _key_equal; }

 private:
  std::shared_ptr<ListNode> _head;
  std::weak_ptr<ListNode> _tail;
  size_type _size;
  Hash _hasher;
  KeyEqual _key_equal;
};

/**
 * @brief create list with a dummy node.
 */
template <typename T, typename Hash, typename KeyEqual>
List<T, Hash, KeyEqual>::List() : _size(0) {
  _head = std::make_shared<ListNode>();
  _tail = _head;
}

/**
 * @brief destroy a list.
 */
template <typename T, typename Hash, typename KeyEqual>
List<T, Hash, KeyEqual>::~List() {}

template <typename T, typename Hash, typename KeyEqual>
struct List<T, Hash, KeyEqual>::ListNode {
  value_type _value;
  size_type _key;
  std::shared_ptr<ListNode> _next;
  std::weak_ptr<ListNode> _prev;
};

template <typename T, typename Hash, typename KeyEqual>
class List<T, Hash, KeyEqual>::Iterator {};

#endif  // MEDIS_DS_LIST_H_

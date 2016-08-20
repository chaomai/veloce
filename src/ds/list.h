#ifndef MEDIS_DS_LIST_H_
#define MEDIS_DS_LIST_H_

#include <cstddef>  // for size_t
#include <functional>
#include <memory>

// thread safe list
template <typename T, typename Hash = std::hash<T>,
          typename KeyEqual = std::equal_to<T>>
class List {
 private:
  class Iterator;
  struct ListNode;

 public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = Iterator;

  List() {}
  List(const List& rhs);
  List(List&& rhs) = delete;
  List operator=(const List& rhs);
  List operator=(List& rhs) = delete;
  ~List() {}

  value_type front() const;
  value_type back() const;

  iterator begin();
  iterator end();

  bool empty() const;
  size_type size() const { return _size; }

  void clear();
  void push_back();
  value_type pop_back();
  void push_front();
  value_type pop_front();

  Hash hash_function() const { return hasher; }
  KeyEqual key_eq() const { return key_equal; }

 private:
  std::shared_ptr<ListNode> _head;
  std::weak_ptr<ListNode> _tail;
  size_type _size;
  Hash hasher;
  KeyEqual key_equal;
};

template <typename T, typename Hash, typename KeyEqual>
class List<T, Hash, KeyEqual>::Iterator {};

template <typename T, typename Hash, typename KeyEqual>
struct List<T, Hash, KeyEqual>::ListNode {
  value_type _value;
  size_type _key;
  std::shared_ptr<ListNode> _next;
  std::weak_ptr<ListNode> _prev;
};

#endif  // MEDIS_DS_LIST_H_

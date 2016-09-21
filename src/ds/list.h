#ifndef VELOCE_DS_LIST_H_
#define VELOCE_DS_LIST_H_

#include <cstddef>  // for size_t
#include <functional>
#include <utility>

namespace ds {

namespace impl {
/**
 * @brief list node
 */
template <typename T>
struct ListNode {
  ListNode() = default;
  explicit ListNode(const T& value);
  explicit ListNode(T&& value);
  ListNode(const ListNode& rhs) = delete;
  ListNode(ListNode&& rhs) = delete;
  ListNode& operator=(const ListNode& rhs) = delete;
  ListNode& operator=(ListNode&& rhs) = delete;
  ~ListNode() = default;

  T _value;
  ListNode* _next = nullptr;
  ListNode* _prev = nullptr;
};

template <typename T>
ListNode<T>::ListNode(const T& value) : _value(value) {}

template <typename T>
ListNode<T>::ListNode(T&& value) : _value(std::move(value)) {}

/**
 * @brief list iterator
 */
template <typename T>
class ListIterator;

template <typename T>
class List;

template <typename T>
bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs);

template <typename T>
bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs);

template <typename T>
class ListIterator {
  friend class List<T>;
  friend bool operator==
      <T>(const ListIterator<T>& lhs, const ListIterator<T>& rhs);
  friend bool operator!=
      <T>(const ListIterator<T>& lhs, const ListIterator<T>& rhs);

  using node_type = ListNode<T>;
  using node_ptr = node_type*;

 public:
  explicit ListIterator(node_ptr node);
  ListIterator(const ListIterator& rhs);
  ListIterator(ListIterator&& rhs);
  ListIterator& operator=(const ListIterator& rhs);
  ListIterator& operator=(ListIterator&& rhs);
  ~ListIterator() = default;

  ListIterator& operator++();
  ListIterator operator++(int);
  ListIterator& operator--();
  ListIterator operator--(int);
  T& operator*() const;
  T* operator->() const;

 private:
  node_ptr _node_ptr;
};

template <typename T>
ListIterator<T>::ListIterator(node_ptr ptr) : _node_ptr(ptr) {}

template <typename T>
ListIterator<T>::ListIterator(const ListIterator& rhs)
    : _node_ptr(rhs._node_ptr) {}

template <typename T>
ListIterator<T>::ListIterator(ListIterator&& rhs) : _node_ptr(rhs._node_ptr) {
  rhs._node_ptr = nullptr;
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator=(const ListIterator& rhs) {
  _node_ptr = rhs._node_ptr;
  return *this;
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator=(ListIterator&& rhs) {
  if (this != &rhs) {
    _node_ptr = rhs._node_ptr;
    rhs._node_ptr = nullptr;
  }
  return *this;
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator++() {
  _node_ptr = _node_ptr->_next;
  return *this;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator++(int) {
  ListIterator ret = ListIterator(_node_ptr);
  ++*this;
  return ret;
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator--() {
  _node_ptr = _node_ptr->_prev;
  return *this;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator--(int) {
  ListIterator ret = ListIterator(_node_ptr);
  --*this;
  return ret;
}

template <typename T>
T& ListIterator<T>::operator*() const {
  return _node_ptr->_value;
}

template <typename T>
T* ListIterator<T>::operator->() const {
  return &this->operator*();
}

template <typename T>
bool operator==(const ListIterator<T>& lhs, const ListIterator<T>& rhs) {
  auto pos1 = lhs._node_ptr;
  auto pos2 = rhs._node_ptr;
  return pos1 == pos2;
}

template <typename T>
bool operator!=(const ListIterator<T>& lhs, const ListIterator<T>& rhs) {
  return !(lhs == rhs);
}

/**
 * @brief double linked list
 */
template <typename T>
class List {
  using node_type = ListNode<T>;
  using node_ptr = node_type*;

 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = T&;
  using iterator = ListIterator<T>;

  List();
  List(std::initializer_list<T> init);
  List(const List& rhs);
  List(List&& rhs) noexcept;
  List& operator=(const List& rhs);
  List& operator=(List&& rhs) noexcept;
  ~List();

  inline reference front();
  inline reference back();

  inline iterator begin();
  inline iterator begin() const;
  inline iterator end();
  inline iterator end() const;

  inline bool empty() const;
  inline size_type size() const;

  void clear();
  iterator insert(iterator pos, const T& value);
  iterator insert(iterator pos, T&& value);
  iterator erase(iterator pos);
  void push_back(const T& value);
  void push_back(T&& value);
  void pop_back();
  void push_front(const T& value);
  void push_front(T&& value);
  void pop_front();

  iterator find(const T& value);
  iterator find(std::function<bool(const T&)> p);

 private:
  void init_impl();
  iterator insert_impl(iterator pos, node_ptr node);
  void copy_impl();
  void release_impl();

  node_ptr _head;
  node_ptr _tail;
  size_type _size;
};

template <typename T>
List<T>::List() {
  init_impl();
}

template <typename T>
List<T>::List(std::initializer_list<T> init) : List() {
  for (const auto& elem : init) {
    push_back(elem);
  }
}

template <typename T>
List<T>::List(const List& rhs) {
  init_impl();

  for (const T& val : rhs) {
    push_back(val);
  }
}

template <typename T>
List<T>::List(List&& rhs) noexcept : _head(rhs._head),
                                     _tail(rhs._tail),
                                     _size(rhs._size) {
  rhs._head = nullptr;
  rhs._tail = nullptr;
  rhs._size = 0;
}

template <typename T>
List<T>& List<T>::operator=(const List& rhs) {
  init_impl();

  for (const T& val : rhs) {
    push_back(val);
  }
}

template <typename T>
List<T>& List<T>::operator=(List&& rhs) noexcept {
  if (this != &rhs) {
    _head = rhs._head;
    _tail = rhs._tail;
    _size = rhs._size;

    rhs._head = nullptr;
    rhs._tail = nullptr;
    rhs._size = 0;
  }
  return *this;
}

template <typename T>
List<T>::~List() {
  release_impl();
}

template <typename T>
typename List<T>::reference List<T>::front() {
  return *begin();
}

template <typename T>
typename List<T>::reference List<T>::back() {
  return *(--end());
}

template <typename T>
typename List<T>::iterator List<T>::begin() {
  return iterator(_head->_next);
}

template <typename T>
typename List<T>::iterator List<T>::begin() const {
  return iterator(_head->_next);
}

template <typename T>
typename List<T>::iterator List<T>::end() {
  return iterator(_tail);
}

template <typename T>
typename List<T>::iterator List<T>::end() const {
  return iterator(_tail);
}

template <typename T>
bool List<T>::empty() const {
  return _size == 0;
}

template <typename T>
typename List<T>::size_type List<T>::size() const {
  return _size;
}

template <typename T>
void List<T>::clear() {
  release_impl();
  init_impl();
}

/**
 * @brief Insert lvalue before pos
 * @param pos Iterator before which the content will be inserted
 * @param value Value to insert
 * @return Iterator pointing to the inserted value
 */
template <typename T>
typename List<T>::iterator List<T>::insert(iterator pos, const T& value) {
  node_ptr new_node = new node_type(value);
  return insert_impl(pos, new_node);
}

/**
 * @brief Insert rvalue before pos
 * @param pos Iterator before which the content will be inserted
 * @param value Value to insert
 * @return Iterator pointing to the inserted value
 */
template <typename T>
typename List<T>::iterator List<T>::insert(iterator pos, T&& value) {
  node_ptr new_node = new node_type(std::move(value));
  return insert_impl(pos, new_node);
}

/**
 * @brief Erase a node at pos
 * @param pos Iterator to the element to remove
 * @return Iterator pointing to the next node of erased node
 */
template <typename T>
typename List<T>::iterator List<T>::erase(iterator pos) {
  // todo: erased node is not freed (to ensure thread safe right now)
  --_size;

  node_ptr next_node = pos._node_ptr->_next;
  node_ptr prev_node = pos._node_ptr->_prev;

  prev_node->_next = next_node;
  next_node->_prev = prev_node;

  return iterator(next_node);
}

template <typename T>
void List<T>::push_back(const T& value) {
  insert(end(), value);
}

template <typename T>
void List<T>::push_back(T&& value) {
  insert(end(), std::move(value));
}

template <typename T>
void List<T>::pop_back() {
  erase(--end());
}

template <typename T>
void List<T>::push_front(const T& value) {
  insert(begin(), value);
}

template <typename T>
void List<T>::push_front(T&& value) {
  insert(begin(), std::move(value));
}

template <typename T>
void List<T>::pop_front() {
  erase(begin());
}

template <typename T>
typename List<T>::iterator List<T>::find(const T& value) {
  for (auto iter = begin(); iter != end(); ++iter) {
    if (*iter == value) {
      return iter;
    }
  }
  return end();
}

template <typename T>
typename List<T>::iterator List<T>::find(std::function<bool(const T&)> p) {
  for (auto iter = begin(); iter != end(); ++iter) {
    if (p(*iter)) {
      return iter;
    }
  }
  return end();
}

template <typename T>
void List<T>::init_impl() {
  _head = new node_type();
  _head->_next = new node_type();
  _head->_next->_prev = _head;
  _tail = _head->_next;
  _size = 0;
}

/**
 * @brief Implementation of insert
 * @param pos Iterator before which the content will be inserted
 * @param new_node Value to insert
 * @return Iterator pointing to the inserted value
 */
template <typename T>
typename List<T>::iterator List<T>::insert_impl(iterator pos,
                                                node_ptr new_node) {
  ++_size;

  node_ptr next_node = pos._node_ptr;
  node_ptr prev_node = pos._node_ptr->_prev;

  prev_node->_next = new_node;
  new_node->_next = next_node;

  next_node->_prev = new_node;
  new_node->_prev = prev_node;

  return ListIterator<T>(new_node);
}

template <typename T>
void List<T>::release_impl() {
  while (_head != nullptr) {
    node_ptr next = _head->_next;
    delete _head;
    _head = next;
  }
}
}

template <typename T>
using List = impl::List<T>;
}

#endif  // VELOCE_DS_LIST_H_

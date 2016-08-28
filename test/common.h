#ifndef MEDIS_TEST_COMMON_H_
#define MEDIS_TEST_COMMON_H_

#include <iostream>

constexpr int thread_count = 4;

class Copyable {
 public:
  Copyable(int id = 10) : _id(id) {}
  ~Copyable() = default;

  int get_id() const { return _id; }

 private:
  int _id;
};

class UnCopyable {
 public:
  UnCopyable(int id = 10) : _id(id) {}
  UnCopyable(const UnCopyable& rhs) = delete;
  UnCopyable(UnCopyable&& rhs) : _id(rhs._id) {
    std::cout << "move constructor" << std::endl;
    rhs._id = 0;
  }
  UnCopyable& operator=(const UnCopyable& rhs) = delete;
  UnCopyable& operator=(UnCopyable&& rhs) {
    std::cout << "move assign operator" << std::endl;
    if (this != &rhs) {
      _id = rhs._id;
      rhs._id = 0;
    }
    return *this;
  }
  ~UnCopyable() = default;

  int get_id() const { return _id; }

 private:
  int _id;
};

#endif  // MEDIS_TEST_COMMON_H_

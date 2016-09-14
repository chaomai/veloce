#ifndef MEDIS_TEST_COMMON_H_
#define MEDIS_TEST_COMMON_H_

#include <chrono>
#include <iostream>

constexpr int thread_count = 4;
constexpr int iteration_count = 5000000;

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
  ~UnCopyable() { std::cout << "deconstructor" << std::endl; }

  int get_id() const { return _id; }

 private:
  int _id;
};

class Timer {
 public:
  Timer() { starting_time = std::chrono::high_resolution_clock::now(); }

  ~Timer() {
    std::cout << "duration: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - starting_time)
                     .count()
              << std::endl;
  }

 private:
  std::chrono::high_resolution_clock::time_point starting_time;
};

#endif  // MEDIS_TEST_COMMON_H_

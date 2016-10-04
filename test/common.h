#ifndef VELOCE_TEST_COMMON_H_
#define VELOCE_TEST_COMMON_H_

#include <chrono>
#include <iostream>
#include <string>

constexpr int thread_count = 4;
constexpr int iteration_count = 500000;

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
  Timer()
      : start_time(std::chrono::high_resolution_clock::now()),
        last_time(start_time) {}
  ~Timer() {
    std::cout << "total: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - start_time)
                     .count()
              << " ms" << std::endl;
  }

  void print_passed(const std::string& msg = "") {
    if (msg.size() > 0) {
      std::cout << msg << ", ";
    }

    auto now = std::chrono::high_resolution_clock::now();
    std::cout << "passed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - last_time)
                     .count()
              << " ms" << std::endl;
    last_time = now;
  }

 private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::chrono::high_resolution_clock::time_point last_time;
};

#endif  // VELOCE_TEST_COMMON_H_

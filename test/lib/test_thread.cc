#include <iostream>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "lib/thread.h"

using std::cout;
using std::endl;
using std::vector;

constexpr int thread_count = 3;

TEST(thread_test, default_constructor) { Thread t; }

TEST(thread_test, constructor) {
  Thread t([](int num) { cout << "hello, thread, " << num << endl; }, 3);
  t.join();
}

TEST(thread_test, hardware_concurrency) {
  Thread t;
  cout << t.hardware_concurrency() << endl;
}

TEST(thread_test, join) {
  int count = 0;
  vector<Thread> vt;

  for (int i = 0; i < thread_count; i++) {
    vt.push_back(Thread([&count]() {
      for (int i = 0; i < 100; i++) {
        ++count;
      }
    }));
  }

  for (int i = 0; i < thread_count; i++) {
    vt[i].join();
  }

  cout << count << endl;
}

TEST(thread_test, copyable_parameter) {
  class Copyable {
   public:
    Copyable(int id = 10) : _id(id) {}
    ~Copyable() = default;

    int get_id() const { return _id; }

   private:
    int _id;
  };

  Thread t([](Copyable obj) { cout << obj.get_id() << endl; }, Copyable());
  t.join();
}

TEST(thread_test, uncopyable_parameter) {
  class UnCopyable {
   public:
    UnCopyable(int id = 10) : _id(id) {}
    UnCopyable(const UnCopyable& rhs) = delete;
    UnCopyable(UnCopyable&& rhs) : _id(rhs._id) {
      cout << "move constructor" << endl;
      rhs._id = 0;
    }
    UnCopyable& operator=(const UnCopyable& rhs) = delete;
    UnCopyable& operator=(UnCopyable&& rhs) {
      cout << "move assign operator" << endl;
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

  UnCopyable unobj(100);
  UnCopyable unobj_ret;

  Thread t(
      [&unobj_ret](UnCopyable obj) {
        cout << obj.get_id() << endl;

        ASSERT_EQ(100, obj.get_id());

        unobj_ret = std::move(obj);

        ASSERT_EQ(0, obj.get_id());
      },
      std::move(unobj));
  t.join();

  ASSERT_EQ(0, unobj.get_id());
  ASSERT_EQ(100, unobj_ret.get_id());
}

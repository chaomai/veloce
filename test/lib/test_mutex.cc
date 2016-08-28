#include <thread>
#include <vector>

#include "gtest/gtest.h"
#include "lib/mutex.h"

#include "common.h"

using std::thread;
using std::vector;

TEST(Mutex_test, default_constructor) { Mutex m; }

TEST(Mutex_test, lock_and_unlock) {
  Mutex m;
  int count = 0;

  vector<thread> vt;
  for (int i = 0; i < 4; ++i) {
    vt.push_back(thread([&m, &count]() {
      for (int i = 0; i < 1000000; ++i) {
        m.lock();
        ++count;
        m.unlock();
      }
    }));
  }

  for (int i = 0; i < 4; ++i) {
    vt[i].join();
  }

  ASSERT_EQ(4000000, count);
}

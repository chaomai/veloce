#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "lib/mutex.h"
#include "lib/thread.h"

#include "common.h"

using std::vector;

TEST(Thread_test, default_constructor) { Thread t; }

TEST(Thread_test, constructor) {
  Thread t([]() {});
  t.join();
}

TEST(Thread_test, move_contructor) {
  // Thread t([]() {});

  // Thread t1(std::move(t));
  // t1.join();
}

TEST(Thread_test, move_assign_operator) {
  // Thread t([]() {});

  // Thread t1;
  // t1 = std::move(t);
  // t1.join();
}

TEST(Thread_test, hardware_concurrency) {
  Thread t;
  ASSERT_EQ(4, t.hardware_concurrency());
}

TEST(Thread_test, single_thead_join) {
  int count = 0;

  Thread t([&count]() {
    for (int i = 0; i < 1000; i++) {
      ++count;
    }
  });

  t.join();
  ASSERT_EQ(1000, count);
}

TEST(Thread_test, multithread_join) {
  // int count = 0;
  // vector<Thread> vt;
  // Mutex m;

  // for (int i = 0; i < thread_count; i++) {
  // vt.push_back(Thread([&m, &count]() {
  // for (int i = 0; i < 1000000; i++) {
  // mutex.lock();
  // ++count;
  // mutex.unlock();
  // }
  // }));
  // }

  // for (int i = 0; i < thread_count; i++) {
  // vt[i].join();
  // }

  // ASSERT_EQ(4000000, count);
}

TEST(Thread_test, copyable_parameter) {
  // Thread t([](Copyable obj) { obj.get_id(); }, Copyable());
  // t.join();
}

TEST(Thread_test, uncopyable_parameter) {
  // UnCopyable unobj(100);
  // UnCopyable unobj_ret;

  // Thread t(
  // [&unobj_ret](UnCopyable obj) {
  // ASSERT_EQ(100, obj.get_id());

  // unobj_ret = std::move(obj);

  // ASSERT_EQ(0, obj.get_id());
  // },
  // std::move(unobj));
  // t.join();

  // ASSERT_EQ(0, unobj.get_id());
  // ASSERT_EQ(100, unobj_ret.get_id());
}

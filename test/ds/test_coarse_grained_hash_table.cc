#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "common.h"
#include "ds/coarse_grained_hash_table.h"
#include "gtest/gtest.h"

using namespace std;
using ds::CoarseGrainedHashTable;

TEST(CoarseGrainedHashTable, default_constructor) {
  CoarseGrainedHashTable<string, string> hash_table;
}

TEST(CoarseGrainedHashTable, initializer_list_constructor) {
  CoarseGrainedHashTable<string, string> hash_table(
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}});

  ASSERT_EQ(3, hash_table.size());

  string ret = hash_table.find("key1").value();
  ASSERT_EQ("value1", ret);

  ret = hash_table.find("key2").value();
  ASSERT_EQ("value2", ret);

  ret = hash_table.find("key3").value();
  ASSERT_EQ("value3", ret);
}

TEST(CoarseGrainedHashTable, subscript) {
  CoarseGrainedHashTable<string, string> hash_table(
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}});

  ASSERT_EQ(3, hash_table.size());
  ASSERT_EQ("value1", hash_table["key1"]);
  ASSERT_EQ("value2", hash_table["key2"]);
  ASSERT_EQ("value3", hash_table["key3"]);
}

TEST(CoarseGrainedHashTable, subscript_with_exception) {
  CoarseGrainedHashTable<string, string> hash_table(
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}});
  ASSERT_THROW(hash_table["key4"], out_of_range);
}

TEST(CoarseGrainedHashTable, basic_insert) {
  CoarseGrainedHashTable<string, string> hash_table;

  hash_table.insert("key1", "value1");
  hash_table.insert("key2", "value2");
  hash_table.insert("key3", "value3");

  ASSERT_EQ(3, hash_table.size());
}

TEST(CoarseGrainedHashTable, single_thread_insert) {
  CoarseGrainedHashTable<int, int> hash_table;

  Timer timer;
  for (int i = 0; i < thread_count * iteration_count; ++i) {
    hash_table.insert(i, i);
  }

  timer.print_passed("insertion");

  ASSERT_EQ(thread_count * iteration_count, hash_table.size());

  for (int i = 0; i < thread_count * iteration_count; ++i) {
    auto ret = hash_table.find(i);

    ASSERT_TRUE(ret);
    ASSERT_EQ(i, ret.value());
  }

  timer.print_passed("query");
}

TEST(CoarseGrainedHashTable, multithread_insert) {
  CoarseGrainedHashTable<int, int> hash_table;

  Timer timer;
  vector<thread> vt;

  for (int i = 0; i < thread_count; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;
        hash_table.insert(key, value);
      }
    }));
  }

  for (auto& t : vt) {
    t.join();
  }

  timer.print_passed("insertion");

  ASSERT_EQ(thread_count * iteration_count, hash_table.size());

  vt.clear();

  for (int i = 0; i < thread_count; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;

        auto ret = hash_table.find(key);
        ASSERT_TRUE(ret);
        ASSERT_EQ(value, ret.value());
      }
    }));
  }

  for (auto& t : vt) {
    t.join();
  }

  timer.print_passed("query");
}

TEST(CoarseGrainedHashTable, erase) {
  CoarseGrainedHashTable<string, string> hash_table(
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}});

  hash_table.erase("key1");
  hash_table.erase("key2");
  hash_table.erase("key3");
}

TEST(CoarseGrainedHashTable, find) {
  CoarseGrainedHashTable<string, string> hash_table(
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}});

  string ret = hash_table.find("key1").value();
  ASSERT_EQ("value1", ret);

  ret = hash_table.find("key2").value();
  ASSERT_EQ("value2", ret);

  ret = hash_table.find("key3").value();
  ASSERT_EQ("value3", ret);
}

TEST(CoarseGrainedHashTable, multithread_insert_erase) {
  CoarseGrainedHashTable<int, int> hash_table;

  Timer timer;
  vector<thread> vt;

  for (int i = 0; i < thread_count; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;
        hash_table.insert(key, value);
      }
    }));
  }

  for (auto& t : vt) {
    t.join();
  }

  timer.print_passed("insertion");

  ASSERT_EQ(thread_count * iteration_count, hash_table.size());

  vt.clear();

  for (int i = 0; i < thread_count / 2; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        hash_table.erase(key);
      }
    }));
  }

  for (int i = thread_count; i < thread_count + thread_count / 2; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;
        hash_table.insert(key, value);
      }
    }));
  }

  for (auto& t : vt) {
    t.join();
  }

  timer.print_passed("insertion and erasing");

  ASSERT_EQ(thread_count * iteration_count, hash_table.size());

  vt.clear();

  for (int i = thread_count / 2; i < thread_count; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;

        auto ret = hash_table.find(key);
        ASSERT_TRUE(ret);
        ASSERT_EQ(value, ret.value());
      }
    }));
  }

  for (int i = thread_count; i < thread_count + thread_count / 2; ++i) {
    vt.push_back(thread([i, &hash_table]() {
      for (int j = 0; j < iteration_count; ++j) {
        int key = i * iteration_count + j;
        int value = i * iteration_count + j;

        auto ret = hash_table.find(key);
        ASSERT_TRUE(ret);
        ASSERT_EQ(value, ret.value());
      }
    }));
  }

  for (auto& t : vt) {
    t.join();
  }

  timer.print_passed("query");
}

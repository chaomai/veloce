#include <iostream>
#include <string>
#include <utility>

#include "common.h"
#include "ds/list.h"
#include "gtest/gtest.h"

using namespace std;
using ds::List;

TEST(List, default_constructor) { List<string> list; }

TEST(List, initializer_list_constructor) {
  List<int> list{1, 2, 3};

  auto iter = list.begin();
  ASSERT_EQ(1, *iter);

  ++iter;
  ASSERT_EQ(2, *iter);

  ++iter;
  ASSERT_EQ(3, *iter);
}

TEST(List, copy_constructor) {
  List<int> list{1, 2, 3};
  List<int> list2(list);

  auto iter = list2.begin();
  ASSERT_EQ(1, *iter);

  ++iter;
  ASSERT_EQ(2, *iter);

  ++iter;
  ASSERT_EQ(3, *iter);
}

TEST(List, move_constructor) {
  List<int> list{1, 2, 3};
  List<int> list2(std::move(list));

  ASSERT_EQ(true, list.empty());
  ASSERT_EQ(0, list.size());

  auto iter = list2.begin();
  ASSERT_EQ(1, *iter);

  ++iter;
  ASSERT_EQ(2, *iter);

  ++iter;
  ASSERT_EQ(3, *iter);
}

TEST(List, iterator) {
  List<int> list{1, 2, 3};

  for (const auto &elem : list) {
    cout << elem << endl;
  }
}

TEST(List, front) {
  List<int> list{1, 2, 3};
  ASSERT_EQ(1, list.front());
}

TEST(List, back) {
  List<int> list{1, 2, 3};
  ASSERT_EQ(3, list.back());
}

TEST(List, empty) {
  List<string> list;
  ASSERT_EQ(true, list.empty());

  list.insert(list.begin(), "value");
  ASSERT_EQ(false, list.empty());
}

TEST(List, size) {
  List<string> list;
  ASSERT_EQ(0, list.size());

  list.insert(list.begin(), "value");
  ASSERT_EQ(1, list.size());
}

TEST(List, clear) {
  List<int> list{1, 2, 3};

  list.clear();
  ASSERT_EQ(true, list.empty());
  ASSERT_EQ(0, list.size());
}

TEST(List, insert_lvalue) {
  List<int> list;

  int v1 = 1;
  int v2 = 2;
  int v3 = 3;

  list.insert(list.begin(), v1);  // h, 1, t
  list.insert(list.end(), v2);    // h, 1, 2, t
  list.insert(list.begin(), v3);  // h, 3, 1, 2, t

  auto iter = list.begin();
  ASSERT_EQ(v3, *iter);

  ++iter;
  ASSERT_EQ(v1, *iter);

  ++iter;
  ASSERT_EQ(v2, *iter);
}

TEST(List, insert_rvalue) {
  List<UnCopyable> list;

  UnCopyable o1(1);
  UnCopyable o2(2);
  UnCopyable o3(3);

  list.insert(list.begin(), std::move(o1));  // h, 1, t
  list.insert(list.end(), std::move(o2));    // h, 1, 2, t
  list.insert(list.begin(), std::move(o3));  // h, 3, 1, 2, t

  auto iter = list.begin();
  ASSERT_EQ(3, iter->get_id());

  ++iter;
  ASSERT_EQ(1, iter->get_id());

  ++iter;
  ASSERT_EQ(2, iter->get_id());
}

TEST(List, erase) {
  List<int> list{1, 2, 3};

  list.erase(list.begin());
  ASSERT_EQ(2, list.front());

  list.erase(--list.end());
  ASSERT_EQ(2, list.back());

  auto iter = list.erase(list.begin());
  ASSERT_EQ(iter, list.end());
  ASSERT_EQ(true, list.empty());
}

TEST(List, push_back_lvalue) {
  List<int> list;

  int v1 = 1;
  int v2 = 2;
  int v3 = 3;

  list.push_back(v1);
  list.push_back(v2);
  list.push_back(v3);

  auto iter = list.begin();
  ASSERT_EQ(v1, *iter);

  ++iter;
  ASSERT_EQ(v2, *iter);

  ++iter;
  ASSERT_EQ(v3, *iter);
}

TEST(List, push_back_rvalue) {
  List<int> list;

  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto iter = list.begin();
  ASSERT_EQ(1, *iter);

  ++iter;
  ASSERT_EQ(2, *iter);

  ++iter;
  ASSERT_EQ(3, *iter);
}

TEST(List, pop_back) {
  List<int> list{1, 2, 3};

  list.pop_back();
  ASSERT_EQ(2, list.back());

  list.pop_back();
  ASSERT_EQ(1, list.back());
}

TEST(List, push_front_lvalue) {
  List<int> list;

  int v1 = 1;
  int v2 = 2;
  int v3 = 3;

  list.push_front(v1);
  list.push_front(v2);
  list.push_front(v3);

  auto iter = list.begin();
  ASSERT_EQ(v3, *iter);

  ++iter;
  ASSERT_EQ(v2, *iter);

  ++iter;
  ASSERT_EQ(v1, *iter);
}

TEST(List, push_front_rvalue) {
  List<int> list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  auto iter = list.begin();
  ASSERT_EQ(3, *iter);

  ++iter;
  ASSERT_EQ(2, *iter);

  ++iter;
  ASSERT_EQ(1, *iter);
}

TEST(List, pop_front) {
  List<int> list{1, 2, 3};

  list.pop_front();
  ASSERT_EQ(2, list.front());

  list.pop_front();
  ASSERT_EQ(3, list.front());
}

TEST(List, massive_insert) {
  List<int> list;

  for (int i = 0; i < iteration_count / 1000; ++i) {
    switch (i % 3) {
      case 0: {
        list.insert(list.begin(), i);
        break;
      }

      case 1: {
        list.push_back(i);
        break;
      }

      case 2: {
        list.push_front(i);
        break;
      }
    }
  }

  ASSERT_EQ(iteration_count / 1000, list.size());

  for (int i = 0; i < iteration_count / 1000; ++i) {
    auto iter = list.find(i);
    ASSERT_NE(list.end(), iter);
    ASSERT_EQ(i, *iter);
  }
}

TEST(List, find) {
  List<int> list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  auto iter = list.find(3);
  ASSERT_EQ(3, *iter);
}

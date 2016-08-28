#include <iostream>
#include <utility>

#include "base/action.h"
#include "gtest/gtest.h"

#include "common.h"

using std::cout;
using std::endl;
using base::make_action;

TEST(action_test, uncopyable_parameter) {
  UnCopyable unobj1(100);
  UnCopyable unobj2(300);

  auto get_id = make_action(
      [](UnCopyable obj1, UnCopyable obj2) {
        cout << obj1.get_id() << endl;
        cout << obj2.get_id() << endl;
      },
      std::move(unobj1), std::move(unobj2));
  get_id.act();
}

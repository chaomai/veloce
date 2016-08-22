#include <iostream>
#include <utility>

#include "base/action.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using base::make_action;

TEST(action_test, uncopyable_parameter) {
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

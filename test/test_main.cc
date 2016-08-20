#include <iostream>

#include "gtest/gtest.h"

using std::cout;
using std::endl;

GTEST_API_ int main(int argc, char **argv) {
  cout << "Running main() from gtest_main.cc" << endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

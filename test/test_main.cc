#include <iostream>

#include "common.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;

GTEST_API_ int main(int argc, char **argv) {
  cout << "Running main() from gtest_main.cc" << endl;
  cout << "test with " << thread_count << " threads, " << iteration_count
       << " iterations" << endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

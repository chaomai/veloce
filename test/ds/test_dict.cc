#include <string>

#include "common.h"
#include "ds/dict.h"
#include "gtest/gtest.h"

using namespace std;
using ds::Dict;

TEST(Dict, default_constructor) { Dict dict; }

TEST(Dict, basic_set_remove_get) {
  Dict dict;

  string key1("key1");
  string key2("key2");
  string key3("key3");
  string key4("key4");
  string key5("key5");

  string string1("string1");
  string string2("string2");
  string string3("string3");
  string string4("string4");
  string string5("string5");

  Item item1{VELOCE_STRING, &string1};
  Item item2{VELOCE_STRING, &string2};
  Item item3{VELOCE_STRING, &string3};
  Item item4{VELOCE_STRING, &string4};
  Item item5{VELOCE_STRING, &string5};

  dict.set(key1, &item1);
  dict.set(key2, &item2);
  dict.set(key3, &item3);
  dict.set(key4, &item4);
  dict.set(key5, &item5);

  ASSERT_EQ(5, dict.size());

  ASSERT_TRUE(dict.exists(key1));
  ASSERT_TRUE(dict.exists(key2));
  ASSERT_TRUE(dict.exists(key3));
  ASSERT_TRUE(dict.exists(key4));
  ASSERT_TRUE(dict.exists(key5));

  dict.remove(key4);
  dict.remove(key5);

  ASSERT_TRUE(dict.exists(key1));
  ASSERT_TRUE(dict.exists(key2));
  ASSERT_TRUE(dict.exists(key3));
  ASSERT_FALSE(dict.exists(key4));
  ASSERT_FALSE(dict.exists(key5));

  ASSERT_EQ(string1, *reinterpret_cast<string*>(dict.get(key1)->_value_ptr));
  ASSERT_EQ(string2, *reinterpret_cast<string*>(dict.get(key2)->_value_ptr));
  ASSERT_EQ(string3, *reinterpret_cast<string*>(dict.get(key3)->_value_ptr));
}

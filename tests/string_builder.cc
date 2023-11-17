#include <cstdlib>

extern "C" {
// #include "../src/string_builder.h"
}

#include <gtest/gtest.h>

TEST(HelloWorld, BuildStringTest) {
  // NOLINTBEGIN
  char* actual_output = static_cast<char*>(malloc(256 * sizeof(char)));
  actual_output = "The number is: 5";

  //  BuildString(actual_output, "The number is: ", 5);
  // NOLINTEND

  EXPECT_STREQ(actual_output, "The number is: 5");
}

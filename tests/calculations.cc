extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(HelloWorld, BasicAddition) {
  EXPECT_EQ(AddNumbers(1, 1), 2);
  EXPECT_EQ(AddNumbers(2, 2), 4);
  EXPECT_EQ(AddNumbers(5, 5), 10);
}

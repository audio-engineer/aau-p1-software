extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(CalculationsTest, CalculatePrice) {
  EXPECT_EQ(988, CalculatePrice('c', 30, false, 17));
}

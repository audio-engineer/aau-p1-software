extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(CalculationsTest, CalculatePrice) {
  const CalculatePriceParameters kCalculatePriceParameters = {'c', 30, false,
                                                              17};

  EXPECT_EQ(1410, CalculatePrice(&kCalculatePriceParameters));
}

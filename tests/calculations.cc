extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(CalculationsTest, CalculatePrice) {
  const CalculatePriceParameters kParameters01 = {'t', 30, false, 0};
  const CalculatePriceParameters kParameters02 = {'t', 100, false, 10};
  const CalculatePriceParameters kParameters03 = {'t', 150, true, 0};
  const CalculatePriceParameters kParameters04 = {'t', 400, true, 0};

  const CalculatePriceParameters kParameters05 = {'c', 30, false, 17};
  const CalculatePriceParameters kParameters06 = {'c', 100, false, 0};
  const CalculatePriceParameters kParameters07 = {'c', 200, true, 33};
  const CalculatePriceParameters kParameters08 = {'e', 50, false, 10};

  const CalculatePriceParameters kParameters09 = {'b', 320, true, 15};
  const CalculatePriceParameters kParameters10 = {'w', 40, false, 0};

  EXPECT_EQ(900, CalculatePrice(&kParameters01));
  EXPECT_EQ(2730, CalculatePrice(&kParameters02));
  EXPECT_EQ(1294, CalculatePrice(&kParameters03));  // same as in our report
  EXPECT_EQ(1714, CalculatePrice(&kParameters04));

  EXPECT_EQ(1838, CalculatePrice(&kParameters05));  // same as in our report
  EXPECT_EQ(6450, CalculatePrice(&kParameters06));
  EXPECT_EQ(4243, CalculatePrice(&kParameters07));
  EXPECT_EQ(1683, CalculatePrice(&kParameters08));

  EXPECT_EQ(100, CalculatePrice(&kParameters09));
  EXPECT_EQ(0, CalculatePrice(&kParameters10));
}

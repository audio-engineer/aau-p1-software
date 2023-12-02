extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(CalculationsTest, CalculatePrice01) {
  const CalculatePriceParameters kParameters = {'t', 30, false, 0};
  EXPECT_EQ(900, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice02) {
  const CalculatePriceParameters kParameters = {'t', 100, false, 10};
  EXPECT_EQ(2730, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice03) {
  const CalculatePriceParameters kParameters = {'t', 150, true, 0};
  EXPECT_EQ(1294, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice04) {
  const CalculatePriceParameters kParameters = {'t', 400, true, 0};
  EXPECT_EQ(1714, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice05) {
  const CalculatePriceParameters kParameters = {'c', 30, false, 17};
  EXPECT_EQ(1838, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice06) {
  const CalculatePriceParameters kParameters = {'c', 100, false, 0};
  EXPECT_EQ(6450, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice07) {
  const CalculatePriceParameters kParameters = {'c', 200, true, 33};
  EXPECT_EQ(4243, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice08) {
  const CalculatePriceParameters kParameters = {'e', 50, false, 10};
  EXPECT_EQ(1683, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice09) {
  const CalculatePriceParameters kParameters = {'b', 320, true, 15};
  EXPECT_EQ(100, CalculatePrice(&kParameters));
}

TEST(CalculationsTest, CalculatePrice10) {
  const CalculatePriceParameters kParameters = {'w', 40, false, 0};
  EXPECT_EQ(0, CalculatePrice(&kParameters));
}

extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

TEST(CalculatePriceTest, Train30kmAdult0) {
  const CalculatePriceParameters kParameters = {kTrain, 30, false, 0};
  EXPECT_EQ(900, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Train100kmAdult10) {
  const CalculatePriceParameters kParameters = {kTrain, 100, false, 10};
  EXPECT_EQ(2730, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Train150kmYoung0) {
  const CalculatePriceParameters kParameters = {kTrain, 150, true, 0};
  EXPECT_EQ(1294, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Train400kmYoung0) {
  const CalculatePriceParameters kParameters = {kTrain, 400, true, 0};
  EXPECT_EQ(1714, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Car30kmAdult17) {
  const CalculatePriceParameters kParameters = {kCar, 30, false, 17};
  EXPECT_EQ(1838, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Car100kmAdult0) {
  const CalculatePriceParameters kParameters = {kCar, 100, false, 0};
  EXPECT_EQ(6450, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Car200kmYoung33) {
  const CalculatePriceParameters kParameters = {kCar, 200, true, 33};
  EXPECT_EQ(4243, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Ev50kmAdult10) {
  const CalculatePriceParameters kParameters = {kEv, 50, false, 10};
  EXPECT_EQ(1683, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Bike320kmYoung15) {
  const CalculatePriceParameters kParameters = {kBike, 320, true, 15};
  EXPECT_EQ(100, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Walk40kmAdult0) {
  const CalculatePriceParameters kParameters = {kWalk, 40, false, 0};
  EXPECT_EQ(0, CalculatePrice(&kParameters));
}

TEST(CalculateTimeTest, Car200km) {
  const CalculateTimeParameters kParameters = {kCar, 230, false};
  EXPECT_STREQ("02:16", CalculateTime(&kParameters));
}

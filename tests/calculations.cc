extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

// NOLINTBEGIN

TEST(CalculateDistanceTest, HoejeTaastrupToTaastrup) {
  const CalculateDistanceParameters kParameters = {55648873, 12269583, 55652550,
                                                   12302133};
  EXPECT_EQ(2.08, CalculateDistance(&kParameters));
}

TEST(CalculateDistanceTest, DuplicateStationInArray) {
  const CalculateDistanceParameters kParameters = {55648873, 12269583, 55648873,
                                                   12269583};
  EXPECT_EQ(0, CalculateDistance(&kParameters));
}

TEST(CalculatePriceTest, Train3kmAdult0) {
  const CalculatePriceParameters kParameters = {kTrain, 3, false, 0};
  EXPECT_EQ(390, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Train100kmAdult10) {
  const CalculatePriceParameters kParameters = {kTrain, 100, false, 10};
  EXPECT_EQ(2880, CalculatePrice(&kParameters));
}

TEST(CalculatePriceTest, Train150kmYoung0) {
  const CalculatePriceParameters kParameters = {kTrain, 150, true, 0};
  EXPECT_EQ(1324, CalculatePrice(&kParameters));
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
  const CalculateTimeParameters kParameters = {kCar, 230, "NULL", "NULL"};
  EXPECT_STREQ("02:16", CalculateTime(&kParameters));
}

TEST(CalculateTimeTest, Ev354km) {
  const CalculateTimeParameters kParameters = {kEv, 354, "NULL", "NULL"};
  EXPECT_STREQ("03:18", CalculateTime(&kParameters));
}

TEST(CalculateTimeTest, Bike40km) {
  const CalculateTimeParameters kParameters = {kBike, 40, "NULL", "NULL"};
  EXPECT_STREQ("02:17", CalculateTime(&kParameters));
}

TEST(CalculateTimeTest, Walk20km) {
  const CalculateTimeParameters kParameters = {kWalk, 20, "NULL", "NULL"};
  EXPECT_STREQ("04:20", CalculateTime(&kParameters));
}

TEST(CalculateTimeTest, Train1020to1343) {
  const CalculateTimeParameters kParameters = {kTrain, 0, "10:20", "13:43"};
  EXPECT_STREQ("03:23", CalculateTime(&kParameters));
}

TEST(CalculateTimeTest, Bus2346to0013) {
  const CalculateTimeParameters kParameters = {kBus, 0, "23:46", "00:13"};
  EXPECT_STREQ("00:27", CalculateTime(&kParameters));
}

TEST(CalculateSecondTimeTest, 1323plus0105) {
  const CalculateSecondTimeParameters kParameters = {"13:23", "01:05", false};
  EXPECT_STREQ("14:28", CalculateSecondTime(&kParameters));
}

TEST(CalculateSecondTimeTest, 1323minus0105) {
  const CalculateSecondTimeParameters kParameters = {"13:23", "01:05", true};
  EXPECT_STREQ("12:18", CalculateSecondTime(&kParameters));
}

TEST(CalculateSecondTimeTest, 2345plus0232) {
  const CalculateSecondTimeParameters kParameters = {"23:45", "2:32", false};
  EXPECT_STREQ("02:17", CalculateSecondTime(&kParameters));
}

TEST(CalculateSecondTimeTest, 0116minus0253) {
  const CalculateSecondTimeParameters kParameters = {"01:16", "02:53", true};
  EXPECT_STREQ("22:23", CalculateSecondTime(&kParameters));
}

TEST(CalculateCo2Test, train200km) {
  const CalculateCo2Parameters kParameters = {kTrain, 200, 0};
  EXPECT_EQ(264, CalculateCo2(&kParameters));
}

TEST(CalculateCo2Test, strain50km) {
  const CalculateCo2Parameters kParameters = {kSTrain, 50, 0};
  EXPECT_EQ(28, CalculateCo2(&kParameters));
}

TEST(CalculateCo2Test, car200km15) {
  const CalculateCo2Parameters kParameters = {kCar, 200, 15};
  EXPECT_EQ(1429, CalculateCo2(&kParameters));
}

TEST(CalculateCo2Test, car200km0) {
  const CalculateCo2Parameters kParameters = {kCar, 200, 0};
  EXPECT_EQ(2144, CalculateCo2(&kParameters));
}

TEST(CalculateCo2Test, ev10000km) {
  const CalculateCo2Parameters kParameters = {kEv, 10000, 0};
  EXPECT_EQ(0, CalculateCo2(&kParameters));
}

// NOLINTEND

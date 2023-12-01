extern "C" {
#include "../src/calculations.h"
}

#include <gtest/gtest.h>

const CalculatePriceParameters kCalculatePriceParameters = {'c', 30, false, 17};
const auto kCalculatePriceParametersAddress = &kCalculatePriceParameters;
const int kCalculatedPrice = CalculatePrice(kCalculatePriceParametersAddress);

TEST(CalculationsTest, CalculatePrice) { EXPECT_EQ(988, kCalculatedPrice); }

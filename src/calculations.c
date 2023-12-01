#include "calculations.h"

#include <math.h>
#include <stdio.h>

double CalculateDistance(
    const CalculateDistanceParameters* const calculate_distance_parameters) {
  double latitude_first = calculate_distance_parameters->latitude_first;
  double longitude_first = calculate_distance_parameters->longitude_first;
  double latitude_second = calculate_distance_parameters->latitude_second;
  double longitude_second = calculate_distance_parameters->longitude_second;

  // Convert latitude and longitude from degrees to radians
  latitude_first /= kMillion;
  latitude_second /= kMillion;
  longitude_first /= kMillion;
  longitude_second /= kMillion;

  latitude_first *= kRadians;
  longitude_first *= kRadians;
  latitude_second *= kRadians;
  longitude_second *= kRadians;

  // Calculate differences
  const double kDeltaLatitude = latitude_second - latitude_first;
  const double kDeltaLongitude = longitude_second - longitude_first;

  // Haversine formula
  const double kHaversineA = sin(kDeltaLatitude / 2) * sin(kDeltaLatitude / 2) +
                             cos(latitude_first) * cos(latitude_second) *
                                 sin(kDeltaLongitude / 2) *
                                 sin(kDeltaLongitude / 2);
  const double kHaversineC =
      2 * atan2(sqrt(kHaversineA), sqrt(1 - kHaversineA));

  return kEarthRadius * kHaversineC;
}

int CalculatePrice(
    const CalculatePriceParameters* const calculate_price_parameters) {
  const char kType = calculate_price_parameters->type;
  const double kDistance = calculate_price_parameters->distance;
  const bool kIsUng = calculate_price_parameters->is_ung;
  int fuel_efficiency = calculate_price_parameters->fuel_efficiency;

  int price = 0;
  int zones = 0;

  switch (kType) {
    case kTrain:
      zones = (int)kDistance / kZoneSizeAverage;

      if (zones > kTrainPriceTableSize) {
        zones = kTrainPriceTableSize;
      }

      price = kTrainPriceTable[zones];

      if (kIsUng) {
        price = (price - kUngNormalPrice) / 2 + kUngExtraPrice;
      }

      break;

    case kCar:
      if (fuel_efficiency == 0) {
        price = ((int)kDistance / 60) * 25 * 40;
      } else {
        if (fuel_efficiency == -1) {
          fuel_efficiency = kFuelEfficiencyAverage;
        }

        price = ((int)kDistance / fuel_efficiency) * kGasPriceAverage * 40;
      }

      price += kCarMaintenancePriceAverage;

      break;

    case kBike:
      price = kBikePriceAverage;

      break;

    case kWalk:
      price = 0;

      break;

    default:
      perror("Invalid type");

      return 0;
  }

  return price;
}

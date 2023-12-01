#include "calculations.h"

#include <stdio.h>
#include <math.h>

double CalculateDistance(
    CalculateDistanceParameters* const calculate_distance_parameters) {
  double latitude_first = calculate_distance_parameters->latitude_first;
  double longitude_first = calculate_distance_parameters->longitude_first;
  double latitude_second = calculate_distance_parameters->latitude_second;
  double longitude_second = calculate_distance_parameters->longitude_second;

  // Convert latitude and longitude from degrees to radians
  latitude_first /= kMillion;
  latitude_second /= kMillion;
  longitude_first /= kMillion;
  longitude_second /= kMillion;

  latitude_first = latitude_first * kRadians;
  longitude_first = longitude_first * kRadians;
  latitude_second = latitude_second * kRadians;
  longitude_second = longitude_second * kRadians;

  // Calculate differences
  double deltalat = latitude_second - latitude_first;
  double deltalon = longitude_second - longitude_first;

  // Haversine formula
  double haversine_a = sin(deltalat / 2) * sin(deltalat / 2) +
                       cos(latitude_first) * cos(latitude_second) *
                           sin(deltalon / 2) * sin(deltalon / 2);
  double haversine_c = 2 * atan2(sqrt(haversine_a), sqrt(1 - haversine_a));
  double distance = kEarthRadius * haversine_c;

  return distance;
}

int CalculatePrice(CalculatePriceParameters* const calculate_price_parameters) {
  char type = calculate_price_parameters->type;
  double distance = calculate_price_parameters->distance;
  bool is_ung = calculate_price_parameters->is_ung;
  int fuel_efficiency = calculate_price_parameters->fuel_efficiency;

  int price = 0;
  int zones = 0;

  switch (type) {
    case kTrain:
      zones = (int)distance / kZoneSizeAverage;

      if (zones > kTrainPriceTableSize) {
        zones = kTrainPriceTableSize;
      }

      price = kTrainPriceTable[zones];

      if (is_ung) {
        price = (price - kUngNormalprice) / 2 + kUngExtraPrice;
      }
      break;

    case kCar:
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

      return price;
  }

  if (type == 'c') {
    if (fuel_efficiency == 0) {
      price = (distance / 60) * 25 * 40;
    } else {
      if (fuel_efficiency == -1) {
        fuel_efficiency = kFuelEfficiencyAverage;
      }
      price = (distance / fuel_efficiency) * kGasPriceAverage * 40;
    }
    price += kCarMaintenancePriceAverage;
  }

  return price;
}

/*
int CalculateTime()
int CalculateCO2()
int CalculateComfort()
*/

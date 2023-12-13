#include "calculations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum CalculationConstants {
  kAverageZoneSize = 6,
  kTrainPriceTableSize = 50,
  kUngNormalPrice = 2487,
  kUngExtraPrice = 663,

  kAverageFuelEfficiency = 10,
  kEvAverageEfficiency = 60,

  kEvPricePerKm = 25,
  kGasPricePerKm = 14,

  kCarAverageMaintenancePrice = 850,
  kBikeAverageMaintenancePrice = 100,

  kDaysInWorkMonth = 20,

  kEarthRadius = 6371000,

  kTimeBufferSize = 6,
  kTimeMinutesInHour = 60,

  kCarSpeedCity = 35,
  kCarSpeedHighway = 120,

  kCO2Strain = 14,
  kCO2Train = 33,
  kCO2Bus = 10
};

const double kMillion = 1000000.0;
const double kRadians = M_PI / 180.0;

const double kDistanceToHighway = 17.5;
const double kWalkSpeed = 4.6;
const double kBikeSpeed = 17.5;

const int kTrainPriceTable[kTrainPriceTableSize] = {
    0,    390,  390,  540,  720,  900,  1050, 1200, 1350, 1500,
    1650, 1830, 2040, 2190, 2400, 2580, 2730, 2880, 2970, 3120,
    3210, 3330, 3390, 3540, 3660, 3750, 3810, 3870, 3960, 3990,
    4020, 4050, 4080, 4110, 4140, 4170, 4200, 4230, 4260, 4290,
    4320, 4350, 4380, 4410, 4440, 4470, 4500, 4530, 4560, 4590};

double CalculateDistance(
    const CalculateDistanceParameters* const calculate_distance_parameters) {
  double latitude_first = calculate_distance_parameters->kLatitudeFirst;
  double longitude_first = calculate_distance_parameters->kLongitudeFirst;
  double latitude_second = calculate_distance_parameters->kLatitudeSecond;
  double longitude_second = calculate_distance_parameters->kLongitudeSecond;

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
  const ModeOfTransport kModeOfTransport =
      calculate_price_parameters->kModeOfTransport;
  const int kDistance = calculate_price_parameters->kTripDistance;
  const bool kIsUng = calculate_price_parameters->kIsUng;
  int fuel_efficiency = calculate_price_parameters->kFuelEfficiency;

  int zones = 0;
  int price = 0;

  switch (kModeOfTransport) {
    case kTrain:
    case kSTrain:
    case kBus:
      zones = kDistance / kAverageZoneSize;

      if (zones > kTrainPriceTableSize - 1) {
        zones = kTrainPriceTableSize - 1;
      }

      price = kTrainPriceTable[zones];

      if (kIsUng) {
        price = (price - kUngNormalPrice) / 2 + kUngExtraPrice;
      }

      break;

    case kCar:
      if (!fuel_efficiency) {
        fuel_efficiency = kAverageFuelEfficiency;
      }

      price =
          (kDistance * kGasPricePerKm * kDaysInWorkMonth * 2) / fuel_efficiency;

      price += kCarAverageMaintenancePrice;

      break;

    case kEv:
      price = (kDistance * kEvPricePerKm * kDaysInWorkMonth * 2) /
              kEvAverageEfficiency;
      price += kCarAverageMaintenancePrice;

      break;

    case kBike:
      price = kBikeAverageMaintenancePrice;

      break;

    case kWalk:
      break;

    default:
      perror("Invalid type");

      return EXIT_FAILURE;
  }

  return price;
}

char* CalculateTime(
    const CalculateTimeParameters* const calculate_time_parameters) {
  const ModeOfTransport kModeOfTransport =
      calculate_time_parameters->kModeOfTransport;
  const int kDistance = calculate_time_parameters->kTripDistance;
  // const bool kArrival = calculate_time_parameters->kArrival;

  char* time = malloc(kTimeBufferSize * sizeof(char));
  if (!time) {
    perror("Failed to allocate memory");
    return NULL;
  }

  int hrs = 0;
  int mins = 0;

  double car_highway_time = 0;
  double car_city_time = 0;

  switch (kModeOfTransport) {
    case kCar:
    case kEv:

      car_highway_time = (kDistance - kDistanceToHighway) / kCarSpeedHighway;
      car_city_time = kDistanceToHighway / kCarSpeedCity;

      hrs = (int)(car_highway_time + car_city_time);
      mins =
          (int)((car_highway_time + car_city_time - hrs) * kTimeMinutesInHour);

      break;

    case kBike:

      hrs = (int)(kDistance / kBikeSpeed);
      mins = (int)((kDistance / kBikeSpeed - hrs) * kTimeMinutesInHour);

      break;

    case kWalk:

      hrs = (int)(kDistance / kWalkSpeed);
      mins = (int)((kDistance / kWalkSpeed - hrs) * kTimeMinutesInHour);

      break;

    default:
      perror("Invalid type");

      return NULL;
  }

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  sprintf(time, "%02d:%02d", hrs, mins);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  return time;
}

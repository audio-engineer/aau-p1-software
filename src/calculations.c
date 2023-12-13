#include "calculations.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
  kTimeHoursInDay = 24,

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
  const char* k_departure_time = calculate_time_parameters->kDepartureTime;
  const char* k_arrival_time = calculate_time_parameters->kArrivalTime;

  int hrs = 0;
  int mins = 0;

  int hrs_first = 0;
  int mins_first = 0;
  int hrs_second = 0;
  int mins_second = 0;

  double car_highway_time = 0;
  double car_city_time = 0;

  switch (kModeOfTransport) {
    case kTrain:
    case kSTrain:
    case kBus:
      // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      sscanf(k_departure_time, "%d:%d", &hrs_first, &mins_first);
      sscanf(k_arrival_time, "%d:%d", &hrs_second, &mins_second);
      // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

      hrs = hrs_second - hrs_first;
      mins = mins_second - mins_first;

      if (mins < 0) {
        hrs--;
        mins += kTimeMinutesInHour;
      }
      if (hrs < 0) {
        hrs += kTimeHoursInDay;
      }

      break;

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

  char time[kTimeBufferSize] = {0};
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  sprintf(time, "%02d:%02d", hrs, mins);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  char* time_copy = calloc(strlen(time), sizeof(char));
  if (!time_copy) {
    perror("Failed to allocate memory");
    return NULL;
  }
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.strcpy)
  strcpy(time_copy, time);
  // NOLINTEND(clang-analyzer-security.insecureAPI.strcpy)

  return time_copy;
}

char* CalculateSecondTime(const CalculateSecondTimeParameters* const
                              calculate_second_time_parameters) {
  const char* k_time = calculate_second_time_parameters->kTime;
  const char* k_time_difference =
      calculate_second_time_parameters->kTimeDifference;
  const bool kArrival = calculate_second_time_parameters->kArrival;

  int hrs_first = 0;
  int mins_first = 0;
  int hrs_second = 0;
  int mins_second = 0;
  int hrs_difference = 0;
  int mins_difference = 0;

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  sscanf(k_time, "%d:%d", &hrs_first, &mins_first);
  sscanf(k_time_difference, "%d:%d", &hrs_difference, &mins_difference);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  if (!kArrival) {
    hrs_second = hrs_first + hrs_difference;
    mins_second = mins_first + mins_difference;
  } else {
    hrs_second = hrs_first - hrs_difference;
    mins_second = mins_first - mins_difference;
  }

  if (mins_second >= kTimeMinutesInHour) {
    hrs_second++;
    mins_second -= kTimeMinutesInHour;
  }
  if (hrs_second >= kTimeHoursInDay) {
    hrs_second -= kTimeHoursInDay;
  }
  if (mins_second < 0) {
    hrs_second--;
    mins_second += kTimeMinutesInHour;
  }
  if (hrs_second < 0) {
    hrs_second += kTimeHoursInDay;
  }

  char time[kTimeBufferSize] = {0};
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  sprintf(time, "%02d:%02d", hrs_second, mins_second);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  char* time_copy = calloc(strlen(time), sizeof(char));
  if (!time_copy) {
    perror("Failed to allocate memory");
    return NULL;
  }
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.strcpy)
  strcpy(time_copy, time);
  // NOLINTEND(clang-analyzer-security.insecureAPI.strcpy)

  return time_copy;
}

int CalculateCo2(const CalculateCo2Parameters* calculate_co2_parameters) {
  const ModeOfTransport kModeOfTransport =
      calculate_co2_parameters->kModeOfTransport;
  const int kDistance = calculate_co2_parameters->kTripDistance;

  int co2 = 0;

  switch (kModeOfTransport) {
    case kTrain:
      co2 = kCO2Train * kDistance;

      break;

    case kSTrain:
      co2 = kCO2Strain * kDistance;

      break;

    case kBus:
      co2 = kCO2Bus * kDistance;

      break;

    case kCar:
      // make stuff yeet
    case kEv:
      // here too woo
    case kBike:
    case kWalk:
      break;

    default:
      perror("Invalid type");

      return EXIT_FAILURE;
  }

  return co2;
}

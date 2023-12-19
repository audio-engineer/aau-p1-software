#ifndef AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_
#define AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

typedef enum ModeOfTransport {
  kTrain = 'T',
  kSTrain = 'S',
  kBus = 'B',
  kCar = 'C',
  kEv = 'E',
  kBike = 'I',
  kWalk = 'W'
} ModeOfTransport;

typedef struct CalculateDistanceParameters {
  const double kLatitudeFirst;
  const double kLongitudeFirst;
  const double kLatitudeSecond;
  const double kLongitudeSecond;
} CalculateDistanceParameters;

typedef struct CalculatePriceParameters {
  const char kModeOfTransport;
  const double kTripDistance;
  const bool kIsUng;
  const int kFuelEfficiency;
} CalculatePriceParameters;

typedef struct CalculateTimeParameters {
  const char kModeOfTransport;
  const double kTripDistance;
  const char* departure_time;
  const char* arrival_time;
} CalculateTimeParameters;

typedef struct CalculateSecondTimeParameters {
  const char* time;
  const char* time_difference;
  const bool kArrival;
} CalculateSecondTimeParameters;

typedef struct CalculateCo2Parameters {
  const char kModeOfTransport;
  const double kTripDistance;
  const int kFuelEfficiency;
} CalculateCo2Parameters;

double CalculateDistance(
    const CalculateDistanceParameters* calculate_distance_parameters);

double CalculatePrice(
    const CalculatePriceParameters* calculate_price_parameters);

double CalculateTime(const CalculateTimeParameters* calculate_time_parameters);

char* CalculateSecondTime(
    const CalculateSecondTimeParameters* calculate_second_time_parameter);

double CalculateCo2(const CalculateCo2Parameters* calculate_co2_parameters);

// int CalculateComfort(); TODO(Optional): Add comfort as an attribute

#endif  // AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

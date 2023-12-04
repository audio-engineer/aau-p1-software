#ifndef AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_
#define AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

typedef struct CalculateDistanceParameters {
  const double kLatitudeFirst;
  const double kLongitudeFirst;
  const double kLatitudeSecond;
  const double kLongitudeSecond;
} CalculateDistanceParameters;

double CalculateDistance(
    const CalculateDistanceParameters* calculate_distance_parameters);

typedef enum ModeOfTransport {
  kTrain = 't',
  kCar = 'c',
  kEv = 'e',
  kBike = 'b',
  kWalk = 'w'
} ModeOfTransport;

typedef struct CalculatePriceParameters {
  const ModeOfTransport kModeOfTransport;
  const int kTripDistance;
  const bool kIsUng;
  const int kFuelEfficiency;
} CalculatePriceParameters;

int CalculatePrice(const CalculatePriceParameters* calculate_price_parameters);

char CalculateTime(const char* departure_time, const char* arrival_time);

int CalculateCo2();

int CalculateComfort();

#endif  // AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

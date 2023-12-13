#ifndef AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_
#define AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

typedef enum ModeOfTransport {
  kTrain = 't',
  kSTrain = 's',
  kBus = 'b',
  kCar = 'c',
  kEv = 'e',
  kBike = 'i',
  kWalk = 'w'
} ModeOfTransport;

typedef struct CalculateDistanceParameters {
  const double kLatitudeFirst;
  const double kLongitudeFirst;
  const double kLatitudeSecond;
  const double kLongitudeSecond;
} CalculateDistanceParameters;

typedef struct CalculatePriceParameters {
  const ModeOfTransport kModeOfTransport;
  const int kTripDistance;
  const bool kIsUng;
  const int kFuelEfficiency;
} CalculatePriceParameters;

typedef struct CalculateTimeParameters {
  const ModeOfTransport kModeOfTransport;
  const int kTripDistance;
  const bool kArrival;
} CalculateTimeParameters;

typedef struct CalculateTimeDifferenceParameters {
  const char* kDepartureTime;
  const char* kArrivalTime;
} CalculateTimeDifferenceParameters;

typedef struct CalculateCo2Parameters {
  const ModeOfTransport kModeOfTransport;
  const int kTripDistance;
} CalculateCo2Parameters;

double CalculateDistance(
    const CalculateDistanceParameters* calculate_distance_parameters);

int CalculatePrice(const CalculatePriceParameters* calculate_price_parameters);

char* CalculateTime(const CalculateTimeParameters* calculate_time_parameters);

char* CalcualteTimeDifference(const CalculateTimeDifferenceParameters*
                                  calcualte_time_difference_parameters);

int CalculateCo2(const CalculateCo2Parameters* calculate_co2_parameters);

// int CalculateComfort(); TODO(Optional): Add comfort as an attribute

#endif  // AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

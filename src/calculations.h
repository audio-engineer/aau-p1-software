#ifndef AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_
#define AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

#include <math.h>

enum { kEarthRadius = 6371000 };

double const kMillion = 1000000.0;
double const kRadians = M_PI / 180.0;

typedef struct CalculateDistanceParameters {
  double latitude_first;
  double longitude_first;
  double latitude_second;
  double longitude_second;
} CalculateDistanceParameters;

double CalculateDistance(
    CalculateDistanceParameters* calculate_distance_parameters);

#endif  // AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

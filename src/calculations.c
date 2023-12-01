#include "calculations.h"

#include <stdio.h>

#define EarthRadius 6371  // Earth radius in kilometers
double const kMillion = 1000000.0;
double const kPi = 3.14159265358979323846;
double const kRadians = kPi / 180.0;

double CalcDist(double lat1, double lon1, double lat2, double lon2) {
  // Convert latitude and longitude from degrees to radians

  lat1 /= kMillion;
  lat2 /= kMillion;
  lon1 /= kMillion;
  lon2 /= kMillion;

  lat1 = lat1 * kRadians;
  lon1 = lon1 * kRadians;
  lat2 = lat2 * kRadians;
  lon2 = lon2 * kRadians;

  // Calculate differences
  double deltalat = lat2 - lat1;
  double deltalon = lon2 - lon1;

  // Haversine formula
  double a = sin(deltalat / 2) * sin(deltalat / 2) +
             cos(lat1) * cos(lat2) * sin(deltalon / 2) * sin(deltalon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EarthRadius * c;

  return distance;
}

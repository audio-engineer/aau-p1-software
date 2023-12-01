#ifndef AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_
#define AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

#include <math.h>

typedef struct CalculatePriceParameters {
  char type;
  double distance;
  bool is_ung;
  int fuel_efficiency;
} CalculatePriceParameters;

enum CalculationConstants {
  kZoneSizeAverage = 6,
  kGasPriceAverage = 14,
  kFuelEfficiencyAverage = 10,
  kElectricityPriceAverage = 25,
  kBikePriceAverage = 100,
  kCarMaintenancePriceAverage = 850,
  kTrainPriceTableSize = 50,
  kUngNormalprice = 2487,
  kUngExtraPrice = 663
};

enum TravelTypeCase { kTrain = 't', kCar = 'c', kBike = 'b', kWalk = 'w' };

enum { kEarthRadius = 6371000 };

double const kMillion = 1000000.0;
double const kRadians = M_PI / 180.0;

typedef struct CalculateDistanceParameters {
  double latitude_first;
  double longitude_first;
  double latitude_second;
  double longitude_second;
} CalculateDistanceParameters;

const int kTrainPriceTable[kTrainPriceTableSize] = {
    0,    390,  390,  540,  720,  900,  1050, 1200, 1350, 1500,
    1650, 1830, 2040, 2190, 2400, 2580, 2730, 2880, 2970, 3120,
    3210, 3330, 3390, 3540, 3660, 3750, 3810, 3870, 3960, 3990,
    4020, 4050, 4080, 4110, 4140, 4170, 4200, 4230, 4260, 4290,
    4320, 4350, 4380, 4410, 4440, 4470, 4500, 4530, 4560, 4590};

double CalculateDistance(
    CalculateDistanceParameters* calculate_distance_parameters);

int CalculatePrice(CalculatePriceParameters* calculate_price_parameters);

char CalculateTime(char departure_time[], char arrival_time[]);

int CalculateCO2();

int CalculateComfort();

#endif  // AAU_P1_SOFTWARE_SRC_CALCULATIONS_H_

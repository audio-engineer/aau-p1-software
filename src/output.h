#ifndef AAU_P1_SOFTWARE_SRC_OUTPUT_H_
#define AAU_P1_SOFTWARE_SRC_OUTPUT_H_

#include "evaluate.h"
#include "trip.h"

enum OutputConstants { kBaseTen = 10, kMaxAmountRoutesToPrint = 5 };

void Output(TripData data_arr[], TripScore score_arr[], size_t size,
            Trips* trips);

void PrintTripScoresAndData(TripData data_arr[], TripScore score_arr[],
                            size_t size);

bool IsInteger(const char* str);

bool IsCharacter(const char* str);

void PrintRouteDetails(Trips* trips, long choice, TripScore score_arr[], size_t number_trips);

#endif  // AAU_P1_SOFTWARE_SRC_OUTPUT_H_

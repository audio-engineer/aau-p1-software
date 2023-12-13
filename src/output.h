#ifndef AAU_P1_SOFTWARE_SRC_OUTPUT_H_
#define AAU_P1_SOFTWARE_SRC_OUTPUT_H_
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "evaluate.h"

enum Constants { kBaseTen = 10, kAmountRoutesToPrint = 5 };

void Output(TripData data_arr[], TripScore score_arr[], size_t size);
void PrintTripScoresAndData(TripData data_arr[], TripScore score_arr[], size_t size);
bool IsInteger(const char* str);
bool IsCharacter(const char* str);
#endif  // AAU_P1_SOFTWARE_SRC_OUTPUT_H_

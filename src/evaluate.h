#ifndef AAU_P1_SOFTWARE_SRC_EVALUATE_H_
#define AAU_P1_SOFTWARE_SRC_EVALUATE_H_
#include <stddef.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

/* Struct initialized for testing. May look different when final form is
   decided. */
typedef struct TripData {
  // int trip_id_;
  double overall_score;
  double price;
  double price_score;
  double comfortability;
  double comfortability_score;
  double time;
  double time_score;
  double emissions;
  double emissions_score;
} TripData;

typedef struct CalculateScoreParameters {
  const size_t kNumTrips;
  const size_t kReadOffset;
  const size_t kWriteOffset;
  const int kInverted;
} CalculateScoreParameters;

void CalculateScore(TripData trip_data[],
                    const CalculateScoreParameters* calculate_score_parameters);
void Evaluate(TripData trip_arr[], size_t size_of_struct_array);
void EvaluateTestingFunction();
#endif  // AAU_P1_SOFTWARE_SRC_EVALUATE_H_

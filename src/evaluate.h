#ifndef AAU_P1_SOFTWARE_SRC_EVALUATE_H_
#define AAU_P1_SOFTWARE_SRC_EVALUATE_H_
#include <stddef.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

// Constants for testing until we have concrete calculations of the attributes
// of the routes.
enum ArbitrarySizes {
  kSizeOfArrayForTesting = 5,
  kArbitrarySizeOfValuesInTest = 500
};

/* Struct initialized for testing. May look different when final form is
   decided. */
typedef struct TripData {
  int trip_id;
  double price;
  double health;
  double time;
  double environment;
} TripData;

typedef struct TripScore {
  int trip_id;
  double overall_score;
  double price_score;
  double health_score;
  double time_score;
  double environment_score;
} TripScore;

typedef struct CalculateScoreParameters {
  const size_t kNumTrips;
  const size_t kReadOffset;
  const size_t kWriteOffset;
  const int kInverted;
} CalculateScoreParameters;

void CalculateScore(TripData trip_data[], TripScore trip_score[],
                    const CalculateScoreParameters* calculate_score_parameters);
void Evaluate(TripData data_arr[], TripScore score_arr[], size_t size_arr);
void TESTPopulateTripArray(TripData arr[],
                           size_t size);  // TODO(unknown): To be removed.
#endif                                    // AAU_P1_SOFTWARE_SRC_EVALUATE_H_

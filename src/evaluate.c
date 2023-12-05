#include "evaluate.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "preferences.h"

#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

// Constants for testing until we have concrete calculations of the attributes
// of the routes.
enum Constants {
  kSizeOfArrayForTesting = 5,
  kArbitrarySizeOfValuesInTest = 500
};

/* This function is included as initial testing code to populate the struct
   array. */
void EvaluateTestingFunction() {
  srand((unsigned int)time(NULL));
  size_t size_of_struct_array = kSizeOfArrayForTesting;
  TripData trip_arr[size_of_struct_array];

  // Population the struct array.
  for (size_t i = 0; i < size_of_struct_array; i++) {
    // NOLINTBEGIN(concurrency-mt-unsafe)
    trip_arr[i].price = (double)(rand() % kArbitrarySizeOfValuesInTest);
    trip_arr[i].comfortability =
        (double)(rand() % kArbitrarySizeOfValuesInTest);
    trip_arr[i].time = (double)(rand() % kArbitrarySizeOfValuesInTest);
    trip_arr[i].emissions = (double)(rand() % kArbitrarySizeOfValuesInTest);
    // NOLINTEND(concurrency-mt-unsafe)
  }
  Evaluate(trip_arr, size_of_struct_array);
}

void CalculateScore(
    TripData trip_data[],
    const CalculateScoreParameters* calculate_score_parameters) {
  size_t num_trips = calculate_score_parameters->kNumTrips;
  size_t read_offset = calculate_score_parameters->kReadOffset;
  size_t write_offset = calculate_score_parameters->kWriteOffset;
  int inverted = calculate_score_parameters->kInverted;

  double attribute_largest = 0;
  double attribute_smallest = 0;
  void* read_member = NULL;
  void* write_member = NULL;

  /* For loop for finding largest and smallest value among the same members in
     difference instances of the struct. */
  for (size_t i = 0; i < num_trips; i++) {
    /* Using pointer arithmetic to navigate to the correct members in each
       instance of the struct. */
    read_member = (void*)((char*)&trip_data[i] + read_offset);
    // NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
    write_member = (void*)((char*)&trip_data[i] + write_offset);

    // Updating largest and smallest attribute value
    if (*(double*)read_member > attribute_largest) {
      attribute_largest = *(double*)read_member;
    }
    if (i == 0) {
      attribute_smallest = *(double*)read_member;
    }
    if (*(double*)read_member < attribute_smallest) {
      attribute_smallest = *(double*)read_member;
    }
  }

  // For loop to update the score for the member in each instance of the struct.
  for (size_t j = 0; j < num_trips; j++) {
    read_member = (void*)((char*)&trip_data[j] + read_offset);
    write_member = (void*)((char*)&trip_data[j] + write_offset);

    // Relative score calculation
    double score = (*(double*)read_member - attribute_smallest) /
                   (attribute_largest - attribute_smallest);

    // If big is bad, like price, the score is inverted.
    if (inverted) {
      score = 1 - score;
    }

    // Relevant score member is updated.
    *(double*)write_member = score;
  }
}

void Evaluate(TripData trip_arr[], size_t size_of_struct_array) {
  // Calculating score.
  CalculateScoreParameters price_params = {size_of_struct_array,
                                           offsetof(TripData, price),
                                           offsetof(TripData, price_score), 1};
  CalculateScore(trip_arr, &price_params);

  CalculateScoreParameters comfortability_params = {
      size_of_struct_array, offsetof(TripData, comfortability),
      offsetof(TripData, comfortability_score), 0};
  CalculateScore(trip_arr, &comfortability_params);

  CalculateScoreParameters time_params = {size_of_struct_array,
                                          offsetof(TripData, time),
                                          offsetof(TripData, time_score), 1};
  CalculateScore(trip_arr, &time_params);

  CalculateScoreParameters emissions_params = {
      size_of_struct_array, offsetof(TripData, emissions),
      offsetof(TripData, emissions_score), 0};
  CalculateScore(trip_arr, &emissions_params);

  // Calculate overall_score using all other scores.
  for (size_t i = 0; i < size_of_struct_array; i++) {
    trip_arr[i].overall_score =
        (GetUserPreference("price") / 1.0) * trip_arr[i].price_score +
        (GetUserPreference("health") / 1.0) *
            trip_arr[i].comfortability_score +
        (GetUserPreference("time") / 1.0) * trip_arr[i].time_score +
        (GetUserPreference("environment") / 1.0) * trip_arr[i].emissions_score;
  }

  // Print of the results.
  printf("\n");
  for (size_t i = 0; i < size_of_struct_array; i++) {
    printf(
        "Trip %zu: P: %6.2lf H: %6.2lf T: %6.2lf E: %6.2lf --- Ps: %2.2lf Hs: "
        "%2.2lf Ts: %2.2lf Es: %2.2lf --- Os: %2.2lf.\n",
        i + 1, trip_arr[i].price, trip_arr[i].comfortability,
        trip_arr[i].time, trip_arr[i].emissions, trip_arr[i].price_score,
        trip_arr[i].comfortability_score, trip_arr[i].time_score,
        trip_arr[i].emissions_score, trip_arr[i].overall_score);
  }
}

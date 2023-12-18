#include "evaluate.h"

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "preferences.h"

/* This function is included as initial testing code to populate the struct
   array. */
void TESTPopulateTripArray(TripData arr[], size_t size) {
  srand((unsigned int)time(NULL));

  // Populating the struct array.
  for (size_t i = 0; i < size; i++) {
    arr[i].trip_id = (int)i;
#ifdef __linux__
    // NOLINTBEGIN(concurrency-mt-unsafe)
    arr[i].price = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].health = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].time = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].environment = (double)(rand() % kArbitrarySizeOfValuesInTest);
    // NOLINTEND(concurrency-mt-unsafe)
#elifdef __APPLE__
    arr[i].price = (double)(arc4random() % kArbitrarySizeOfValuesInTest);
    arr[i].health = (double)(arc4random() % kArbitrarySizeOfValuesInTest);
    arr[i].time = (double)(arc4random() % kArbitrarySizeOfValuesInTest);
    arr[i].environment = (double)(arc4random() % kArbitrarySizeOfValuesInTest);
#endif
  }
}

void CalculateScore(
    TripData trip_data[], TripScore trip_score[],
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
    write_member = (void*)((char*)&trip_score[j] + write_offset);

    // Relative score calculation
    double score = (*(double*)read_member - attribute_smallest) /
                   (attribute_largest - attribute_smallest);

    // If big is bad, like price, the score is inverted.
    if (inverted) {
      score = 1.0 - score;
    }

    // Check to minimize floating number funk.
    if (score > 1.0) {
      score = 1.0;
    }

    // Relevant score member is updated.
    *(double*)write_member = score;
  }
}

void Evaluate(TripData data_arr[], TripScore score_arr[], size_t size_arr) {
  // Calculating score.
  CalculateScoreParameters price_params = {size_arr, offsetof(TripData, price),
                                           offsetof(TripScore, price_score), 1};
  CalculateScore(data_arr, score_arr, &price_params);

  CalculateScoreParameters health_params = {
      size_arr, offsetof(TripData, health), offsetof(TripScore, health_score),
      0};
  CalculateScore(data_arr, score_arr, &health_params);

  CalculateScoreParameters time_params = {size_arr, offsetof(TripData, time),
                                          offsetof(TripScore, time_score), 1};
  CalculateScore(data_arr, score_arr, &time_params);

  CalculateScoreParameters environment_params = {
      size_arr, offsetof(TripData, environment),
      offsetof(TripScore, environment_score), 1};
  CalculateScore(data_arr, score_arr, &environment_params);

  // Calculate overall_score using all other scores.
  for (size_t i = 0; i < size_arr; i++) {
    score_arr[i].trip_id = data_arr[i].trip_id;
    score_arr[i].overall_score =
        GetUserPreference("price") * score_arr[i].price_score +
        GetUserPreference("health") * score_arr[i].health_score +
        GetUserPreference("time") * score_arr[i].time_score +
        GetUserPreference("environment") * score_arr[i].environment_score;
  }
}

#include "evaluate.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "preferences.h"

#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

/* This function is included as initial testing code to populate the struct
   array. */
void TESTPopulateTripArray(TripData arr[], size_t size) {
  srand((unsigned int)time(NULL));

  // Population the struct array.
  for (size_t i = 0; i < size; i++) {
    // NOLINTBEGIN(concurrency-mt-unsafe)
    arr[i].price = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].comfortability = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].time = (double)(rand() % kArbitrarySizeOfValuesInTest);
    arr[i].emissions = (double)(rand() % kArbitrarySizeOfValuesInTest);
    // NOLINTEND(concurrency-mt-unsafe)
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
      score = 1 - score;
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

  CalculateScoreParameters comfortability_params = {
      size_arr, offsetof(TripData, comfortability),
      offsetof(TripScore, comfortability_score), 0};
  CalculateScore(data_arr, score_arr, &comfortability_params);

  CalculateScoreParameters time_params = {size_arr, offsetof(TripData, time),
                                          offsetof(TripScore, time_score), 1};
  CalculateScore(data_arr, score_arr, &time_params);

  CalculateScoreParameters emissions_params = {
      size_arr, offsetof(TripData, emissions),
      offsetof(TripScore, emissions_score), 0};
  CalculateScore(data_arr, score_arr, &emissions_params);

  // Calculate overall_score using all other scores.
  for (size_t i = 0; i < size_arr; i++) {
    score_arr[i].overall_score =
        GetUserPreference("price") * score_arr[i].price_score +
        GetUserPreference("health") * score_arr[i].comfortability_score +
        GetUserPreference("time") * score_arr[i].time_score +
        GetUserPreference("environment") * score_arr[i].emissions_score;
  }
}

int RouteCompareOverall(const void* ptr1, const void* ptr2) {
  // Should convert double to int down to 0.01 or 1%
  const double kConvertMultiplier = 100;

  TripData trip1 = *(TripData*)ptr1;
  TripData trip2 = *(TripData*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.overall_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.overall_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteComparePrice(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripData trip1 = *(TripData*)ptr1;
  TripData trip2 = *(TripData*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.price_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.price_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareTime(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripData trip1 = *(TripData*)ptr1;
  TripData trip2 = *(TripData*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.time_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.time_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareEnvironment(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripData trip1 = *(TripData*)ptr1;
  TripData trip2 = *(TripData*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.emissions_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.emissions_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareHealth(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripData trip1 = *(TripData*)ptr1;
  TripData trip2 = *(TripData*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.comfortability_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.comfortability_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

void InitTrip(TripData* arr, int arr_size) {
  const double kNum1 = 0.02;
  const double kNum2 = 0.4;
  const double kNum3 = 0.33;
  const double kNum4 = 0.78;
  const double kNum5 = 0.4;

  arr[0].overall_score = kNum1;
  arr[1].overall_score = kNum2;
  arr[2].overall_score = kNum3;
  arr[3].overall_score = kNum4;
  arr[4].overall_score = kNum5;

  for (int i = 0; i < arr_size; i++) {
    printf("Trip: %d overall score: %lf\n", i + 1, arr[i].overall_score);
  }
  printf("\n");
}

void PrintSortedTrip(TripData* arr, int arr_size) {
  for (int i = 0; i < arr_size; i++) {
    printf("Trip: %d overall score: %lf\n", i + 1, arr[i].overall_score);
  }
}

void SortRoutes(TripData* trips, char* attribute, size_t size_of_struct_array) {
  if (!strcmp(attribute, "overall")) {
    qsort(trips, size_of_struct_array, sizeof(TripData), RouteCompareOverall);
  } else if (!strcmp(attribute, "price")) {
    qsort(trips, size_of_struct_array, sizeof(TripData), RouteComparePrice);
  } else if (!strcmp(attribute, "time")) {
    qsort(trips, size_of_struct_array, sizeof(TripData), RouteCompareTime);
  } else if (!strcmp(attribute, "environment")) {
    qsort(trips, size_of_struct_array, sizeof(TripData),
          RouteCompareEnvironment);
  } else if (!strcmp(attribute, "health")) {
    qsort(trips, size_of_struct_array, sizeof(TripData), RouteCompareHealth);
  } else {
    printf("Error sorting attribute!\n");
    return;
  }
}

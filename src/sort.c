#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evaluate.h"

int RouteCompareOverall(const void* ptr1, const void* ptr2) {
  // Should convert double to int down to 0.01 or 1%
  const double kConvertMultiplier = 100;

  TripScore trip1 = *(TripScore*)ptr1;
  TripScore trip2 = *(TripScore*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.overall_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.overall_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteComparePrice(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripScore trip1 = *(TripScore*)ptr1;
  TripScore trip2 = *(TripScore*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.price_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.price_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareTime(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripScore trip1 = *(TripScore*)ptr1;
  TripScore trip2 = *(TripScore*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.time_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.time_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareEnvironment(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripScore trip1 = *(TripScore*)ptr1;
  TripScore trip2 = *(TripScore*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.environment_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.environment_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

int RouteCompareHealth(const void* ptr1, const void* ptr2) {
  const double kConvertMultiplier =
      100;  // Should convert double to int down to 0.01 or 1%
  TripScore trip1 = *(TripScore*)ptr1;
  TripScore trip2 = *(TripScore*)ptr2;

  // Should be able to compare down to 1% or 0.01
  int attribute_trip1 = (int)(trip1.health_score * kConvertMultiplier);
  int attribute_trip2 = (int)(trip2.health_score * kConvertMultiplier);

  return attribute_trip2 - attribute_trip1;
}

void SortRoutes(TripScore* trips, char* attribute,
                size_t size_of_struct_array) {
  if (!strcmp(attribute, "overall")) {
    qsort(trips, size_of_struct_array, sizeof(TripScore), RouteCompareOverall);
  } else if (!strcmp(attribute, "price")) {
    qsort(trips, size_of_struct_array, sizeof(TripScore), RouteComparePrice);
  } else if (!strcmp(attribute, "time")) {
    qsort(trips, size_of_struct_array, sizeof(TripScore), RouteCompareTime);
  } else if (!strcmp(attribute, "environment")) {
    qsort(trips, size_of_struct_array, sizeof(TripScore),
          RouteCompareEnvironment);
  } else if (!strcmp(attribute, "health")) {
    qsort(trips, size_of_struct_array, sizeof(TripScore), RouteCompareHealth);
  } else {
    printf("Error sorting attribute!\n");
    return;
  }
}

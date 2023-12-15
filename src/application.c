#include "application.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "evaluate.h"
#include "input.h"
#include "output.h"
#include "preferences.h"
#include "sort.h"
#include "trip.h"

void Run() {
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  SetUserPreference("price", 0.10);
  SetUserPreference("time", 0.20);
  SetUserPreference("environment", 0.30);
  SetUserPreference("health", 0.40);
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  CURL* const kCurl = curl_easy_init();

  if (!kCurl) {
    perror("cURL could not be initialized");

    exit(EXIT_FAILURE);
  }

  char* const kStartLocationInput = ReadUserInput("Enter a start location:");
  char* const kStopLocationInput = ReadUserInput("Enter a stop location:");

  printf("\n");

  printf("GETTING TRIP DATA...\n");
  Trips* trips = GetTrips(kCurl, kStartLocationInput, kStopLocationInput);

  free(kStartLocationInput);
  free(kStopLocationInput);

  printf("There are %zu routes available.\n", trips->number_of_trips);

  free(trips->trips);
  free(trips);

  // Initialization of struct arrays.
  TripData trip_data[kSizeOfArrayForTesting] = {0};
  TripScore trip_score[kSizeOfArrayForTesting] = {0};
  size_t trip_data_size_elements = sizeof(trip_data) / sizeof(TripData);

  // Function to populate data array to be deleted later.
  TESTPopulateTripArray(trip_data, trip_data_size_elements);

  // Reads data from data array and writes data to score array.
  Evaluate(trip_data, trip_score, trip_data_size_elements);

  // Iterates over user input.
  Output(trip_data, trip_score, trip_data_size_elements);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

#include "application.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "evaluate.h"
#include "input.h"
#include "output.h"
#include "trip.h"

void Run() {
  InputParameters user = {0};

  /*
   * PromptInputParameters now saves to the preferences,json.
   * It also handles loading from the file and presets.
   * You can currently test the program by using predefined in the first prompt.
   */
  PromptInputParameters(&user);

  CURL* const kCurl = curl_easy_init();

  if (!kCurl) {
    perror("cURL could not be initialized");

    exit(EXIT_FAILURE);
  }

  printf("GETTING TRIP DATA...\n");

  Trips* trips =
      GetTrips(kCurl, user.origin_location, user.destination_location);

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

  CoordinatesForStations(kCurl);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

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
   * PromptInputParameters now saves to the preferences.json.
   * It also handles loading from the file and presets.
   * You can currently test the program by using predefined in the first prompt.
   */
  PromptInputParameters(&user);

  CURL* const kCurl = curl_easy_init();

  if (!kCurl) {
    perror("cURL could not be initialized");

    exit(EXIT_FAILURE);
  }

  Trips* trips =
      GetTrips(kCurl, user.origin_location, user.destination_location);

  printf("There are %zu distinct routes available for this trip.\n",
         trips->number_of_trips);

  // Start test
  CoordinatesData* coordinates_for_trip =
      GetCoordinatesForTrip(kCurl, &(trips->trips[0]));
  free(coordinates_for_trip);
  // End test

  // Initialization of struct arrays.
  TripData* trip_data = NULL;
  trip_data = (TripData*)calloc(trips->number_of_trips, sizeof(TripData));
  TripScore* trip_score = NULL;
  trip_score = (TripScore*)calloc(trips->number_of_trips, sizeof(TripScore));

  // Function to populate data array to be deleted later.
  TESTPopulateTripArray(trip_data, trips->number_of_trips);

  // Reads data from data array and writes data to score array.
  Evaluate(trip_data, trip_score, trips->number_of_trips);

  // Iterates over user input.
  Output(trip_data, trip_score, trips->number_of_trips, trips);

  //  free(coordinates_for_trip);
  free(trips->trips);
  free(trips);
  free(trip_data);
  free(trip_score);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

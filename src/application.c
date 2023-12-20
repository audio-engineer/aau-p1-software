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
  UserPreferences* user_preferences = GetUserPreferences();

  CURL* const kCurl = curl_easy_init();

  if (!kCurl) {
    perror("cURL could not be initialized");

    exit(EXIT_FAILURE);
  }

  Trips* trips = GetTrips(kCurl, user_preferences->trip_parameters);

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

  // Calculate the attributes.
  CalculateTripData(trip_data, trips, kCurl, user_preferences);

  // Reads data from data array and writes data to score array.
  Evaluate(trip_data, trip_score, trips->number_of_trips);

  // Iterates over user input.
  Output(trip_data, trip_score, trips->number_of_trips, trips);

  FreeUserPreferences(user_preferences);

  free(trips->trips);
  free(trips);
  free(trip_data);
  free(trip_score);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

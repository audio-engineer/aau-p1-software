#include "application.h"

#include "curl/curl.h"
#include "curl/easy.h"
#include "evaluate.h"
#include "output.h"
#include "preferences.h"
#include "trip.h"

void Run() {
  /**
   * preferences.json test
   */
  // How to change the user preferences in the preferences file.
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  SetUserPreference("price", 0.10);
  SetUserPreference("time", 0.20);
  SetUserPreference("environment", 0.30);
  SetUserPreference("health", 0.40);
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  CURL* const kCurl = curl_easy_init();

  if (!kCurl) {
    return;
  }

  Trip(kCurl);

  // Initialization of struct arrays.
  TripData trip_data[kSizeOfArrayForTesting] = {0};
  TripScore trip_score[kSizeOfArrayForTesting] = {0};
  size_t trip_data_size_elements = sizeof(trip_data) / sizeof(TripData);

  // Function to populate data array to be deleted later.
  TESTPopulateTripArray(trip_data, trip_data_size_elements);

  // Reads data from data array and writes data to score array.
  Evaluate(trip_data, trip_score, trip_data_size_elements);

  // Prints data, scores and route details.
  Output(trip_data, trip_score, trip_data_size_elements);

  // Sorting test.
  const int kArrSize = 5;
  TripData testing_trips[kArrSize];
  InitTrip(testing_trips, kArrSize);
  SortRoutes(testing_trips, "overall", kArrSize);
  PrintSortedTrip(testing_trips, kArrSize);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

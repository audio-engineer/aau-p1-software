#include "trip.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api_handler.h"
#include "cJSON.h"
#include "globals.h"
#include "input.h"

void Trip(CURL* const curl) {
  const cJSON* location_list = NULL;
  const cJSON* stop_location = NULL;
  Response location_response = {NULL, 0};

  char* const kStartLocationInput = ReadUserInput("Enter a start location:");
  char* const kStopLocationInput = ReadUserInput("Enter a stop location:");
  char location_endpoint[kBufferSize] = "";

  char* start_encoded_input = curl_easy_escape(
      curl, kStartLocationInput, (int)strlen(kStartLocationInput));

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(location_endpoint, "location?input=", strlen("location?input="));
  strncat(location_endpoint, start_encoded_input, strlen(start_encoded_input));
  strncat(location_endpoint, "&format=json", strlen("&format=json"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  curl_free(start_encoded_input);
  DoRequest(curl, location_endpoint, &location_response);
  cJSON* const kStartLocationResponseBody = cJSON_Parse(location_response.body);

  location_list = cJSON_GetObjectItemCaseSensitive(kStartLocationResponseBody,
                                                   "LocationList");

  stop_location =
      cJSON_GetObjectItemCaseSensitive(location_list, "StopLocation");

  printf("\n");
  const cJSON* firstlocation = cJSON_GetArrayItem(stop_location, 0);
  long start_id = -1;

  if (firstlocation != NULL) {
    const cJSON* const kName =
        cJSON_GetObjectItemCaseSensitive(firstlocation, "name");
    const cJSON* const kXCoordinate =
        cJSON_GetObjectItemCaseSensitive(firstlocation, "x");
    const cJSON* const kYCoordinate =
        cJSON_GetObjectItemCaseSensitive(firstlocation, "y");
    const cJSON* const kLocationId =
        cJSON_GetObjectItemCaseSensitive(firstlocation, "id");

    printf("%s %s %s %s\n", kName->valuestring, kXCoordinate->valuestring,
           kYCoordinate->valuestring, kLocationId->valuestring);
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    start_id = strtol(kLocationId->valuestring, NULL, 10);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  free(kStartLocationInput);
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memset(location_endpoint, 0, sizeof(location_endpoint));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  cJSON_Delete(kStartLocationResponseBody);

  char* stop_encoded_input = curl_easy_escape(curl, kStopLocationInput,
                                              (int)strlen(kStopLocationInput));

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(location_endpoint, "location?input=", strlen("location?input="));
  strncat(location_endpoint, stop_encoded_input, strlen(stop_encoded_input));
  strncat(location_endpoint, "&format=json", strlen("&format=json"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  // Allocated in ReadUserInput
  free(kStopLocationInput);
  curl_free(stop_encoded_input);

  Response location_response_second = {NULL, 0};
  DoRequest(curl, location_endpoint, &location_response_second);
  cJSON* const kStopLocationResponseBody =
      cJSON_Parse(location_response_second.body);

  location_list = cJSON_GetObjectItemCaseSensitive(kStopLocationResponseBody,
                                                   "LocationList");

  stop_location =
      cJSON_GetObjectItemCaseSensitive(location_list, "StopLocation");

  const cJSON* secondlocation = cJSON_GetArrayItem(stop_location, 0);
  long dest_id = -1;

  if (secondlocation != NULL) {
    const cJSON* const kNameSecond =
        cJSON_GetObjectItemCaseSensitive(secondlocation, "name");
    const cJSON* const kXCoordinateSecond =
        cJSON_GetObjectItemCaseSensitive(secondlocation, "x");
    const cJSON* const kYCoordinateSecond =
        cJSON_GetObjectItemCaseSensitive(secondlocation, "y");
    const cJSON* const kLocationIdSecond =
        cJSON_GetObjectItemCaseSensitive(secondlocation, "id");

    printf("%s %s %s %s\n", kNameSecond->valuestring,
           kXCoordinateSecond->valuestring, kYCoordinateSecond->valuestring,
           kLocationIdSecond->valuestring);
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    dest_id = strtol(kLocationIdSecond->valuestring, NULL, 10);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  location_response_second = (Response){NULL, 0};

  cJSON_Delete(kStopLocationResponseBody);

  printf("CALCULATING TRIP\n");

  Response trip_response = {NULL, 0};
  char trip_endpoint[kBufferSize] = "";
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  char start_id_str[20];
  char dest_id_str[20];
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

  snprintf(start_id_str, sizeof(start_id), "%ld", start_id);
  snprintf(dest_id_str, sizeof(dest_id), "%ld", dest_id);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  char* encoded_start_id =
      curl_easy_escape(curl, start_id_str, (int)strlen(start_id_str));
  char* encoded_dest_id =
      curl_easy_escape(curl, dest_id_str, (int)strlen(dest_id_str));

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(trip_endpoint, "trip?originId=", strlen("trip?originId="));
  strncat(trip_endpoint, encoded_start_id, strlen(encoded_start_id));
  strncat(trip_endpoint, "&destId=", strlen("&destId="));
  strncat(trip_endpoint, encoded_dest_id, strlen(encoded_dest_id));
  strncat(trip_endpoint, "&format=json", strlen("&format=json"));
  strncat(trip_endpoint, "&useBus=0", strlen("&useBus=0"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  DoRequest(curl, trip_endpoint, &trip_response);
  cJSON* const kTripResponseBody = cJSON_Parse(trip_response.body);

  // printf("Trip response body: %s\n", trip_response.body);

  printf("trip_endpoint: %s\n", trip_endpoint);

  const cJSON* const kTripList =
      cJSON_GetObjectItemCaseSensitive(kTripResponseBody, "TripList");
  const cJSON* const kTripArray =
      cJSON_GetObjectItemCaseSensitive(kTripList, "Trip");

  const cJSON* k_trip = NULL;
  const cJSON* k_leg = NULL;

  // Iterate through each trip
  cJSON_ArrayForEach(k_trip, kTripArray) {
    const cJSON* const kLegArray =
        cJSON_GetObjectItemCaseSensitive(k_trip, "Leg");

    if (cJSON_IsObject(kLegArray)) {
      // Accessing different attributes of the leg
      const cJSON* const kOrigin =
          cJSON_GetObjectItemCaseSensitive(kLegArray, "Origin");
      const cJSON* const kTrainName =
          cJSON_GetObjectItemCaseSensitive(kLegArray, "name");
      const cJSON* const kOriginName =
          cJSON_GetObjectItemCaseSensitive(kOrigin, "name");
      const cJSON* const kOriginTime =
          cJSON_GetObjectItemCaseSensitive(kOrigin, "time");
      const cJSON* const kDestination =
          cJSON_GetObjectItemCaseSensitive(kLegArray, "Destination");
      const cJSON* const kDestinationName =
          cJSON_GetObjectItemCaseSensitive(kDestination, "name");
      const cJSON* const kDestinationTime =
          cJSON_GetObjectItemCaseSensitive(kDestination, "time");

      // Print the details
      printf("\n");
      printf("Origin: %s\n", kOriginName->valuestring);
      printf("Train: %s\n", kTrainName->valuestring);
      printf("Origin time: %s\n", kOriginTime->valuestring);
      printf("Destination: %s\n", kDestinationName->valuestring);
      printf("Destination time: %s\n", kDestinationTime->valuestring);

      // Check if there are more legs in the trip
      cJSON* const kNextLeg = kLegArray->next;
      if (kNextLeg != NULL) {
        const cJSON* const kNextOrigin =
            cJSON_GetObjectItemCaseSensitive(kNextLeg, "Origin");
        const cJSON* const kNextTrainName =
            cJSON_GetObjectItemCaseSensitive(kNextLeg, "name");
        const cJSON* const kNextOriginName =
            cJSON_GetObjectItemCaseSensitive(kNextOrigin, "name");
        const cJSON* const kNextOriginTime =
            cJSON_GetObjectItemCaseSensitive(kNextOrigin, "time");

        // Print information about switching trains
        printf("Switching to Train: %s at %s from %s\n",
               kNextTrainName->valuestring, kNextOriginTime->valuestring,
               kNextOriginName->valuestring);
      }
    }

    if (cJSON_IsArray(kLegArray)) {
      // Iterate through each leg in the trip
      cJSON_ArrayForEach(k_leg, kLegArray) {
        // Accessing different attributes of the leg
        const cJSON* const kOrigin =
            cJSON_GetObjectItemCaseSensitive(k_leg, "Origin");
        const cJSON* const kTrainName =
            cJSON_GetObjectItemCaseSensitive(k_leg, "name");
        const cJSON* const kOriginName =
            cJSON_GetObjectItemCaseSensitive(kOrigin, "name");
        const cJSON* const kOriginTime =
            cJSON_GetObjectItemCaseSensitive(kOrigin, "time");
        const cJSON* const kDestination =
            cJSON_GetObjectItemCaseSensitive(k_leg, "Destination");
        const cJSON* const kDestinationName =
            cJSON_GetObjectItemCaseSensitive(kDestination, "name");
        const cJSON* const kDestinationTime =
            cJSON_GetObjectItemCaseSensitive(kDestination, "time");

        // Print the details
        printf("\n");
        printf("Origin: %s\n", kOriginName->valuestring);
        printf("Train: %s\n", kTrainName->valuestring);
        printf("Origin time: %s\n", kOriginTime->valuestring);
        printf("Destination: %s\n", kDestinationName->valuestring);
        printf("Destination time: %s\n", kDestinationTime->valuestring);

        // Check if there are more legs in the trip
        cJSON* const kNextLeg = k_leg->next;
        if (kNextLeg != NULL) {
          const cJSON* const kNextOrigin =
              cJSON_GetObjectItemCaseSensitive(kNextLeg, "Origin");
          const cJSON* const kNextTrainName =
              cJSON_GetObjectItemCaseSensitive(kNextLeg, "name");
          const cJSON* const kNextOriginName =
              cJSON_GetObjectItemCaseSensitive(kNextOrigin, "name");
          const cJSON* const kNextOriginTime =
              cJSON_GetObjectItemCaseSensitive(kNextOrigin, "time");

          // Print information about switching trains
          printf("Switching to Train: %s at %s from %s\n",
                 kNextTrainName->valuestring, kNextOriginTime->valuestring,
                 kNextOriginName->valuestring);
        }
      }
    }
  }
  cJSON_Delete(kTripResponseBody);
  free(trip_response.body);
}

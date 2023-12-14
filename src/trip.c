#include "trip.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api_handler.h"
#include "cJSON.h"
#include "globals.h"

/**
 * @brief Makes an API call to Rejseplanen to get the location ID of the nearest
 * matching location.
 *
 * @param curl
 * @param location
 * @return Location ID as a heap-allocated string.
 */
char* GetLocationId(CURL* const curl, const char* const location) {
  char endpoint[kBufferSize] = {0};

  char* const kEncodedLocation =
      curl_easy_escape(curl, location, (int)strlen(location));

  strncat(endpoint, "location?input=", strlen("location?input="));
  strncat(endpoint, kEncodedLocation, strlen(kEncodedLocation));
  strncat(endpoint, "&format=json", strlen("&format=json"));

  curl_free(kEncodedLocation);

  Response response = {NULL, 0};

  DoRequest(curl, endpoint, &response);

  cJSON* const kStartLocationResponseBody = cJSON_Parse(response.body);
  free(response.body);

  const cJSON* const kLocationList = cJSON_GetObjectItemCaseSensitive(
      kStartLocationResponseBody, "LocationList");

  const cJSON* const kStopLocation =
      cJSON_GetObjectItemCaseSensitive(kLocationList, "StopLocation");

  const cJSON* const kNearestLocation = cJSON_GetArrayItem(kStopLocation, 0);

  if (!kNearestLocation) {
    return 0;
  }

  const char* const kNearestLocationId =
      cJSON_GetObjectItemCaseSensitive(kNearestLocation, "id")->valuestring;

  char* const kNearestLocationIdCopy =
      calloc(strlen(kNearestLocationId), sizeof(char));
  strcpy(kNearestLocationIdCopy, kNearestLocationId);

  cJSON_Delete(kStartLocationResponseBody);

  return kNearestLocationIdCopy;
}

/**
 * @brief Makes an API call to Rejseplanen to get the full trip data.
 *
 * @param curl
 * @param origin_location_id
 * @param destination_location_id
 * @return Pointer to heap-allocated cJSON struct. Delete it with cJSON_Delete.
 */
cJSON* GetTrip(CURL* const curl, const char* const origin_location_id,
               const char* const destination_location_id) {
  char endpoint[kBufferSize] = {0};

  strncat(endpoint, "trip?originId=", strlen("trip?originId="));
  strncat(endpoint, origin_location_id, strlen(origin_location_id));
  strncat(endpoint, "&destId=", strlen("&destId="));
  strncat(endpoint, destination_location_id, strlen(destination_location_id));
  strncat(endpoint, "&format=json", strlen("&format=json"));
  strncat(endpoint, "&useBus=0", strlen("&useBus=0"));

  Response response = {NULL, 0};

  DoRequest(curl, endpoint, &response);

  cJSON* const kTripResponseBody = cJSON_Parse(response.body);
  free(response.body);

  const cJSON* const kTripList =
      cJSON_GetObjectItemCaseSensitive(kTripResponseBody, "TripList");

  return cJSON_GetObjectItemCaseSensitive(kTripList, "Trip");
}

void GetTripData(CURL* const curl, const char* const origin,
                 const char* const destination) {
  char* const kOriginLocationId = GetLocationId(curl, origin);
  char* const kDestinationLocationId = GetLocationId(curl, destination);

  cJSON* trip = GetTrip(curl, kOriginLocationId, kDestinationLocationId);

  free(kOriginLocationId);
  free(kDestinationLocationId);

  // Iterate through each trip
  const cJSON* leg = NULL;

  cJSON_ArrayForEach(leg, trip) {
    const cJSON* const kLeg = cJSON_GetObjectItemCaseSensitive(leg, "Leg");

    if (cJSON_IsObject(kLeg)) {
      // Accessing different attributes of the leg
      const cJSON* const kOrigin =
          cJSON_GetObjectItemCaseSensitive(kLeg, "Origin");
      const cJSON* const kTrainName =
          cJSON_GetObjectItemCaseSensitive(kLeg, "name");
      const cJSON* const kOriginName =
          cJSON_GetObjectItemCaseSensitive(kOrigin, "name");
      const cJSON* const kOriginTime =
          cJSON_GetObjectItemCaseSensitive(kOrigin, "time");
      const cJSON* const kDestination =
          cJSON_GetObjectItemCaseSensitive(kLeg, "Destination");
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
      cJSON* const kNextLeg = kLeg->next;
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

    if (cJSON_IsArray(kLeg)) {
      // Iterate through each leg in the trip
      const cJSON* k_leg = NULL;

      cJSON_ArrayForEach(k_leg, kLeg) {
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

  cJSON_Delete(trip);
}

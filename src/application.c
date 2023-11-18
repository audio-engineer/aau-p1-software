#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api_handler.h"
#include "cJSON.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "globals.h"
#include "input.h"

void Run() {
  /*
   * Rejseplanen API test
   */
  printf(
      "### Rejseplanen API test "
      "#######################################################\n");

  CURL* const kCurl = curl_easy_init();
  const cJSON* location_list = NULL;
  const cJSON* stop_location = NULL;
  const cJSON* location = NULL;

  if (!kCurl) {
    return;
  }

  Response stops_nearby_response = {NULL, 0};

  DoRequest(kCurl, "stopsNearby?coordX=55673059&coordY=12565557&format=json",
            &stops_nearby_response);

  cJSON* const kStopsNearbyResponseBody =
      cJSON_Parse(stops_nearby_response.body);

  location_list = cJSON_GetObjectItemCaseSensitive(kStopsNearbyResponseBody,
                                                   "LocationList");
  stop_location =
      cJSON_GetObjectItemCaseSensitive(location_list, "StopLocation");

  printf("Stops nearby:\n");

  cJSON_ArrayForEach(location, stop_location) {
    const cJSON* const kName =
        cJSON_GetObjectItemCaseSensitive(location, "name");
    const cJSON* const kXCoordinate =
        cJSON_GetObjectItemCaseSensitive(location, "x");
    const cJSON* const kYCoordinate =
        cJSON_GetObjectItemCaseSensitive(location, "y");
    const cJSON* const kLocationId =
        cJSON_GetObjectItemCaseSensitive(location, "id");
    const cJSON* const kDistance =
        cJSON_GetObjectItemCaseSensitive(location, "distance");

    printf("%s %s %s %s %s\n", kName->valuestring, kXCoordinate->valuestring,
           kYCoordinate->valuestring, kLocationId->valuestring,
           kDistance->valuestring);
  }

  cJSON_Delete(kStopsNearbyResponseBody);

  // Allocated in SaveResponse
  free(stops_nearby_response.body);

  printf("\n");

  char* const kInput = ReadUserInput("Enter a location:");
  char location_endpoint[kBufferSize] = "";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(location_endpoint, "location?input=", strlen("location?input="));
  strncat(location_endpoint, kInput, strlen(kInput));
  strncat(location_endpoint, "&format=json", strlen("&format=json"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  // Allocated in ReadUserInput
  free(kInput);

  Response start_location_response = {NULL, 0};

  DoRequest(kCurl, location_endpoint, &start_location_response);

  cJSON* const kStartLocationResponseBody =
      cJSON_Parse(start_location_response.body);

  location_list = cJSON_GetObjectItemCaseSensitive(kStartLocationResponseBody,
                                                   "LocationList");
  stop_location =
      cJSON_GetObjectItemCaseSensitive(location_list, "StopLocation");

  printf("Location:\n");

  cJSON_ArrayForEach(location, stop_location) {
    const cJSON* const kName =
        cJSON_GetObjectItemCaseSensitive(location, "name");
    const cJSON* const kXCoordinate =
        cJSON_GetObjectItemCaseSensitive(location, "x");
    const cJSON* const kYCoordinate =
        cJSON_GetObjectItemCaseSensitive(location, "y");
    const cJSON* const kLocationId =
        cJSON_GetObjectItemCaseSensitive(location, "id");

    printf("%s %s %s %s\n", kName->valuestring, kXCoordinate->valuestring,
           kYCoordinate->valuestring, kLocationId->valuestring);
  }

  cJSON_Delete(kStartLocationResponseBody);

  // Allocated in SaveResponse
  free(start_location_response.body);

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

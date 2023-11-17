#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "cJSON.h"
#include "calculations.h"
#include "constants.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "input.h"
#include "string_builder.h"

size_t SaveResponse(char* data, size_t size, size_t nmemb, Response* response) {
  const size_t kResponseSize = size * nmemb;

  char* body = realloc(response->body, response->size + kResponseSize + 1);

  if (!body) {
    exit(EXIT_FAILURE);
  }

  response->body = body;
  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memcpy(&(response->body[response->size]), data, kResponseSize);
  response->size += kResponseSize;
  response->body[response->size] = '\0';

  return kResponseSize;
}

void Run() {
  /*
   * Print test
   */
  const int kFirstNumber = 3;
  const int kSecondNumber = 5;

  const int kAddedNumbers = AddNumbers(kFirstNumber, kSecondNumber);

  printf(
      "### Print test "
      "#################################################################\n");
  printf("First number: %d\n", kFirstNumber);
  printf("Second number: %d\n", kSecondNumber);
  printf("Added numbers: %d\n", kAddedNumbers);
  printf("\n");

  /*
   * String builder test
   */
  char* output_string = malloc(kInputBufferSize * sizeof(char));
  BuildString(output_string, "The number is: ", kSecondNumber);

  printf(
      "### String builder test "
      "########################################################\n");
  printf("%s\n", output_string);
  printf("\n");

  free(output_string);

  /*
   * cJSON test
   */
  const int kAge = 30;

  cJSON* json_object = cJSON_CreateObject();
  cJSON_AddStringToObject(json_object, "name", "John Doe");
  cJSON_AddNumberToObject(json_object, "age", kAge);
  cJSON_AddStringToObject(json_object, "email", "john.doe@example.com");

  char* json_string = cJSON_Print(json_object);

  printf(
      "### cJSON test "
      "#################################################################\n");
  printf("%s\n", json_string);
  printf("\n");

  cJSON_free(json_string);
  cJSON_Delete(json_object);

  /*
   * cURL test
   */
  CURL* curl = curl_easy_init();

  if (!curl) {
    return;
  }

  char endpoint[kInputBufferSize] = "";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(endpoint, kRejseplanenApiBaseUrl, strlen(kRejseplanenApiBaseUrl));
  strncat(endpoint, "stopsNearby?coordX=55673059&coordY=12565557&format=json",
          strlen("stopsNearby?coordX=55673059&coordY=12565557&format=json"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  Response response = {NULL, 0};

  curl_easy_setopt(curl, CURLOPT_URL, endpoint);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SaveResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  curl_easy_perform(curl);

  printf(
      "### cURL test "
      "##################################################################\n");

  cJSON* body = cJSON_Parse(response.body);

  const cJSON* kLocationList =
      cJSON_GetObjectItemCaseSensitive(body, "LocationList");
  const cJSON* kStopLocation =
      cJSON_GetObjectItemCaseSensitive(kLocationList, "StopLocation");

  const cJSON* kLocation = NULL;

  cJSON_ArrayForEach(kLocation, kStopLocation) {
    const cJSON* kName = cJSON_GetObjectItemCaseSensitive(kLocation, "name");
    const cJSON* kXCoordinate =
        cJSON_GetObjectItemCaseSensitive(kLocation, "x");
    const cJSON* kYCoordinate =
        cJSON_GetObjectItemCaseSensitive(kLocation, "y");
    const cJSON* kLocationId =
        cJSON_GetObjectItemCaseSensitive(kLocation, "id");
    const cJSON* kDistance =
        cJSON_GetObjectItemCaseSensitive(kLocation, "distance");

    printf("%s %s %s %s %s\n", kName->valuestring, kXCoordinate->valuestring,
           kYCoordinate->valuestring, kLocationId->valuestring,
           kDistance->valuestring);
  }

  cJSON_Delete(body);

  // Allocated in SaveResponse
  free(response.body);

  char* input = ReadUserInput("Enter a location:");
  char start_point[kInputBufferSize] = "";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(start_point, kRejseplanenApiBaseUrl, strlen(kRejseplanenApiBaseUrl));
  strncat(start_point, "location?input=", strlen("location?input="));
  strncat(start_point, input, strlen(input));
  strncat(start_point, "&format=json", strlen("&format=json"));
  printf("%s\n", start_point);
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  Response start_location_response = {NULL, 0};

  curl_easy_setopt(curl, CURLOPT_URL, start_point);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SaveResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &start_location_response);

  curl_easy_perform(curl);

  cJSON* start_location_response_body =
      cJSON_Parse(start_location_response.body);

  kLocationList = cJSON_GetObjectItemCaseSensitive(start_location_response_body,
                                                   "LocationList");
  kStopLocation =
      cJSON_GetObjectItemCaseSensitive(kLocationList, "StopLocation");

  cJSON_Print(kStopLocation);

  curl_easy_cleanup(curl);

  curl_global_cleanup();
}

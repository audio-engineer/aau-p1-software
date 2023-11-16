#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "cJSON.h"
#include "calculations.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "string_builder.h"

size_t SaveResponse(char* data, size_t size, size_t nmemb, Response* response) {
  size_t realsize = size * nmemb;

  char* ptr = realloc(response->body, response->size + realsize + 1);

  if (!ptr) {
    exit(EXIT_FAILURE);
  }

  response->body = ptr;
  memcpy(&(response->body[response->size]), data, realsize);
  response->size += realsize;
  response->body[response->size] = 0;

  return realsize;
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
  char* output_string = malloc(kBufferSize * sizeof(char));
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

  char endpoint[kBufferSize] = "";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(endpoint, kRejseplanenApiBaseUrl, 44);
  strncat(endpoint, "stopsNearby?coordX=55673059&coordY=12565557&format=json",
          55);
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

  const cJSON* location_list =
      cJSON_GetObjectItemCaseSensitive(body, "LocationList");
  const cJSON* stop_location =
      cJSON_GetObjectItemCaseSensitive(location_list, "StopLocation");

  const cJSON* location = NULL;

  cJSON_ArrayForEach(location, stop_location) {
    const cJSON* name = cJSON_GetObjectItemCaseSensitive(location, "name");
    const cJSON* x_coordinate = cJSON_GetObjectItemCaseSensitive(location, "x");
    const cJSON* y_coordinate = cJSON_GetObjectItemCaseSensitive(location, "y");
    const cJSON* location_id = cJSON_GetObjectItemCaseSensitive(location, "id");
    const cJSON* distance =
        cJSON_GetObjectItemCaseSensitive(location, "distance");

    printf("%s %s %s %s %s\n", name->valuestring, x_coordinate->valuestring,
           y_coordinate->valuestring, location_id->valuestring,
           distance->valuestring);
  }

  cJSON_Delete(body);

  free(response.body);

  curl_easy_cleanup(curl);
}

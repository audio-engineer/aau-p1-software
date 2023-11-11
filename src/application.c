#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "calculations.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "string_builder.h"

void Run() {
  /*
   * Print test
   */
  const int kFirstNumber = 3;
  const int kSecondNumber = 5;

  int added_numbers = AddNumbers(kFirstNumber, kSecondNumber);

  printf(
      "### Print test "
      "#################################################################\n");
  printf("First number: %d\n", kFirstNumber);
  printf("Second number: %d\n", kSecondNumber);
  printf("Added numbers: %d\n", added_numbers);
  printf("\n");

  /*
   * String builder test
   */
  char *output_string = malloc(kBufferSize * sizeof(char));
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

  cJSON *json_object = cJSON_CreateObject();
  cJSON_AddStringToObject(json_object, "name", "John Doe");
  cJSON_AddNumberToObject(json_object, "age", kAge);
  cJSON_AddStringToObject(json_object, "email", "john.doe@example.com");

  char *json_string = cJSON_Print(json_object);

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
  CURL *curl = curl_easy_init();

  if (!curl) {
    return;
  }

  char endpoint[kBufferSize] = "";
  strlcat(endpoint, kRejseplanenApiBaseUrl, kBufferSize);
  strlcat(endpoint, "stopsNearby?coordX=55673059&coordY=12565557&format=json",
          kBufferSize);

  curl_easy_setopt(curl, CURLOPT_URL, endpoint);

  printf(
      "### cURL test "
      "##################################################################\n");
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
}

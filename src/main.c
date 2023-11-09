#include "main.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calculations.h"
#include "string_builder.h"

int main() {
  /*
   * Basic output test
   */
  const int kFirstNumber = 3;
  const int kSecondNumber = 5;

  int added_numbers = AddNumbers(kFirstNumber, kSecondNumber);

  printf("First added_numbers: %d\n", kFirstNumber);
  printf("Second added_numbers: %d\n", kSecondNumber);
  printf("Added numbers: %d\n", added_numbers);

  /*
   * Some string test
   */
  char *output_string = malloc(kBufferSize * sizeof(char));
  BuildString(output_string, "The number is: ", kSecondNumber);
  printf("%s", output_string);
  free(output_string);

  /*
   * Some cURL
   */
  CURL *curl = curl_easy_init();

  if (!curl) {
    return EXIT_FAILURE;
  }

  char endpoint[kBufferSize] = "";
  strcat(endpoint, kRejseplanenApiBaseUrl);
  strcat(endpoint, "stopsNearby?coordX=55673059&coordY=12565557&format=json");

  curl_easy_setopt(curl, CURLOPT_URL, endpoint);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return EXIT_SUCCESS;
}

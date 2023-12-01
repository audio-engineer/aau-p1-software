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

  curl_easy_cleanup(kCurl);

  curl_global_cleanup();
}

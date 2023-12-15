#include "api_handler.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "globals.h"

size_t SaveResponse(const char* const data, const size_t size,
                    const size_t number_of_elements, Response* const response) {
  const size_t kResponseSize = size * number_of_elements;

  char* const kBody =
      realloc(response->body, response->size + kResponseSize + 1);

  if (!kBody) {
    exit(EXIT_FAILURE);
  }

  response->body = kBody;
  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memcpy(&(response->body[response->size]), data, kResponseSize);
  response->size += kResponseSize;
  response->body[response->size] = '\0';

  return kResponseSize;
}

void DoRequest(CURL* const curl, const char* const endpoint,
               const Response* response) {
  char url[kBufferSize * 2] = "";

  const char* const kRejseplanenApiBaseUrl =
      "https://xmlopen.rejseplanen.dk/bin/rest.exe/";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(url, kRejseplanenApiBaseUrl, strlen(kRejseplanenApiBaseUrl));
  strncat(url, endpoint, strlen(endpoint));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SaveResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

  curl_easy_perform(curl);
}

#include "api_handler.h"

#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "curl/curl.h"
#include "curl/easy.h"
#include "globals.h"

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

void DoRequest(CURL* curl, const char* const endpoint,
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

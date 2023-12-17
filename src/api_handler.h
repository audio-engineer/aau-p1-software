#ifndef AAU_P1_SOFTWARE_SRC_API_HANDLER_H_
#define AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

#include <stddef.h>

#include "curl/curl.h"

typedef struct Response {
  char* body;
  size_t size;
} Response;

void DoRequest(CURL* curl, const char* endpoint, const Response* response,
               bool prepend_base_url);

#endif  // AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

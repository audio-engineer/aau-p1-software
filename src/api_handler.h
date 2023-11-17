#ifndef AAU_P1_SOFTWARE_SRC_API_HANDLER_H_
#define AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

#ifdef __linux__
#include <stdlib.h>
#elifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "curl/curl.h"

typedef struct Response {
  char* body;
  size_t size;
} Response;

void DoRequest(CURL* curl, const char* endpoint, const Response* response);

#endif  // AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

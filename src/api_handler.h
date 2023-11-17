#ifndef AAU_P1_SOFTWARE_SRC_API_HANDLER_H_
#define AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

#include "curl/curl.h"
#include "globals.h"

void DoRequest(CURL* curl, const char* endpoint, Response* response);

#endif  // AAU_P1_SOFTWARE_SRC_API_HANDLER_H_

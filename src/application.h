#ifndef AAU_P1_SOFTWARE_SRC_APPLICATION_H_
#define AAU_P1_SOFTWARE_SRC_APPLICATION_H_

#ifdef __linux__
#include <stdlib.h>
#elifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
const char* kRejseplanenApiBaseUrl =
    "https://xmlopen.rejseplanen.dk/bin/rest.exe/";

enum Constants { kBufferSize = 256 };

typedef struct Response {
  char* body;
  size_t size;
} Response;

void Run();

#endif  // AAU_P1_SOFTWARE_SRC_APPLICATION_H_

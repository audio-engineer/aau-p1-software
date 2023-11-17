#ifndef AAU_P1_SOFTWARE_SRC_GLOBALS_H_
#define AAU_P1_SOFTWARE_SRC_GLOBALS_H_

#ifdef __linux__
#include <stdlib.h>
#elifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

enum Constants { kBufferSize = 256 };

typedef struct Response {
  char* body;
  size_t size;
} Response;

#endif  // AAU_P1_SOFTWARE_SRC_GLOBALS_H_

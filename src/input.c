#include "input.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"

char* ReadUserInput(const char* const message) {
  printf("%s ", message);

  char* input = calloc(kBufferSize, sizeof(char));

  if (!input) {
    perror("Memory allocation failed");

    return NULL;
  }

  if (!fgets(input, kBufferSize, stdin)) {
    free(input);

    return NULL;
  }

  const size_t kUserInputLength = strlen(input);

  if (1 > kUserInputLength || '\n' != input[kUserInputLength - 1]) {
    free(input);

    return NULL;
  }

  input[kUserInputLength - 1] = '\0';

  return input;
}

#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

char* ReadUserInput(const char* message) {
  printf("%s ", message);

  char* input = malloc(kInputBufferSize * sizeof(char));

  if (NULL == input) {
    perror("Memory allocation failed");

    return NULL;
  }

  if (!fgets(input, kInputBufferSize, stdin)) {
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

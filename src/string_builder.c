#include "string_builder.h"

#include <stdio.h>

void BuildString(char* output_string, const char* base_string, int number) {
  snprintf(output_string, 60, "%s%d", base_string, number);
}

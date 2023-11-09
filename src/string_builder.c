#include <stdio.h>

#include "string_builder.h"

void BuildString(char* output_string, const char* base_string, int number) {
  sprintf(output_string, "%s%d", base_string, number);
}

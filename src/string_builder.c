#include "string_builder.h"

#include <stdio.h>

void BuildString(char* output_string, const char* base_string, int number) {
  sprintf(output_string, "%s%d", base_string, number);
}

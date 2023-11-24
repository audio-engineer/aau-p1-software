#ifndef AAU_P1_SOFTWARE_SRC_PREFERENCES_H_
#define AAU_P1_SOFTWARE_SRC_PREFERENCES_H_

#include <stdio.h>

void SetUserPreference(const char* key, double value);

double GetUserPreference(FILE* preferences, const char* key);

#endif  // AAU_P1_SOFTWARE_SRC_PREFERENCES_H_

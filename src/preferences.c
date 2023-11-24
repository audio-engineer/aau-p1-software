#include "preferences.h"

#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

void InitializePreferenceFile() {
  const double kInitialValue = 0.25;

  cJSON* const kJsonPreferences = cJSON_CreateObject();

  cJSON_AddNumberToObject(kJsonPreferences, "price", kInitialValue);
  cJSON_AddNumberToObject(kJsonPreferences, "time", kInitialValue);
  cJSON_AddNumberToObject(kJsonPreferences, "environment", kInitialValue);
  cJSON_AddNumberToObject(kJsonPreferences, "health", kInitialValue);

  char* const kSerializedJsonWithNull = cJSON_Print(kJsonPreferences);
  char* const kSerializedJsonWithNewline =
      realloc(kSerializedJsonWithNull, strlen(kSerializedJsonWithNull) + 1);
  kSerializedJsonWithNewline[strlen(kSerializedJsonWithNull)] = '\n';

  FILE* preferences = fopen("preferences.json", "w");
  fputs(kSerializedJsonWithNewline, preferences);
  fclose(preferences);
  free(kSerializedJsonWithNewline);
}

void SetUserPreference(const char* key, double value) {
  FILE* preferences = fopen("preferences.json", "r");

  if (!preferences) {
    InitializePreferenceFile();

    preferences = fopen("preferences.json", "r");
  }

  fseek(preferences, 0, SEEK_END);
  const unsigned long kFileSize = (unsigned long)ftell(preferences);
  fseek(preferences, 0, SEEK_SET);

  char* const kFileBuffer = calloc(kFileSize, sizeof(char));

  size_t const kBytes =
      fread(kFileBuffer, sizeof(char), kFileSize, preferences);

  fclose(preferences);

  if (kBytes < kFileSize) {
    perror("Something happened while reading user preference");

    free(kFileBuffer);

    return;
  }

  kFileBuffer[kFileSize - 1] = '\0';

  const cJSON* const kJsonPreferences = cJSON_Parse(kFileBuffer);
  free(kFileBuffer);

  cJSON* const kJsonValue = cJSON_GetObjectItem(kJsonPreferences, key);

  kJsonValue->valuedouble = value;

  char* const kSerializedJsonWithNull = cJSON_Print(kJsonPreferences);
  char* const kSerializedJsonNewlineWithNewline =
      realloc(kSerializedJsonWithNull, strlen(kSerializedJsonWithNull) + 1);
  kSerializedJsonNewlineWithNewline[strlen(kSerializedJsonWithNull)] = '\n';

  preferences = fopen("preferences.json", "w");
  fputs(kSerializedJsonNewlineWithNewline, preferences);
  fclose(preferences);
  free(kSerializedJsonNewlineWithNewline);
}

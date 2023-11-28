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
  const unsigned long kSerializedJsonWithNullLength =
      strlen(kSerializedJsonWithNull);
  char* const kSerializedJsonWithNewline =
      realloc(kSerializedJsonWithNull, kSerializedJsonWithNullLength + 1);
  kSerializedJsonWithNewline[kSerializedJsonWithNullLength] = '\n';

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
  const unsigned long kSerializedJsonWithNullLength =
      strlen(kSerializedJsonWithNull);
  char* const kSerializedJsonNewlineWithNewline =
      realloc(kSerializedJsonWithNull, kSerializedJsonWithNullLength + 1);
  kSerializedJsonNewlineWithNewline[kSerializedJsonWithNullLength] = '\n';

  preferences = fopen("preferences.json", "w");
  fputs(kSerializedJsonNewlineWithNewline, preferences);
  fclose(preferences);
  free(kSerializedJsonNewlineWithNewline);
}

double GetUserPreference(const char* const key) {
  double value = 0;
  FILE* user_preferences = fopen("preferences.json", "r");

  if (!user_preferences) {
    printf("Error opening user preferences file!\n");
    return value;
  }

  fseek(user_preferences, 0, SEEK_END);
  unsigned long kFileSize = (unsigned long)ftell(user_preferences);
  fseek(user_preferences, 0, SEEK_SET);

  char* kFileBuffer = calloc(kFileSize + 1, sizeof(char));
  if (!kFileBuffer) {
    printf("Error allocating memory for file buffer!\n");
    fclose(user_preferences);
    return value;
  }
  const unsigned long kBytes =
      fread(kFileBuffer, sizeof(char), kFileSize, user_preferences);
  fclose(user_preferences);

  if (kBytes != kFileSize) {
    printf("Error reading user preferences file!\n");
    free(kFileBuffer);
    return value;
  }

  // kFileBuffer contains the content of the file
  cJSON* json_preferences = cJSON_Parse(kFileBuffer);
  free(kFileBuffer);

  if (!json_preferences) {
    printf("Error parsing JSON file!\n");
    return value;
  }

  cJSON* file_item = cJSON_GetObjectItem(json_preferences, key);

  value = cJSON_GetNumberValue(file_item);

  cJSON_Delete(json_preferences);

  return value;
}

#include "preferences.h"

#include <stdlib.h>
#include <unistd.h>

#include "cJSON.h"

void InitializePreferenceFile(FILE* preferences) {
  const double kValue = 0.25;
  char* serialized_json = NULL;

  cJSON* json_preferences = cJSON_CreateObject();

  cJSON* price_value = cJSON_CreateNumber(kValue);
  cJSON_AddItemToObject(json_preferences, "price", price_value);

  cJSON* time_value = cJSON_CreateNumber(kValue);
  cJSON_AddItemToObject(json_preferences, "time", time_value);

  cJSON* environment_value = cJSON_CreateNumber(kValue);
  cJSON_AddItemToObject(json_preferences, "environment", environment_value);

  cJSON* health_value = cJSON_CreateNumber(kValue);
  cJSON_AddItemToObject(json_preferences, "health", health_value);

  serialized_json = cJSON_Print(json_preferences);

  fputs(serialized_json, preferences);
  fseek(preferences, 0, SEEK_SET);
}

FILE* GetPreferenceFile() {
  FILE* preferences = NULL;

  if (access("preferences.json", F_OK) != 0) {
    preferences = fopen("preferences.json", "w+");

    if (!preferences) {
      perror("File creation failed");

      return NULL;
    }

    InitializePreferenceFile(preferences);

    return preferences;
  }

  preferences = fopen("preferences.json", "r+");

  if (!preferences) {
    perror("File opening failed");

    return NULL;
  }

  return preferences;
}

void SetUserPreference(FILE* user_preferences_file, const char* key,
                       double value) {
  fseek(user_preferences_file, 0, SEEK_END);
  unsigned long file_size = (unsigned long)ftell(user_preferences_file);
  fseek(user_preferences_file, 0, SEEK_SET);

  char* file_buffer = calloc(file_size, sizeof(char));

  size_t bytes =
      fread(file_buffer, sizeof(char), file_size, user_preferences_file);
  fseek(user_preferences_file, 0, SEEK_SET);

  if (bytes < file_size) {
    perror("Something happened while reading user preference");

    return;
  }

  file_buffer[file_size] = '\0';

  cJSON* read_file = cJSON_Parse(file_buffer);
  free(file_buffer);
  cJSON* read_value = cJSON_GetObjectItem(read_file, key);

  read_value->valuedouble = value;

  char* serialized_json = cJSON_Print(read_file);
  serialized_json[file_size - 1] = '\n';

  fputs(serialized_json, user_preferences_file);
  fseek(user_preferences_file, 0, SEEK_SET);
}

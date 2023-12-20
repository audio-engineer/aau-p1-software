#include "input.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <sys/unistd.h>
#endif
#include <unistd.h>

#include "globals.h"
#include "output.h"
#include "preferences.h"

enum MaximumValues {
  kMaximumAttributeValue = 10,
  kMaximumHours = 23,
  kMaximumMinutes = 59
};

enum PreferenceSources { kManual = 'm', kFile = 'f' };

enum YesNo { kYes = 'y', kNo = 'n' };

enum PresetValues {
  kTimeHour = 12,
  kTimeMinute = 30,
  kCarFuelEfficiency = 250
};

typedef enum HoursOrMinutes { kHours = 'h', kMinutes = 'm' } HoursOrMinutes;

char* ReadUserInput(const char* const message) {
  printf("\t%s%s%s ", COLOR_GREEN, message, COLOR_RESET);
  fflush(stdout);

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

bool IsValidTimeMode(const char choice) {
  return (bool)((choice == kArrival) || (choice == kDeparture));
}

bool IsValidPreferenceSource(const char choice) {
  return (bool)((choice == kManual) || (choice == kFile));
}

bool IsValidYesNo(const char choice) {
  return (bool)((choice == kYes) || (choice == kNo));
}

void PrintInstructionMessage(const char* const message) {
  printf("%s%s%s\n", COLOR_YELLOW, message, COLOR_RESET);
}

char ReadCharInput(const char* const message,
                   bool validation_function(const char choice)) {
  char choice = '\0';

  while (true) {
    char* choose_prompts = ReadUserInput(message);

    choice = choose_prompts[0];

    if (validation_function(choice)) {
      free(choose_prompts);

      break;
    }

    printf("%sInvalid input. Try again.%s\n", COLOR_RED, COLOR_RESET);

    free(choose_prompts);
  }

  return choice;
}

bool ReadBooleanInput(const char* const message) {
  return (bool)(ReadCharInput(message, IsValidYesNo) == 'y' ? 1 : 0);
}

int ReadIntegerInput(const char* const message, const int max_value) {
  int output = 0;
  char* end = NULL;

  while (true) {
    char* attribute = ReadUserInput(message);

    if (IsInteger(attribute)) {
      long attribute_num = strtol(attribute, &end, kBaseTen);

      if (0 <= attribute_num && attribute_num <= max_value) {
        free(attribute);

        output = (int)attribute_num;

        break;
      }
    }

    free(attribute);

    printf("%sInvalid input. Pick a number between 0 and %d!%s\n", COLOR_RED,
           max_value, COLOR_RESET);
  }

  return output;
}

char* GetTimeModePromptText(TimeMode time_mode,
                            HoursOrMinutes hours_or_minutes) {
  char* time_mode_prompt = calloc(kBufferSize, sizeof(char));

  char* selected_time_mode = time_mode == kDeparture ? "departure" : "arrival";
  char* selected_hours_or_minutes =
      hours_or_minutes == kHours ? " hour (0 - 23)" : " minute (0 - 59)";

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(time_mode_prompt, "Enter ", strlen("Enter "));
  strncat(time_mode_prompt, selected_time_mode, strlen(selected_time_mode));
  strncat(time_mode_prompt, selected_hours_or_minutes,
          strlen(selected_hours_or_minutes));
  strncat(time_mode_prompt, ":", strlen(":"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  return time_mode_prompt;
}

TripParameters* GetTripParameters() {
  TripParameters* trip_parameters = calloc(1, sizeof(TripParameters));

  PrintInstructionMessage("What are the origin and destination for your trip?");
  trip_parameters->origin_location =
      ReadUserInput("\U0001F4CD Enter origin location:");
  trip_parameters->destination_location =
      ReadUserInput("\U0001F4CD Enter destination location:");
  PrintInstructionMessage(
      "Do you want the trip to be planned according to a departure time, or "
      "arrival time?");
  trip_parameters->time_mode = (TimeMode)ReadCharInput(
      "Enter 'd' for departure or 'a' for arrival:", IsValidTimeMode);

  PrintInstructionMessage("Specify your preferred time constraints.");
  char* hours_prompt =
      GetTimeModePromptText(trip_parameters->time_mode, kHours);
  trip_parameters->time_hour = ReadIntegerInput(hours_prompt, kMaximumHours);
  free(hours_prompt);

  char* minutes_prompt =
      GetTimeModePromptText(trip_parameters->time_mode, kMinutes);
  trip_parameters->time_minute =
      ReadIntegerInput(minutes_prompt, kMaximumMinutes);
  free(minutes_prompt);

  return trip_parameters;
}

CommutingPreferences* GetCommutingPreferences() {
  CommutingPreferences* commuting_preferences =
      calloc(1, sizeof(CommutingPreferences));

  const int kMaxCarEfficiency = 500;
  const double kConvertToAttributeDouble = 10.0;

  PrintInstructionMessage(
      "Now assign weights (from 1 to 10) to price, time, and sustainability in "
      "your choice of transport.");

  double attribute_price =
      ReadIntegerInput("\U0001F4B0 Price (0 - 10):", kMaximumAttributeValue) /
      kConvertToAttributeDouble;
  double attribute_health = 0;
  double attribute_time =
      ReadIntegerInput("\U0001F552 Time (0 - 10):", kMaximumAttributeValue) /
      kConvertToAttributeDouble;
  double attribute_environment =
      ReadIntegerInput("\U0001F332 Sustainability (0 - 10):",
                       kMaximumAttributeValue) /
      kConvertToAttributeDouble;

  double attribute_total = attribute_price + attribute_health + attribute_time +
                           attribute_environment;

  commuting_preferences->price = attribute_price / attribute_total;
  commuting_preferences->time = attribute_time / attribute_total;
  commuting_preferences->environment = attribute_environment / attribute_total;
  commuting_preferences->health = attribute_health / attribute_total;

  PrintInstructionMessage("What modes of transport can your route include?");
  commuting_preferences->is_walking =
      ReadBooleanInput("\U0001F3C3 Walking? (y/n):");
  commuting_preferences->is_biking =
      ReadBooleanInput("\U0001F6B4 Biking? (y/n):");
  commuting_preferences->is_using_train =
      ReadBooleanInput("\U0001F682 Trains? (y/n):");
  commuting_preferences->is_driving =
      ReadBooleanInput("\U0001F697 Car? (y/n):");

  if (!commuting_preferences->is_driving) {
    commuting_preferences->car_fuel_efficiency = 0;

    return commuting_preferences;
  }

  commuting_preferences->car_fuel_efficiency =
      ReadIntegerInput("What is the efficiency of the car in km/l? (0 - 500):",
                       kMaxCarEfficiency);

  return commuting_preferences;
}

UserPreferences* GetManualPreferences() {
  UserPreferences* user_preferences = calloc(1, sizeof(UserPreferences));

  user_preferences->trip_parameters = GetTripParameters();
  user_preferences->commuting_preferences = GetCommutingPreferences();

  return user_preferences;
}

UserPreferences* GetFilePreferences() {
  if (0 != access("preferences.json", F_OK)) {
    return NULL;
  }

  UserPreferences* user_preferences = calloc(1, sizeof(UserPreferences));
  CommutingPreferences* commuting_preferences =
      calloc(1, sizeof(CommutingPreferences));
  TripParameters* trip_parameters = calloc(1, sizeof(TripParameters));

  if (!user_preferences || !commuting_preferences || !trip_parameters) {
    perror(
        "Could not allocate UserPreferences, or CommutingPreferences, or "
        "TripParameters");

    exit(EXIT_FAILURE);
  }

  commuting_preferences->price = GetUserPreference("price");
  commuting_preferences->time = GetUserPreference("time");
  commuting_preferences->environment = GetUserPreference("environment");
  commuting_preferences->health = GetUserPreference("health");
  commuting_preferences->is_walking = true;
  commuting_preferences->is_biking = true;
  commuting_preferences->is_using_train = true;
  commuting_preferences->is_driving = true;
  commuting_preferences->car_fuel_efficiency = kCarFuelEfficiency;
  user_preferences->commuting_preferences = commuting_preferences;

  trip_parameters->origin_location = strdup("Slagelse");
  trip_parameters->destination_location = strdup("Hellerup");
  trip_parameters->time_mode = kDeparture;
  trip_parameters->time_hour = kTimeHour;
  trip_parameters->time_minute = kTimeMinute;
  user_preferences->trip_parameters = trip_parameters;

  return user_preferences;
}

void SavePreferencesToFile(UserPreferences* user_preferences) {
  SetUserPreference("price", user_preferences->commuting_preferences->price);
  SetUserPreference("time", user_preferences->commuting_preferences->time);
  SetUserPreference("environment",
                    user_preferences->commuting_preferences->environment);
  SetUserPreference("health", user_preferences->commuting_preferences->health);
}

void ShowBanner() {
  FILE* file = fopen("banner.txt", "r");

  if (file == NULL) {
    perror("Error opening banner.txt file");

    exit(EXIT_FAILURE);
  }

  char line[kBufferSize] = {0};

  while (fgets(line, sizeof(line), file)) {
    printf("%s%s%s", COLOR_MAGENTA, line, COLOR_RESET);
  }

  fclose(file);
}

UserPreferences* GetUserPreferences() {
  printf("%sWELCOME TO THE PERSONALIZED INTERACTIVE%s\n", COLOR_CYAN,
         COLOR_RESET);
  ShowBanner();
  printf("%sCopyright \U000000A9 2023 AAU%s\n", COLOR_CYAN, COLOR_RESET);

  printf("\n");
  PrintInstructionMessage(
      "Would you like to initialize using the values saved in your preferences "
      "file, or manually input them?");

  UserPreferences* user_preferences = NULL;

  switch (ReadCharInput("Enter 'f' for file values or 'm' for manual input:",
                        IsValidPreferenceSource)) {
    case kFile:
      user_preferences = GetFilePreferences();

      if (user_preferences) {
        break;
      }

      printf(
          "%sPreferences file could not be found. Continuing using manual "
          "input.%s\n",
          COLOR_RED, COLOR_RESET);
    case kManual:
      user_preferences = GetManualPreferences();

      PrintInstructionMessage(
          "Would you like to save these preferences to the preferences file?");
      if (ReadBooleanInput("Save to file? (y/n):")) {
        SavePreferencesToFile(user_preferences);
      }

      break;
    default:
      break;
  }

  if (!user_preferences) {
    perror("User preferences could not be initialized");

    exit(EXIT_FAILURE);
  }

  return user_preferences;
}

void FreeTripParameters(TripParameters* trip_parameters) {
  free(trip_parameters->origin_location);
  free(trip_parameters->destination_location);
  free(trip_parameters);
}

void FreeCommutingPreferences(CommutingPreferences* commuting_preferences) {
  free(commuting_preferences);
}

void FreeUserPreferences(UserPreferences* user_preferences) {
  FreeTripParameters(user_preferences->trip_parameters);
  FreeCommutingPreferences(user_preferences->commuting_preferences);
  free(user_preferences);
}

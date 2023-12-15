#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

#include "globals.h"
#include "output.h"
#include "preferences.h"

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

int GetInputInteger(const char* message, int max_value) {
  bool valid_answer = false;
  int output = 0;
  while (!valid_answer) {
    char* attribute = ReadUserInput(message);

    if (IsInteger(attribute)) {
      long int attribute_num = strtol(attribute, NULL, kBaseTen);

      if (0 <= attribute_num && attribute_num <= max_value) {
        output = (int)attribute_num;
        valid_answer = true;
      } else {
        printf("Invalid answer please pick a number between 0 and %d! \n",
               max_value);
      }
    }
    free(attribute);
  }

  return output;
}

bool GetInputBoolean(const char* message) {
  while (1) {
    char* attribute_bool = ReadUserInput(message);

    char attribute_bool_answer = attribute_bool[0];
    free(attribute_bool);

    switch (attribute_bool_answer) {
      case kYes:
        return true;

      case kNo:
        return false;

      default:
        printf("Invalid answer!\n");
        break;
    }
  }
}

UserPreferences ManualAttributes(void) {
  UserPreferences attributes;
  const int kMaxNum = 10;
  const int kMaxCarEfficiency = 500;
  const double kConvertToAttributeDouble = 10.0;

  printf(
      "To input your personal preferences please write a number between "
      "0-10!\n");

  double attribute_price =
      GetInputInteger("Price preference (0-10):", kMaxNum) /
      kConvertToAttributeDouble;
  double attribute_time = GetInputInteger("Time preference (0-10):", kMaxNum) /
                          kConvertToAttributeDouble;
  double attribute_environment =
      GetInputInteger("Sustainability preference (0-10):", kMaxNum) /
      kConvertToAttributeDouble;
  double attribute_health =
      GetInputInteger("Health preference (0-10):", kMaxNum) /
      kConvertToAttributeDouble;

  double attribute_total = attribute_price + attribute_health + attribute_time +
                           attribute_environment;

  // Normalize the attributes to between 0 and 1.
  attribute_price = attribute_price / attribute_total;
  attribute_time = attribute_time / attribute_total;
  attribute_environment = attribute_environment / attribute_total;
  attribute_health = attribute_health / attribute_total;

  bool attribute_walk =
      GetInputBoolean("Can your route include walking? (y/n):");
  bool attribute_bike =
      GetInputBoolean("Can your route include biking? (y/n):");
  bool attribute_train =
      GetInputBoolean("Can your route include trains? (y/n):");
  bool attribute_car = GetInputBoolean("Can your route include a car? (y/n):");

  // Transfer the user data to the UserPreferences struct.
  attributes.price = attribute_price;
  attributes.time = attribute_time;
  attributes.environment = attribute_environment;
  attributes.health = attribute_health;
  // Save attributes to file.
  SetUserPreference("price", attribute_price);
  SetUserPreference("time", attribute_time);
  SetUserPreference("environment", attribute_environment);
  SetUserPreference("health", attribute_health);

  attributes.walk = attribute_walk;
  attributes.bike = attribute_bike;
  attributes.train = attribute_train;

  if (attribute_car) {
    int attribute_car_efficiency = GetInputInteger(
        "What is the efficiency of the car in kilometers/liter:",
        kMaxCarEfficiency);
    attributes.car = attribute_car;
    attributes.car_fuel_efficiency = attribute_car_efficiency;
  } else {
    attributes.car_fuel_efficiency = 0;
  }

  return attributes;
}

UserPreferences PresetAttributes(void) {
  const double kAttributePreferences = 0.25;
  const bool kPredefinedTransport = true;
  const int kFuelEfficiency = 100;

  UserPreferences user_attributes_generic = {
      kAttributePreferences, kAttributePreferences, kAttributePreferences,
      kAttributePreferences, kPredefinedTransport,  kPredefinedTransport,
      kPredefinedTransport,  kPredefinedTransport,  kFuelEfficiency};

  return user_attributes_generic;
}

UserPreferences FileAttributes(void) {
  const bool kTransportPreferences = true;
  const int kFuelEfficiency = 100;

  UserPreferences file_attributes = {0};

  file_attributes.price = GetUserPreference("price");
  file_attributes.time = GetUserPreference("time");
  file_attributes.environment = GetUserPreference("environment");
  file_attributes.health = GetUserPreference("health");

  file_attributes.walk = kTransportPreferences;
  file_attributes.bike = kTransportPreferences;
  file_attributes.train = kTransportPreferences;
  file_attributes.car = kTransportPreferences;
  file_attributes.car_fuel_efficiency = kFuelEfficiency;

  return file_attributes;
}

UserPreferences PromptAttributes(void) {
  while (1) {
    char* choice = ReadUserInput(
        "Do you want to manually input your preferenes? (m)\nLoad preferences "
        "from a file (f)\nUse presets? (p)\n       >>>");

    char user_choice = choice[0];

    free(choice);

    switch (user_choice) {
      case kManually:
        return ManualAttributes();

      case kPreset:
        return PresetAttributes();

      case kFile:
        return FileAttributes();

      default:
        printf("Invalid input!\n");
        break;
    }
    printf("\n");
  }
}

void PromptInputParameters(InputParameters* user_data_struct) {
  const double kAttributePreferences = 0.25;
  const bool kTransportPreferences = true;
  const int kHoursInDay = 24;
  const int kMinutesInHour = 60;

  const int kFuelEfficiency = 100;
  const int kClockHour = 12;
  const int kClockMinutes = 30;

  bool valid_answer = false;

  UserPreferences user_attributes_generic = {
      kAttributePreferences, kAttributePreferences, kAttributePreferences,
      kAttributePreferences, kTransportPreferences, kTransportPreferences,
      kTransportPreferences, kTransportPreferences, kFuelEfficiency};

  char choice = 0;

  while (!valid_answer) {
    char* choose_prompts = ReadUserInput(
        "Use predefined or manually input? (p = predefined or m = manual) :");

    choice = choose_prompts[0];

    if (choice == kPreset || choice == kManually) {
      valid_answer = true;
    }

    free(choose_prompts);
  }

  if (choice == kPreset) {
    user_data_struct->origin_location = "Frederikssund";
    user_data_struct->destination_location = "Carlsberg";

    user_data_struct->departure_time_mode = true;
    user_data_struct->clock_hour = kClockHour;
    user_data_struct->clock_minute = kClockMinutes;

    user_data_struct->user_attributes = user_attributes_generic;
  } else if (choice == kManually) {
    char* origin_location = ReadUserInput("Start location :");
    char* destination_location = ReadUserInput("End location :");

    user_data_struct->origin_location = origin_location;
    user_data_struct->destination_location = destination_location;

    user_data_struct->departure_time_mode = GetInputBoolean(
        "Do you want to plan routes for Departure time? (y) Or Arrival time? "
        "(n):");

    user_data_struct->clock_hour =
        GetInputInteger("Input arrival/departure hour:", kHoursInDay - 1);
    user_data_struct->clock_minute = GetInputInteger(
        "Input arrival/departure minutes :", kMinutesInHour - 1);

    user_data_struct->user_attributes = PromptAttributes();
  }
}

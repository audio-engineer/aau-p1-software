#ifndef AAU_P1_SOFTWARE_SRC_INPUT_H_
#define AAU_P1_SOFTWARE_SRC_INPUT_H_

enum YesNo { kYes = 'y', kNo = 'n' };
enum UserChoice { kManually = 'm', kFile = 'f', kPreset = 'p' };

typedef struct {
  double price;
  double time;
  double environment;
  double health;
  bool walk;
  bool bike;
  bool car;
  bool train;
  int car_fuel_efficiency;
} UserPreferences;

typedef struct {
  const char* origin_location;
  const char* destination_location;

  // If true search for departure, if false for arrival.
  bool departure_time_mode;
  int clock_hour;
  int clock_minute;
  UserPreferences user_attributes;
} InputParameters;

char* ReadUserInput(const char* message);

void PromptInputParameters(InputParameters* user_data_struct);

#endif  // AAU_P1_SOFTWARE_SRC_INPUT_H_

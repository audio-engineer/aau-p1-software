#ifndef AAU_P1_SOFTWARE_SRC_INPUT_H_
#define AAU_P1_SOFTWARE_SRC_INPUT_H_

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"

typedef enum TimeMode { kDeparture = 'd', kArrival = 'a' } TimeMode;

typedef struct CommutingPreferences {
  double price;
  double time;
  double environment;
  double health;
  bool is_walking;
  bool is_biking;
  bool is_using_train;
  bool is_driving;
  int car_fuel_efficiency;
} CommutingPreferences;

typedef struct TripParameters {
  char* origin_location;
  char* destination_location;
  TimeMode time_mode;
  int time_hour;
  int time_minute;
} TripParameters;

typedef struct UserPreferences {
  CommutingPreferences* commuting_preferences;
  TripParameters* trip_parameters;
} UserPreferences;

/**
 * @brief Generic API for prompting user input.
 *
 * @param message A string with the message that will be displayed to the user
 * in the prompt.
 * @return A pointer to a heap-allocated string.
 */
char* ReadUserInput(const char* message);

/**
 * @brief Gets the users commuting preferences and trip parameters. Should be
 * called at the beginning of the program so that the values can then be used
 * later in the program.
 *
 * @return A pointer to a heap-allocated UserPreferences struct. Should be freed
 * with FreeUserPreferences.
 * @see FreeUserPreferences
 */
UserPreferences* GetUserPreferences();

/**
 * @brief Frees a heap-allocated UserPreferences struct.
 *
 * @param user_preferences A pointer to a heap-allocated UserPreferences struct.
 */
void FreeUserPreferences(UserPreferences* user_preferences);

#endif  // AAU_P1_SOFTWARE_SRC_INPUT_H_

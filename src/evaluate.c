#include "evaluate.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calculations.h"
#include "curl/curl.h"
#include "curl/easy.h"
#include "input.h"
#include "preferences.h"
#include "trip.h"

void CalculateTripData(TripData trip_data_arr[], Trips* trips,
                       CURL* const k_curl,
                       UserPreferences* user_input_parameters) {
  // Iterating through all trips.
  for (size_t trip_index = 0; trip_index < trips->number_of_trips;
       trip_index++) {
    int trip_distance = 0;
    trip_data_arr[trip_index].trip_id = trips->trips[trip_index].trip_id;

    // Iterating through all legs.
    for (size_t leg_index = 0;
         leg_index < trips->trips[trip_index].number_of_legs; leg_index++) {
      // Calculating leg distance
      int leg_distance = 0;
      leg_distance = CalculateLegDistance(k_curl, trips->trips, leg_index);

      // Tracking sum og all legs.
      trip_distance += leg_distance;

      // Calculating attributes for each leg and tracking sum.
      CalculatePriceParameters price_calculation_params = {
          DetermineModeOfTransport(trips->trips[trip_index].legs->type),
          leg_distance, false,
          user_input_parameters->commuting_preferences->car_fuel_efficiency};
      trip_data_arr[trip_index].price +=
          CalculatePrice(&price_calculation_params);

      CalculateCo2Parameters co2_calculation_params = {
          DetermineModeOfTransport(trips->trips[trip_index].legs->type),
          leg_distance,
          user_input_parameters->commuting_preferences->car_fuel_efficiency};
      trip_data_arr[trip_index].environment +=
          CalculateCo2(&co2_calculation_params);
    }
    // Updating final distance for a trip and the time.
    trip_data_arr[trip_index].trip_distance = trip_distance;
    trip_data_arr[trip_index].time = CalculateMinutes(trips->trips[trip_index]);
  }
}

int CalculateMinutes(Trip trip) {
  // Copying strings to local variables.
  const char* const kOriginTime = trip.legs[0].origin->kTime;
  size_t destination_leg_index = trip.number_of_legs - 1;
  const char* const kDestinationTime =
      trip.legs[destination_leg_index].destination->kTime;
  const int kMinutesPrHour = 60;
  const int kHoursPrDay = 24;
  const int kSizeOfTimeStr = 5;

  // Checking if strings are formatted correctly.
  if (strlen(kOriginTime) < kSizeOfTimeStr ||
      strlen(kDestinationTime) < kSizeOfTimeStr) {
    perror("Error: Invalid time format in input strings.");
    exit(EXIT_FAILURE);
  }

  // Initializing the hours and minutes en separate strings.
  char origin_hours[3] = {kOriginTime[0], kOriginTime[1], '\0'};
  char origin_minutes[3] = {kOriginTime[3], kOriginTime[4], '\0'};

  char destination_hours[3] = {kDestinationTime[0], kDestinationTime[1], '\0'};
  char destination_minutes[3] = {kDestinationTime[3], kDestinationTime[4],
                                 '\0'};

  // Calculating total minutes for each time.
  int total_origin_minutes =
      atoi(origin_hours) * kMinutesPrHour + atoi(origin_minutes);
  int total_destination_minutes =
      atoi(destination_hours) * kMinutesPrHour + atoi(destination_minutes);

  // Returning the difference.
  int total_minutes = total_destination_minutes - total_origin_minutes;
  if (total_minutes < 0) {
    total_minutes = kHoursPrDay * kMinutesPrHour + total_destination_minutes -
                    total_origin_minutes;
  }
  return total_minutes;
}

ModeOfTransport DetermineModeOfTransport(char* vehicle) {
  // Finding the mode of transport as parsed by the API.
  ModeOfTransport value = 0;
  int other_train_type =
      strcmp(vehicle, "IC") == 0 || strcmp(vehicle, "LYN") == 0 ||
      strcmp(vehicle, "REG") == 0 || strcmp(vehicle, "TOG") == 0 ||
      strcmp(vehicle, "M") == 0;
  int other_bus_type = strcmp(vehicle, "BUS") == 0 ||
                       strcmp(vehicle, "EXB") == 0 ||
                       strcmp(vehicle, "NB") == 0 || strcmp(vehicle, "TB") == 0;

  if (other_train_type) {
    value = 't';
  } else if (strcmp(vehicle, "S") == 0) {
    value = 's';
  } else if (other_bus_type) {
    value = 'b';
  } else if (strcmp(vehicle, "F") == 0) {
    value = '0';
  } else {
    perror("Error: Type of transportation does not exist in the program.");
    value = 0;
  }
  return value;
}

int CalculateLegDistance(CURL* const k_curl, const Trip* const trip,
                         size_t leg_index) {
  // Initializing a pointer to all coordinates of a leg.
  CoordinatesData* coordinates_for_leg =
      GetCoordinatesForLeg(k_curl, &(trip->legs[leg_index]));
  int distance = 0;

  // Iterating through all coordinates of a leg.
  for (size_t coordinate_index = 0;
       coordinate_index + 1 < coordinates_for_leg->number_of_coordinates;
       coordinate_index++) {
    // Calculating the distances between the coordinates.
    CalculateDistanceParameters distance_params = {
        coordinates_for_leg->coordinates[coordinate_index].latitude,
        coordinates_for_leg->coordinates[coordinate_index].longitude,
        coordinates_for_leg->coordinates[coordinate_index + 1].latitude,
        coordinates_for_leg->coordinates[coordinate_index + 1].longitude};
    // Keeping track of sum.
    distance += (int)CalculateDistance(&distance_params);
  }
  free(coordinates_for_leg);
  return distance;
}

void CalculateScore(
    TripData trip_data[], TripScore trip_score[],
    const CalculateScoreParameters* calculate_score_parameters) {
  size_t num_trips = calculate_score_parameters->kNumTrips;
  size_t read_offset = calculate_score_parameters->kReadOffset;
  size_t write_offset = calculate_score_parameters->kWriteOffset;
  int inverted = calculate_score_parameters->kInverted;

  int attribute_largest = 0;
  int attribute_smallest = 0;
  void* read_member = NULL;
  void* write_member = NULL;

  /* For loop for finding largest and smallest value among the same members in
     difference instances of the struct. */
  for (size_t i = 0; i < num_trips; i++) {
    /* Using pointer arithmetic to navigate to the correct members in each
       instance of the struct. */
    read_member = (void*)((char*)&trip_data[i] + read_offset);

    // Updating largest and smallest attribute value
    if (*(int*)read_member > attribute_largest) {
      attribute_largest = *(int*)read_member;
    }
    if (i == 0) {
      attribute_smallest = *(int*)read_member;
    }
    if (*(int*)read_member < attribute_smallest) {
      attribute_smallest = *(int*)read_member;
    }
  }

  // For loop to update the score for the member in each instance of the struct.
  for (size_t j = 0; j < num_trips; j++) {
    read_member = (void*)((char*)&trip_data[j] + read_offset);
    write_member = (void*)((char*)&trip_score[j] + write_offset);

    double score = 0;
    // Relative score calculation
    if (attribute_largest - attribute_smallest != 0) {
      score = (*(int*)read_member - (double)attribute_smallest) /
              (attribute_largest - attribute_smallest);

      // If big is bad, like price, the score is inverted.
      if (inverted) {
        score = 1.0 - score;
      }
    } else {
      score = 1;
    }

    // Relevant score member is updated.
    *(double*)write_member = score;
  }
}

void Evaluate(TripData data_arr[], TripScore score_arr[], size_t size_arr) {
  // Calculating score.
  CalculateScoreParameters price_params = {size_arr, offsetof(TripData, price),
                                           offsetof(TripScore, price_score), 1};
  CalculateScore(data_arr, score_arr, &price_params);

  CalculateScoreParameters health_params = {
      size_arr, offsetof(TripData, health), offsetof(TripScore, health_score),
      0};
  CalculateScore(data_arr, score_arr, &health_params);

  CalculateScoreParameters time_params = {size_arr, offsetof(TripData, time),
                                          offsetof(TripScore, time_score), 1};
  CalculateScore(data_arr, score_arr, &time_params);

  CalculateScoreParameters environment_params = {
      size_arr, offsetof(TripData, environment),
      offsetof(TripScore, environment_score), 1};
  CalculateScore(data_arr, score_arr, &environment_params);

  // Calculate overall_score using all other scores.
  for (size_t i = 0; i < size_arr; i++) {
    score_arr[i].trip_id = data_arr[i].trip_id;
    double overall_score =
        GetUserPreference("price") * score_arr[i].price_score +
        GetUserPreference("health") * score_arr[i].health_score +
        GetUserPreference("time") * score_arr[i].time_score +
        GetUserPreference("environment") * score_arr[i].environment_score;
    score_arr[i].overall_score = overall_score;
  }
}

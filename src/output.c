#include "output.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evaluate.h"
#include "input.h"
#include "sort.h"
#include "trip.h"

// Function outputs data about the trips.
void Output(TripData data_arr[], TripScore score_arr[], size_t size,
            Trips* trips) {
  // Initial sort and print of the trip data and score data.
  SortRoutes(score_arr, "overall", size);
  PrintTripScoresAndData(data_arr, score_arr, size);

  // Iterating until termination.
  while (1) {
    char* user_choice = ReadUserInput(
        "View route details (number), sort the list (Price (P),Time (T), "
        "Environment (E), Overall (O)) or terminate (Q): ");

    if (IsInteger(user_choice)) {
      long choice = strtol(user_choice, NULL, kBaseTen);
      if (choice > (long)size || choice > kMaxAmountRoutesToPrint) {
        printf(
            "Error: You entered a number that was too large. Please try "
            "again.\n");
        free(user_choice);
        continue;
      }
      PrintRouteDetails(trips, choice, score_arr, size);
    }

    else if (IsCharacter(user_choice)) {
      char choice = user_choice[0];
      switch (choice) {
        case 'q':
        case 'Q':
          free(user_choice);
          return;
        case 'p':
        case 'P':
          SortRoutes(score_arr, "price", size);
          break;
        case 't':
        case 'T':
          SortRoutes(score_arr, "time", size);
          break;
        case 'e':
        case 'E':
          SortRoutes(score_arr, "environment", size);
          break;
        case 'o':
        case 'O':
          SortRoutes(score_arr, "overall", size);
          break;
        default:
          printf("%c does not trigger any functionality. Please try again.\n",
                 choice);
          free(user_choice);
          continue;
      }
      PrintTripScoresAndData(data_arr, score_arr, size);
    } else {
      // If it is neither an integer nor a character, treat it as an error.
      printf("Error: Invalid input. You entered: %s\n", user_choice);
    }
    free(user_choice);
  }
}

void PrintTripScoresAndData(TripData data_arr[], TripScore score_arr[],
                            size_t size) {
  printf("\n");

  // Variable to track the index of the trip_id in the data_arr.
  size_t trip_index = 0;

  // Variable to make sure no empty lines are printed.
  size_t amount_routes_to_print =
      size < kMaxAmountRoutesToPrint ? size : kMaxAmountRoutesToPrint;

  // Iterating over amount of trips to be shown.
  PrintSeparator();
  for (size_t i = 0; i < amount_routes_to_print; i++) {
    // For loop to identify the index of the relevant struct in the data_arr.
    for (trip_index = 0; trip_index < size; trip_index++) {
      if (score_arr[i].trip_id == data_arr[trip_index].trip_id) {
        break;
      }
    }

    // Printing trip data and scores.
    printf("Priority %zu: ", i + 1);
#ifndef NDEBUG
    printf("ID: %3d ", score_arr[i].trip_id);
#endif
    printf("P: %6.2lf ", data_arr[trip_index].price);
    printf("T: %6.2lf ", data_arr[trip_index].time);
    printf("E: %6.2lf ", data_arr[trip_index].environment);
    printf("--- ");
    printf("Ps: %2.2lf ", score_arr[i].price_score);
    printf("Ts: %2.2lf ", score_arr[i].time_score);
    printf("Es: %2.2lf ", score_arr[i].environment_score);
    printf("--- ");
    printf("Os: %2.2lf ", score_arr[i].overall_score);
    printf("\n");
  }
  PrintSeparator();
  printf("\n");
}

bool IsInteger(const char* str) {
  // Function to test for if integer.
  char* end_pointer = NULL;
  strtol(str, &end_pointer, kBaseTen);
  // NOLINTNEXTLINE(readability-implicit-bool-conversion)
  return (*end_pointer == '\0');
}

// Function to test for if str is char.
// NOLINTNEXTLINE(readability-implicit-bool-conversion)
bool IsCharacter(const char* str) { return (strlen(str) == 1); }

void PrintRouteDetails(Trips* trips, long choice, TripScore score_arr[],
                       size_t number_trips) {
  int trip_id = score_arr[choice - 1].trip_id;
  size_t trip_id_index = 0;

  for (trip_id_index = 0; trip_id_index < number_trips; trip_id_index++) {
    if (trips->trips[trip_id_index].trip_id == trip_id) {
      break;
    }
  }
  PrintSeparator();
  printf("Number of legs in the chosen trip: %zu\n",
         trips->trips[trip_id_index].number_of_legs);
  PrintSeparator();
  for (size_t i = 0; i < trips->trips[trip_id_index].number_of_legs; i++) {
    printf("Leg %zu:\n", i + 1);
    printf("Transportation name:       %s\n",
           trips->trips[trip_id_index].legs[i].name);
    printf("Transportation type:       %s\n",
           trips->trips[trip_id_index].legs[i].type);
    printf("\n");
    printf("Origin station name:       %s\n",
           trips->trips[trip_id_index].legs[i].origin->kName);
    printf("Origin station time:       %s\n",
           trips->trips[trip_id_index].legs[i].origin->kTime);
    printf("Origin station date:       %s\n",
           trips->trips[trip_id_index].legs[i].origin->kDate);
    printf("Origin station Track:      %s\n",
           trips->trips[trip_id_index].legs[i].origin->kRtTrack);
    printf("\n");
    printf("Destination station name:  %s\n",
           trips->trips[trip_id_index].legs[i].destination->kName);
    printf("Destination station time:  %s\n",
           trips->trips[trip_id_index].legs[i].destination->kTime);
    printf("Destination station date:  %s\n",
           trips->trips[trip_id_index].legs[i].destination->kDate);
    printf("Destination station Track: %s\n",
           trips->trips[trip_id_index].legs[i].destination->kRtTrack);
    PrintSeparator();
  }
}

void PrintSeparator(void) {
  printf("-----------------------------------------------------------------\n");
}

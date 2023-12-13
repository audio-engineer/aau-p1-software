#include "output.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evaluate.h"
#include "input.h"
#include "sort.h"

#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif

// Function outputs data about the trips.
void Output(TripData data_arr[], TripScore score_arr[], size_t size) {
  // Initial print of the trip data and score data.
  PrintTripScoresAndData(data_arr, score_arr);

  // Iterating until termination.
  while (1) {
    char* user_choice = ReadUserInput(
        "View route details (1-5), sort the list (P,T,E,H,O) or terminate "
        "(Q): ");  // TODO(unknown): Find a way to replace "XXX"

    if (IsInteger(user_choice)) {
      long choice = strtol(user_choice, NULL, kBaseTen);
      printf("You entered: %ld\n", choice);
      if (choice > (long)size) {
        printf("Error");  // TODO(unknown): Better error message.
      }
    }
    // TODO(unknown): Print route details.

    else if (IsCharacter(user_choice)) {
      char choice = user_choice[0];
      printf("You entered: %c\n", choice);
      switch (choice) {
        case 'q':
        case 'Q':
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
        case 'h':
        case 'H':
          SortRoutes(score_arr, "health", size);
          break;
        case 'o':
        case 'O':
          SortRoutes(score_arr, "overall", size);
          break;
        default:
          printf("%c does not trigger any functionality. Please try again.\n",
                 choice);
          break;
      }
      PrintTripScoresAndData(data_arr, score_arr);
    } else {
      // If it is neither an integer nor a character, treat it as an error.
      printf("Error: Invalid input. You entered: %s\n", user_choice);
    }
  }
}

void PrintTripScoresAndData(TripData data_arr[], TripScore score_arr[]) {
  // Function printing trip data and scores.
  printf("\n");
  for (size_t i = 0; i < kAmountRoutesToPrint; i++) {
    printf(
        "Trip %zu: ID: %10d P: %6.2lf T: %6.2lf E: %6.2lf H: %6.2lf --- Ps: "
        "%2.2lf Ts: "
        "%2.2lf Es: %2.2lf Hs: %2.2lf --- Os: %2.2lf.\n",
        i + 1, score_arr[i].trip_id, data_arr[i].price, data_arr[i].time,
        data_arr[i].environment, data_arr[i].health, score_arr[i].price_score,
        score_arr[i].time_score, score_arr[i].environment_score,
        score_arr[i].health_score, score_arr[i].overall_score);
  }
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

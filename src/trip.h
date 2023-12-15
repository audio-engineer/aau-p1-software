#ifndef AAU_P1_SOFTWARE_SRC_TRIP_H_
#define AAU_P1_SOFTWARE_SRC_TRIP_H_

#include <stddef.h>

#include "curl/curl.h"

typedef struct Location {
  const char* const kName;
  const char* const kType;
  const char* const kRouteIdx;
  const char* const kTime;
  const char* const kDate;
  const char* const kTrack;
  const char* const kRtTime;
  const char* const kRtDate;
  const char* const kRtTrack;
} Location;

typedef struct Notes {
  const char* const kText;
} Notes;

typedef struct JourneyDetailRef {
  const char* const kRef;
} JourneyDetailRef;

typedef struct Leg {
  char* name;
  char* type;
  Location* origin;
  Location* destination;
  Notes* notes;
  JourneyDetailRef* journey_detail_ref;
} Leg;

typedef struct Trip {
  int trip_id;
  Leg* legs;
  size_t number_of_legs;
} Trip;

typedef struct Trips {
  int origin_location_id;
  int destination_location_id;
  Trip* trips;
  size_t number_of_trips;
} Trips;

/**
 * @brief Makes an API call to Rejseplanen to get an array of possible trips.
 *
 * @param curl A CURL instance.
 * @param origin The name of the origin location.
 * @param destination The name of the destination location.
 * @return Pointer to a Trips struct containing the trip data from the
 * Rejseplanen API.
 */
Trips* GetTrips(CURL* curl, const char* origin, const char* destination);

void CoordinatesForStations(CURL* curl);

#endif  // AAU_P1_SOFTWARE_SRC_TRIP_H_

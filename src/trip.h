#ifndef AAU_P1_SOFTWARE_SRC_TRIP_H_
#define AAU_P1_SOFTWARE_SRC_TRIP_H_

#include <stddef.h>

#include "curl/curl.h"
#include "input.h"

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

typedef struct Coordinates {
  double latitude;
  double longitude;
} Coordinates;

typedef struct CoordinatesData {
  Coordinates* coordinates;
  size_t number_of_coordinates;
} CoordinatesData;

/**
 * @brief Makes an API call to Rejseplanen to get an array of possible trips.
 *
 * @param curl A CURL instance.
 * @param origin The name of the origin location.
 * @param destination The name of the destination location.
 * @return Pointer to a Trips struct containing the trip data from the
 * Rejseplanen API.
 */
Trips* GetTrips(CURL* curl, TripParameters* trip_parameters);

/**
 * @brief Merges the coordinates for each leg of a trip into a single array.
 *
 * @param curl A CURL instance.
 * @param trip A Trip struct.
 * @return Pointer to a CoordinatesData struct that contains a pointer to an
 * array of Coordinates structs.
 */
CoordinatesData* GetCoordinatesForTrip(CURL* curl, const Trip* trip);

/**
 * @brief Merges the coordinates for a leg of a trip into an array.
 *
 * @param curl A CURL instance.
 * @param trip A Leg struct.
 * @return Pointer to a CoordinatesData struct that contains a pointer to an
 * array of Coordinates structs.
 */
CoordinatesData* GetCoordinatesForLeg(CURL* curl, const Leg* leg);

#endif  // AAU_P1_SOFTWARE_SRC_TRIP_H_

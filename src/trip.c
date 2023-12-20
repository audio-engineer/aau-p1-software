#include "trip.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api_handler.h"
#include "cJSON.h"
#include "globals.h"
#include "input.h"

/**
 * @brief Makes a call to the Rejseplanen API to get the location ID of the
 * nearest matching location.
 *
 * @param curl A CURL instance.
 * @param location The name of the location to be searched for.
 * @return The location ID as a heap-allocated string.
 */
char* GetLocationId(CURL* const curl, const char* const location) {
  char endpoint[kBufferSize] = {0};

  char* const kEncodedLocation =
      curl_easy_escape(curl, location, (int)strlen(location));

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(endpoint, "location?input=", strlen("location?input="));
  strncat(endpoint, kEncodedLocation, strlen(kEncodedLocation));
  strncat(endpoint, "&format=json", strlen("&format=json"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  curl_free(kEncodedLocation);

  Response response = {NULL, 0};

  DoRequest(curl, endpoint, &response, true);

  cJSON* const kStartLocationResponseBody = cJSON_Parse(response.body);
  free(response.body);

  const cJSON* const kLocationList = cJSON_GetObjectItemCaseSensitive(
      kStartLocationResponseBody, "LocationList");

  const cJSON* const kStopLocation =
      cJSON_GetObjectItemCaseSensitive(kLocationList, "StopLocation");

  const cJSON* const kNearestLocation = cJSON_GetArrayItem(kStopLocation, 0);

  if (!kNearestLocation) {
    return 0;
  }

  const char* const kNearestLocationId =
      cJSON_GetObjectItemCaseSensitive(kNearestLocation, "id")->valuestring;

  char* const kNearestLocationIdCopy =
      calloc(strlen(kNearestLocationId), sizeof(char));
  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.strcpy)
  strcpy(kNearestLocationIdCopy, kNearestLocationId);

  cJSON_Delete(kStartLocationResponseBody);

  return kNearestLocationIdCopy;
}

/**
 * @brief Makes a call to the Rejseplanen API to get the array of possible
 * trips.
 *
 * @param curl A CURL instance.
 * @param origin_location_id The location ID of the origin location.
 * @param destination_location_id The location ID of the destination location.
 * @return Pointer to heap-allocated cJSON struct.
 */
cJSON* GetTripArray(CURL* const curl, TripParameters* trip_parameters,
                    const char* const origin_location_id,
                    const char* const destination_location_id) {
  char time[kBufferSize] = {0};

  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  sprintf(time, "%d:%d", trip_parameters->time_hour,
          trip_parameters->time_minute);

  char endpoint[kBufferSize * 2] = {0};

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(endpoint, "trip?originId=", strlen("trip?originId="));
  strncat(endpoint, origin_location_id, strlen(origin_location_id));
  strncat(endpoint, "&destId=", strlen("&destId="));
  strncat(endpoint, destination_location_id, strlen(destination_location_id));
  strncat(endpoint, "&format=json", strlen("&format=json"));
  strncat(endpoint, "&time=", strlen("&time="));
  strncat(endpoint, time, strlen(time));
  strncat(endpoint, "&searchForArrival=", strlen("&searchForArrival="));
  strncat(endpoint, trip_parameters->time_mode == kArrival ? "1" : "0",
          sizeof(char));
  strncat(endpoint, "&useBus=0", strlen("&useBus=0"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  Response response = {NULL, 0};

  DoRequest(curl, endpoint, &response, true);

  cJSON* const kTripResponseBody = cJSON_Parse(response.body);
  free(response.body);

  const cJSON* const kTripList =
      cJSON_GetObjectItemCaseSensitive(kTripResponseBody, "TripList");

  return cJSON_GetObjectItemCaseSensitive(kTripList, "Trip");
}

/**
 * @brief Builds a Location struct using the data extracted from the JSON object
 * found at the "Location" key in a "leg" JSON object.
 *
 * @param location The "Location" JSON object.
 * @return A Location struct.
 */
Location BuildLocationStruct(const cJSON* const location) {
  const cJSON* const kName = cJSON_GetObjectItemCaseSensitive(location, "name");
  const cJSON* const kType = cJSON_GetObjectItemCaseSensitive(location, "type");
  const cJSON* const kRouteIDx =
      cJSON_GetObjectItemCaseSensitive(location, "routeIdx");
  const cJSON* const kTime = cJSON_GetObjectItemCaseSensitive(location, "time");
  const cJSON* const kDate = cJSON_GetObjectItemCaseSensitive(location, "date");
  const cJSON* const kTrack =
      cJSON_GetObjectItemCaseSensitive(location, "track");
  const cJSON* const kRtTime =
      cJSON_GetObjectItemCaseSensitive(location, "rtTime");
  const cJSON* const kRtDate =
      cJSON_GetObjectItemCaseSensitive(location, "rtDate");
  const cJSON* const kRtTrack =
      cJSON_GetObjectItemCaseSensitive(location, "rtTrack");

  return (Location){kName->valuestring,
                    kType->valuestring,
                    kRouteIDx ? kRouteIDx->valuestring : "",
                    kTime ? kTime->valuestring : "",
                    kDate ? kDate->valuestring : "",
                    kTrack ? kTrack->valuestring : "",
                    kRtTime ? kRtTime->valuestring : "",
                    kRtDate ? kRtDate->valuestring : "",
                    kRtTrack ? kRtTrack->valuestring : ""};
}

/**
 * @brief Builds a Leg struct using the data extracted from the JSON object
 * found either as the value at the "Leg" key, or one of the array elements at
 * the "Leg" key.
 *
 * @param leg The "Leg" JSON object.
 * @return A Leg struct.
 */
Leg* BuildLegStruct(const cJSON* const leg) {
  const cJSON* const kOrigin = cJSON_GetObjectItemCaseSensitive(leg, "Origin");
  Location origin = BuildLocationStruct(kOrigin);

  const cJSON* const kDestination =
      cJSON_GetObjectItemCaseSensitive(leg, "Destination");
  Location destination = BuildLocationStruct(kDestination);

  const cJSON* const kNotes = cJSON_GetObjectItemCaseSensitive(leg, "Notes");
  const cJSON* const kNotesText =
      cJSON_GetObjectItemCaseSensitive(kNotes, "text");
  Notes notes = {kNotesText ? kNotesText->valuestring : ""};

  const cJSON* const kJourneyDetailRef =
      cJSON_GetObjectItemCaseSensitive(leg, "JourneyDetailRef");
  JourneyDetailRef journey_detail_ref = {
      kJourneyDetailRef
          ? cJSON_GetObjectItemCaseSensitive(kJourneyDetailRef, "ref")
                ->valuestring
          : ""};

  Leg* leg_struct = calloc(1, sizeof(Leg));
  leg_struct->name = cJSON_GetObjectItemCaseSensitive(leg, "name")->valuestring;
  leg_struct->type = cJSON_GetObjectItemCaseSensitive(leg, "type")->valuestring;
  leg_struct->origin = calloc(1, sizeof(Location));
  leg_struct->destination = calloc(1, sizeof(Location));
  leg_struct->notes = calloc(1, sizeof(Notes));
  leg_struct->journey_detail_ref = calloc(1, sizeof(JourneyDetailRef));

  if (!leg_struct->origin || !leg_struct->destination || !leg_struct->notes ||
      !leg_struct->journey_detail_ref) {
    perror("Could not allocate Leg struct members");

    exit(EXIT_FAILURE);
  }

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memcpy(leg_struct->origin, &origin, sizeof(Location));
  memcpy(leg_struct->destination, &destination, sizeof(Location));
  memcpy(leg_struct->notes, &notes, sizeof(Notes));
  memcpy(leg_struct->journey_detail_ref, &journey_detail_ref,
         sizeof(JourneyDetailRef));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  return leg_struct;
}

Trips* GetTrips(CURL* const curl, TripParameters* trip_parameters) {
  char* const kOriginLocationId =
      GetLocationId(curl, trip_parameters->origin_location);
  char* const kDestinationLocationId =
      GetLocationId(curl, trip_parameters->destination_location);

#ifndef NDEBUG
  printf("Origin location ID: %s\n", kOriginLocationId);
  printf("Destination location ID: %s\n", kDestinationLocationId);
#endif

  Trips* trips = calloc(1, sizeof(Trips));

  if (!trips) {
    free(kOriginLocationId);
    free(kDestinationLocationId);

    perror("Could not allocate Trips struct");

    return NULL;
  }

  cJSON* const kTrips = GetTripArray(curl, trip_parameters, kOriginLocationId,
                                     kDestinationLocationId);

  trips->origin_location_id = atoi(kOriginLocationId);
  trips->destination_location_id = atoi(kDestinationLocationId);

  free(kOriginLocationId);
  free(kDestinationLocationId);

  trips->trips = calloc((size_t)cJSON_GetArraySize(kTrips), sizeof(Trip));

  if (!trips->trips) {
    free(trips);

    perror("Could not allocate trips member in Trips struct");

    return NULL;
  }

  size_t trip_id = 0;

  const cJSON* trip = NULL;
  cJSON_ArrayForEach(trip, kTrips) {
    const cJSON* const kLegs = cJSON_GetObjectItemCaseSensitive(trip, "Leg");

    trips->trips[trip_id].trip_id = (int)trip_id;

    Leg* legs = calloc(1, sizeof(Leg));

    if (!legs) {
      free(trips->trips);
      free(trips);

      perror("Could not allocate legs array");

      return NULL;
    }

    size_t number_of_legs = 0;

    if (cJSON_IsObject(kLegs)) {
      Leg* leg_data = BuildLegStruct(kLegs);

      // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
      memcpy(&(legs[0]), leg_data, sizeof(Leg));
      free(leg_data);

      number_of_legs = 1;
    }

    if (cJSON_IsArray(kLegs)) {
      Leg* legs_old = legs;
      legs = realloc(legs, (size_t)cJSON_GetArraySize(kLegs) * sizeof(Leg));

      if (!legs) {
        free(trips->trips);
        free(trips);
        free(legs_old);

        perror("Could not allocate legs array");

        return NULL;
      }

      const cJSON* leg = NULL;
      cJSON_ArrayForEach(leg, kLegs) {
        Leg* leg_data = BuildLegStruct(leg);

        // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
        memcpy(&(legs[number_of_legs]), leg_data, sizeof(Leg));
        free(leg_data);

        number_of_legs++;
      }
    }

    trips->trips[trip_id].legs = legs;
    trips->trips[trip_id].number_of_legs = number_of_legs;

    trip_id++;
  }

  trips->number_of_trips = trip_id;

  return trips;
}

CoordinatesData* GetCoordinatesForLeg(CURL* const curl, const Leg* const leg) {
  if (!leg->journey_detail_ref || !leg->journey_detail_ref->kRef) {
    perror("Invalid JourneyDetailRef");

    exit(EXIT_FAILURE);
  }

  Response response = {NULL, 0};
  DoRequest(curl, leg->journey_detail_ref->kRef, &response, false);

  cJSON* const kJourneyDetailResponseBody = cJSON_Parse(response.body);
  free(response.body);

  CoordinatesData* coordinates_data = calloc(1, sizeof(CoordinatesData));

  if (!coordinates_data) {
    perror("Could not allocate CoordinatesData");

    exit(EXIT_FAILURE);
  }

  const cJSON* const kJourneyDetail = cJSON_GetObjectItemCaseSensitive(
      kJourneyDetailResponseBody, "JourneyDetail");
  const cJSON* const kStops =
      cJSON_GetObjectItemCaseSensitive(kJourneyDetail, "Stop");

  Coordinates* coordinates =
      calloc((size_t)cJSON_GetArraySize(kStops), sizeof(Coordinates));

  if (!coordinates) {
    free(coordinates_data);

    perror("Could not allocate CoordinatesData");

    exit(EXIT_FAILURE);
  }

  size_t number_of_stops = 0;
  bool origin_found = false;

  const cJSON* stop = NULL;
  cJSON_ArrayForEach(stop, kStops) {
    const cJSON* const kName = cJSON_GetObjectItemCaseSensitive(stop, "name");
    const char* name = kName->valuestring;

    /**
     * Continue iterating until the first stop whose name matches the trip's
     * origin location
     */
    if (!origin_found && 0 != strcmp(leg->origin->kName, name)) {
      continue;
    }

    if (!origin_found) {
      origin_found = true;
    }

    const cJSON* const kLatitude = cJSON_GetObjectItemCaseSensitive(stop, "x");
    const cJSON* const kLongitude = cJSON_GetObjectItemCaseSensitive(stop, "y");

    char* end = NULL;
    const double kLatitudeDouble = strtod(kLatitude->valuestring, &end);
    const double kLongitudeDouble = strtod(kLongitude->valuestring, &end);

    coordinates[number_of_stops].latitude = kLatitudeDouble;
    coordinates[number_of_stops].longitude = kLongitudeDouble;

    number_of_stops++;

    // If the current stop is the trip's destination location: break
    if (!strcmp(leg->destination->kName, name)) {
      break;
    }
  }

  if (0 == number_of_stops) {
    free(coordinates);

    perror("No stops");

    exit(EXIT_FAILURE);
  }

  Coordinates* coordinates_copy = calloc(number_of_stops, sizeof(Coordinates));

  if (!coordinates_copy) {
    free(coordinates);

    perror("Could not allocate Coordinates");

    exit(EXIT_FAILURE);
  }

  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  memcpy(coordinates_copy, coordinates, number_of_stops * sizeof(Coordinates));
  free(coordinates);

  coordinates_data->number_of_coordinates = number_of_stops;
  coordinates_data->coordinates = coordinates_copy;

  cJSON_Delete(kJourneyDetailResponseBody);

  return coordinates_data;
}

CoordinatesData* GetCoordinatesForTrip(CURL* const curl,
                                       const Trip* const trip) {
  const size_t kNumberOfLegs = trip->number_of_legs;

  if (1 > kNumberOfLegs) {
    perror("Invalid number of legs in trip");

    exit(EXIT_FAILURE);
  }

  CoordinatesData* coordinates_data = calloc(1, sizeof(CoordinatesData));
  Coordinates* coordinates = calloc(1, sizeof(Coordinates));

  size_t total_number_of_coordinates = 0;
  size_t leg_index = 0;

  /** TODO(martinkedmenec): Filter out duplicate legs and don't copy them to
   * `coordinates`
   */
  for (leg_index = 0; leg_index < kNumberOfLegs; leg_index++) {
    CoordinatesData* leg_coordinates =
        GetCoordinatesForLeg(curl, &(trip->legs[leg_index]));
    const size_t kLegNumberOfCoordinates =
        leg_coordinates->number_of_coordinates;

#ifndef NDEBUG
    printf("Leg: %zu, number of stops: %zu\n", leg_index + 1,
           kLegNumberOfCoordinates);
#endif

    Coordinates* coordinates_old = coordinates;
    coordinates = realloc(
        coordinates, (total_number_of_coordinates + kLegNumberOfCoordinates) *
                         sizeof(Coordinates));

    if (!coordinates) {
      free(coordinates_data);
      free(coordinates_old);

      perror("Could not allocate Coordinates");

      exit(EXIT_FAILURE);
    }

    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    memcpy(&(coordinates[total_number_of_coordinates]),
           leg_coordinates->coordinates,
           kLegNumberOfCoordinates * sizeof(Coordinates));

    total_number_of_coordinates += kLegNumberOfCoordinates;

    free(leg_coordinates);
  }

  coordinates_data->number_of_coordinates = total_number_of_coordinates;
  coordinates_data->coordinates = coordinates;

#ifndef NDEBUG
  printf("Total number of stops on trip: %zu\n",
         coordinates_data->number_of_coordinates);
#endif

  return coordinates_data;
}

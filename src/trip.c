#include "trip.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api_handler.h"
#include "cJSON.h"
#include "calculations.h"
#include "globals.h"

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

  DoRequest(curl, endpoint, &response);

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
cJSON* GetTripArray(CURL* const curl, const char* const origin_location_id,
                    const char* const destination_location_id) {
  char endpoint[kBufferSize] = {0};

  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
  strncat(endpoint, "trip?originId=", strlen("trip?originId="));
  strncat(endpoint, origin_location_id, strlen(origin_location_id));
  strncat(endpoint, "&destId=", strlen("&destId="));
  strncat(endpoint, destination_location_id, strlen(destination_location_id));
  strncat(endpoint, "&format=json", strlen("&format=json"));
  strncat(endpoint, "&useBus=0", strlen("&useBus=0"));
  // NOLINTEND(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)

  Response response = {NULL, 0};

  DoRequest(curl, endpoint, &response);

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
      cJSON_GetObjectItemCaseSensitive(kJourneyDetailRef, "ref")->valuestring};

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

Trips* GetTrips(CURL* curl, const char* origin, const char* destination) {
  char* const kOriginLocationId = GetLocationId(curl, origin);
  char* const kDestinationLocationId = GetLocationId(curl, destination);

  Trips* trips = calloc(1, sizeof(Trips));

  if (!trips) {
    free(kOriginLocationId);
    free(kDestinationLocationId);

    perror("Could not allocate Trips struct");

    return NULL;
  }

  cJSON* const kTrips =
      GetTripArray(curl, kOriginLocationId, kDestinationLocationId);

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
      legs = realloc(legs, (unsigned long)cJSON_GetArraySize(kLegs));

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

    trips->trips[trip_id].trip_id = (int)trip_id;
    trips->trips[trip_id].legs = legs;
    trips->trips[trip_id].number_of_legs = number_of_legs;

    trip_id++;
  }

  trips->number_of_trips = trip_id;

  return trips;
}

void CoordinatesForStations(CURL* const curl) {
  const char* k_ref =
      "http://webapp.rejseplanen.dk/bin//rest.exe/"
      "journeyDetail?ref=171774%2F109037%2F608648%2F247066%2F86%3Fdate%3D15.12."
      "23%26station_evaId%3D8600798%26format%3Djson";

  Response response = {NULL, 0};
  DoRequest(curl, k_ref, &response);

  cJSON* const kJourneyDetailReponseBody = cJSON_Parse(response.body);

  const cJSON* const kJourneyDetail = cJSON_GetObjectItemCaseSensitive(
      kJourneyDetailReponseBody, "JourneyDetail");
  const cJSON* const kStops =
      cJSON_GetObjectItemCaseSensitive(kJourneyDetail, "Stop");

  int num_stops = cJSON_GetArraySize(kStops);

  for (int i = 0; i < num_stops; ++i) {
    const cJSON* const kStop = cJSON_GetArrayItem(kStops, i);
    const cJSON* const kStopName =
        cJSON_GetObjectItemCaseSensitive(kStop, "name");
    const cJSON* const kStopX = cJSON_GetObjectItemCaseSensitive(kStop, "x");
    const cJSON* const kStopY = cJSON_GetObjectItemCaseSensitive(kStop, "y");

    if (i < num_stops - 1) {
      const cJSON* const kNextStop = cJSON_GetArrayItem(kStops, i + 1);
      const char* k_stop_x_next =
          cJSON_GetObjectItemCaseSensitive(kNextStop, "x")->valuestring;
      const char* k_stop_y_next =
          cJSON_GetObjectItemCaseSensitive(kNextStop, "y")->valuestring;

      char* endptr = NULL;
      // Convert coordinate strings to doubles
      double stop_x = strtod(kStopX->valuestring, &endptr);
      double stop_y = strtod(kStopY->valuestring, &endptr);

      double stop_x_next = strtod(k_stop_x_next, &endptr);
      double stop_y_next = strtod(k_stop_y_next, &endptr);
      double calcdist = CalculateDistance(&(CalculateDistanceParameters){
          stop_y, stop_x, stop_y_next, stop_x_next});

      printf("Stop name: %s\n", kStopName->valuestring);
      printf("Distance between this and next stop in km: %.2f\n", calcdist);
      printf("\n");
    } else {
      printf("Stop name: %s\n", kStopName->valuestring);
      printf("This is the last stop\n");
      printf("\n");
    }
  }

  cJSON_Delete(kJourneyDetailReponseBody);
}

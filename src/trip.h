#ifndef AAU_P1_SOFTWARE_SRC_TRIP_H_
#define AAU_P1_SOFTWARE_SRC_TRIP_H_

#include "curl/curl.h"

/**
 * @brief Makes an API call to Rejseplanen to get the trip data.
 *
 * @param curl
 * @param origin
 * @param destination
 */
void GetTripData(CURL* curl, const char* origin, const char* destination);

#endif  // AAU_P1_SOFTWARE_SRC_TRIP_H_

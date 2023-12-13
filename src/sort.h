#ifndef AAU_P1_SOFTWARE_SRC_SORT_H_
#define AAU_P1_SOFTWARE_SRC_SORT_H_
#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif
#include <output.h>
#include <stddef.h>

void SortRoutes(TripScore* trips, char* attribute, size_t size_of_struct_array);
#endif  // AAU_P1_SOFTWARE_SRC_SORT_H_

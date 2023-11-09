project(library C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB LIBRARY_FILES calculations.h calculations.c)
add_library(${PROJECT_NAME} ${LIBRARY_FILES})

project(library C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
find_package(PkgConfig REQUIRED)

# Fetch cJSON and make it available
FetchContent_Declare(
        cjson
        GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
        GIT_TAG v1.7.16
)
FetchContent_MakeAvailable(cjson)

# Check whether OpenSSL is installed on the system. It's a required dependency of cURL
pkg_search_module(OPENSSL REQUIRED openssl)

if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "OpenSSL found. Using version ${OPENSSL_VERSION}")
else ()
    if (UNIX AND NOT APPLE)
        message(STATUS "On Linux, but no OpenSSL found")
    endif ()
endif ()

# Fetch cURL and make it available
FetchContent_Declare(
        curl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG curl-8_4_0
)
FetchContent_MakeAvailable(curl)

file(GLOB LIBRARY_FILES application.c calculations.c string_builder.c)
add_library(${PROJECT_NAME} ${LIBRARY_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${cjson_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC cjson libcurl)

#[[TODO -Werror disabled due to cJSON throwing errors on compilation. We need to disable errors for cJSON until we find
    a better solution.]]
target_compile_options(cjson PRIVATE -Wno-error)

target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion)

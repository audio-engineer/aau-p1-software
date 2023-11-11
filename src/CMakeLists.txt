project(library C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        cjson
        GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
        GIT_TAG v1.7.16
)
FetchContent_MakeAvailable(cjson)

FetchContent_Declare(
        curl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG curl-8_4_0
)
FetchContent_MakeAvailable(curl)

file(GLOB LIBRARY_FILES application.h application.c calculations.h calculations.c string_builder.h string_builder.c)
add_library(${PROJECT_NAME} ${LIBRARY_FILES})

include_directories(${cjson_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC cjson libcurl)
#[[TODO -Werror Disabled due to cJSON throwing errors on compilation. We need to disable errors for cJSON until we find
a better solution.]]
target_compile_options(cjson PRIVATE -Wno-error)

project(tests CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME} calculations.cc)

target_link_libraries(${PROJECT_NAME} PUBLIC aau_p1_software_library GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

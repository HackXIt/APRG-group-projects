cmake_minimum_required(VERSION 3.10)
project(min_max)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set optimization flags for the Release build type
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Add include directory where the header files are located
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Create a library for the maze-solving logic
add_library(${PROJECT_NAME}_lib src/min_max.cpp)

# Add the main program, linking it to the min_max_lib
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the Google Test executable for testing
enable_testing()
add_executable(${PROJECT_NAME}_test test/test_${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
cmake_minimum_required(VERSION 3.10)
project(hidoku_solver)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include FetchContent module
include(FetchContent)

# Fetch Naxos library
FetchContent_Declare(
        naxos
        GIT_REPOSITORY https://github.com/pothitos/naxos.git
        GIT_TAG master # Replace with the desired version or commit hash
)
FetchContent_MakeAvailable(naxos)

# Add Naxos core and plugins
add_subdirectory(${naxos_SOURCE_DIR}/core ${naxos_BINARY_DIR}/core)

# Ensure Naxos core includes are available for plugins
include_directories(${naxos_SOURCE_DIR}/core)

# Add plugins directory
add_subdirectory(${naxos_SOURCE_DIR}/plugins ${naxos_BINARY_DIR}/plugins)

# Include directories for the main project
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${naxos_SOURCE_DIR}/core
        ${naxos_SOURCE_DIR}/plugins
)

# Create a library for the hidoku-solving logic
add_library(${PROJECT_NAME}_lib src/${PROJECT_NAME}.cpp)

# Add the main program, linking it to the hidoku library and Naxos libraries
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib naxos naxos-mini amorgos)

# Google Test setup
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
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}_lib gtest_main naxos naxos-mini amorgos)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

# Copy the input and expected directories to the build output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/input
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/expected
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/input ${CMAKE_CURRENT_BINARY_DIR}/input
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/expected ${CMAKE_CURRENT_BINARY_DIR}/expected
)

# Ensure the directories are also available for the test executable
add_custom_command(TARGET ${PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/input ${CMAKE_CURRENT_BINARY_DIR}/input
)

add_custom_command(TARGET ${PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/expected ${CMAKE_CURRENT_BINARY_DIR}/expected
)
